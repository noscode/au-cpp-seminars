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
    virtual ~evented_buffer_t();
    size_t size() const;
    write_info_t wait_write();
    void write(size_t offset, size_t size, const char *src);
    void read(size_t offset, size_t size, char *dst);
};

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
    // TODO 
    return 0;
}
