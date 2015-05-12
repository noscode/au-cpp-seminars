#include <iostream>
#include <thread>
#include <mutex>
#include <string>
#include <condition_variable>
#include <cstring>
#include <cassert>

struct write_info_t
{
    size_t offset;
    size_t size;
};

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
    std::mutex mutex_;
    char *data_;
    size_t size_;

    std::condition_variable write_wait_cond_;
    std::mutex write_wait_mutex_;
    volatile bool write_occured_;
    volatile size_t waiters_count_;
    write_info_t last_write_info_;
};

evented_buffer_t::evented_buffer_t(size_t size)
    : data_(new char[size])
    , size_(size)
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
    waiters_count_++;
    write_wait_cond_.wait(lock,
            [this] { return write_occured_; });
    waiters_count_--;
    return last_write_info_;
}

void evented_buffer_t::write(size_t offset,
        size_t size, const char *src)
{
    std::unique_lock<std::mutex> lock_data(mutex_, std::defer_lock);
    std::unique_lock<std::mutex> lock_write_wait(
            write_wait_mutex_, std::defer_lock);
    lock_data.lock();
    lock_write_wait.lock();

    last_write_info_.offset = offset;
    last_write_info_.size = size;
    std::memcpy(data_ + offset, src, size);
    write_occured_ = true;

    lock_write_wait.unlock();
    write_wait_cond_.notify_all();

    while(waiters_count_)
        std::this_thread::yield();
    lock_write_wait.lock();
    write_occured_ = false;
}

void evented_buffer_t::read(size_t offset,
        size_t size, char *dst)
{
    std::lock_guard<std::mutex> lock(mutex_);
    std::memcpy(dst, data_ + offset, size);
}

volatile bool finish = false;
const char write_data[] = "0xDEADWRITE";
static std::mutex log_mutex;

static void reader_func(size_t id, evented_buffer_t *evbuf)
{
    std::unique_lock<std::mutex> log_lock(
        log_mutex, std::defer_lock);
    char *data = new char[evbuf->size()];
    while(!finish) {
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

static void writer_func(evented_buffer_t *evbuf)
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
    finish = true;
    // last wakeup
    evbuf->write(write_pos, 0, write_data);
}

#define NUM_READERS 3
int main()
{
    std::unique_lock<std::mutex> log_lock(
        log_mutex, std::defer_lock);
    evented_buffer_t evbuf(256);
    std::thread reader_threads[NUM_READERS];
    for (size_t reader_id = 0; reader_id < NUM_READERS; ++reader_id)
    {
        reader_threads[reader_id] = std::move(
                std::thread(reader_func, reader_id, &evbuf)
        );
    }
    std::thread writer_thread(writer_func, &evbuf);

    log_lock.lock();
    std::cout << "[MAIN] Threads are created. Waiting..." << std::endl;
    log_lock.unlock();

    for (size_t i = 0; i < NUM_READERS; ++i)
        reader_threads[i].join();
    writer_thread.join();

    return 0;
}
