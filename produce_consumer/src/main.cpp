#include <iostream>
#include <thread>
#include <mutex>
#include <string>
#include <condition_variable>
#include <atomic>
#include <cstring>
#include <cassert>

// TODO rename to notifying_counter_t
struct evented_counter_t
{
    explicit evented_counter_t(size_t value)
        : value_(value)
        , exit_(false)
    {}
    evented_counter_t(const evented_counter_t &src) = delete;
    evented_counter_t(evented_counter_t &&src) = delete;
    virtual ~evented_counter_t() {}

    size_t get() const noexcept
    {
        return value_.load();
    }

    /*
     * Before destroing this evented_counter_t
     * you should be sure that no threads are waiting
     * inside this evented_counter_t.
     * This function wakes all threads.
     * Call it and then join all the waiters.
     * Then you can call destructor of this object.
     * Yep. This interface is bad.
     */
    // TODO make convenient simple interface for this
    void exit() noexcept
    {
        exit_.store(true);
        value_wait_cond_.notify_all();
    }

    size_t inc() noexcept
    {
        size_t old_value = value_.fetch_add(1);
        value_wait_cond_.notify_all();
        return old_value + 1;
    }

    size_t dec() noexcept
    {
        size_t old_value = value_.fetch_sub(1);
        value_wait_cond_.notify_all();
        return old_value - 1;
    }

    void wait_value(size_t target_value)
    {
        std::unique_lock<std::mutex> lock(value_wait_mutex_);
        value_wait_cond_.wait(lock,
            [&] { return (value_.load() == target_value) || exit_.load(); });
    }

private:
    std::atomic<size_t> value_;
    std::atomic<bool> exit_;
    std::mutex value_wait_mutex_;
    std::condition_variable value_wait_cond_;
};

struct write_info_t
{
    // TODO return true in buf_destroyed if evented_buffer_t
    // is going to be destroyed
    bool buf_destroyed;
    size_t offset;
    size_t size;
};

// TODO rename to notifying_buffer_t
struct evented_buffer_t
{
    explicit evented_buffer_t(size_t size);
    evented_buffer_t(const evented_buffer_t &src) = delete;
    evented_buffer_t(evented_buffer_t &&src) = delete;
    virtual ~evented_buffer_t();

    size_t size() const;
    write_info_t wait_write();
    void write(size_t offset, size_t size, const char *src);
    void read(size_t offset, size_t size, char *dst);
protected:
    const size_t size_;

    std::mutex data_mutex_;
    char *data_;

    std::mutex write_wait_mutex_;
    std::condition_variable write_wait_cond_;
    bool write_occured_;
    evented_counter_t waiters_count_;
    write_info_t last_write_info_;
};

evented_buffer_t::evented_buffer_t(size_t size)
    : size_(size)
    , data_(new char[size])
    , write_occured_(false)
    , waiters_count_(0)
{}

evented_buffer_t::~evented_buffer_t()
{
    delete[] data_;
}

size_t evented_buffer_t::size() const
{
    return size_;
}

write_info_t evented_buffer_t::wait_write()
{
    std::unique_lock<std::mutex> lock(write_wait_mutex_);
    waiters_count_.inc();
    write_wait_cond_.wait(lock, [this] { return write_occured_; });
    waiters_count_.dec();
    return last_write_info_;
}

void evented_buffer_t::write(size_t offset,
        size_t size, const char *src)
{
    std::unique_lock<std::mutex> lock_data(data_mutex_);
    std::unique_lock<std::mutex> lock_write_wait(write_wait_mutex_);

    last_write_info_.offset = offset;
    last_write_info_.size = size;
    std::memcpy(data_ + offset, src, size);
    write_occured_ = true;

    lock_write_wait.unlock();
    write_wait_cond_.notify_all();

    waiters_count_.wait_value(0);
    lock_write_wait.lock();
    write_occured_ = false;
}

void evented_buffer_t::read(size_t offset,
        size_t size, char *dst)
{
    std::lock_guard<std::mutex> lock(data_mutex_);
    std::memcpy(dst, data_ + offset, size);
}

std::atomic<bool> finish;
const char write_data[] = "0xDEADWRITE";
static std::mutex log_mutex;
#define NUM_READERS 3

static void reader_func_evented_counter(size_t id, evented_counter_t *evcnt)
{
    std::unique_lock<std::mutex> log_lock(
        log_mutex, std::defer_lock);
    size_t target_value = 10;
    while(!finish.load()) {
        evcnt->wait_value(target_value);
        target_value = target_value == 0 ? 10 : 0;

        log_lock.lock();
        std::cout << "[READER " << id << "]"
            << " Observed counter value " << evcnt->get()
            << std::endl;
        log_lock.unlock();
    }
}

static void writer_func_evented_counter(evented_counter_t *evcnt)
{
    std::unique_lock<std::mutex> log_lock(log_mutex, std::defer_lock);
    size_t steps_count = 0;
    bool up = true;
    while(steps_count < 100) {
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        if (up)
            evcnt->inc();
        else
            evcnt->dec();

        if (evcnt->get() == 10)
            up = false;
        else if (evcnt->get() == 0)
            up = true;

        log_lock.lock();
        std::cout << "[WRITER]"
            << " changed to " << evcnt->get()
            << std::endl;
        log_lock.unlock();

        ++steps_count;
    }
}

static void run_evented_counter()
{
    std::unique_lock<std::mutex> log_lock(log_mutex, std::defer_lock);
    finish.store(false);

    evented_counter_t evcnt(0);
    std::thread reader_threads[NUM_READERS];
    for (size_t reader_id = 0; reader_id < NUM_READERS; ++reader_id)
    {
        reader_threads[reader_id] = std::move(
                std::thread(reader_func_evented_counter, reader_id, &evcnt)
        );
    }
    std::thread writer_thread(writer_func_evented_counter, &evcnt);

    log_lock.lock();
    std::cout << "[MAIN] Threads are created. Waiting..." << std::endl;
    log_lock.unlock();

    writer_thread.join();
    finish.store(true);
    evcnt.exit();

    for (size_t i = 0; i < NUM_READERS; ++i)
        reader_threads[i].join();
}

static void reader_func_evented_buffer(size_t id, evented_buffer_t *evbuf)
{
    std::unique_lock<std::mutex> log_lock(
        log_mutex, std::defer_lock);
    char *data = new char[evbuf->size()];
    while(!finish.load()) {
        write_info_t wif = evbuf->wait_write();
        evbuf->read(wif.offset, wif.size, data);
        assert(!std::strcmp(data, write_data));

        log_lock.lock();
        std::cout << "[READER " << id << "]"
            << " Observed write to " << wif.offset
            << " of size " << wif.size
            << " " << std::string(data)
            << std::endl;
        log_lock.unlock();
    }
    delete[] data;
}

static void writer_func_evented_buffer(evented_buffer_t *evbuf)
{
    std::unique_lock<std::mutex> log_lock(
        log_mutex, std::defer_lock);
    const size_t write_size = sizeof(write_data);
    size_t write_pos = 0;
    while(write_pos + write_size < evbuf->size()) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        log_lock.lock();
        std::cout << "[WRITER]"
            << " Write to " << write_pos
            << " of size " << write_size
            << std::endl;
        log_lock.unlock();

        evbuf->write(write_pos, write_size, write_data);
        write_pos += write_size;
    }
}

static void run_evented_buffer()
{
    std::unique_lock<std::mutex> log_lock(log_mutex, std::defer_lock);
    finish.store(false);

    evented_buffer_t evbuf(256);
    std::thread reader_threads[NUM_READERS];
    for (size_t reader_id = 0; reader_id < NUM_READERS; ++reader_id)
    {
        reader_threads[reader_id] = std::move(
                std::thread(reader_func_evented_buffer, reader_id, &evbuf)
        );
    }
    std::thread writer_thread(writer_func_evented_buffer, &evbuf);

    log_lock.lock();
    std::cout << "[MAIN] Threads are created. Waiting..." << std::endl;
    log_lock.unlock();

    writer_thread.join();
    finish.store(true);
    // last readers wakeup
    // TODO make convenient interface for this
    evbuf.write(0, 0, nullptr);

    for (size_t i = 0; i < NUM_READERS; ++i)
        reader_threads[i].join();
}

int main()
{
    run_evented_counter();
    run_evented_buffer();
    return 0;
}
