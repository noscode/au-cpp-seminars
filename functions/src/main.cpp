#include <iostream>
#include <vector>
#include <functional>
#include <cstdio>
#include <cstdint>
#include <cassert>
#include <cstring>
#include <utility>

// == Task 1 ==
static void for_each(std::vector<int> &vec, const std::function<void(int)> func)
{
    for (size_t i = 0; i < vec.size(); ++i)
         func(vec[i]);
}

static void print(int val)
{
    std::cout << val << " ";
}

static void task1()
{
    std::vector<int> vec { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    for_each(vec, [](int val) { print(val); });
    std::cout << std::endl;
    for_each(vec, print);
    std::cout << std::endl;
}

// == Task 2 ==
int vec_sum(const std::vector<int> &vec, int (*trans)(int))
{
    int sum = 0;
    for (auto val : vec)
        sum += trans(val);

    return sum;
}

static int plus3(int val)
{
    return val + 3;
}

static void task2()
{
    std::cout << vec_sum({ 1, 2, 3, 4, 5 }, plus3) << std::endl;
}

// == Task 3 ==
static void read(FILE *f, uint64_t &val)
{
    fread(&val, sizeof(val), 1, f);
}

static void read(FILE *f, double &val)
{
    fread(&val, sizeof(val), 1, f);
}

static void read(FILE *f, int &val)
{
    fread(&val, sizeof(val), 1, f);
}

static void read(FILE *f, const char *(&val))
{
    long str_pos = ftell(f);
    size_t str_len = 0;
    while (fgetc(f) != '\0')
        ++str_len;
    fseek(f, str_pos, SEEK_SET);
    char *str = static_cast<char*>(malloc(str_len + 1));
    for (size_t i = 0; i <= str_len; ++i)
        str[i] = fgetc(f);
    fgetc(f); // skip '\0'

    val = str;
}

static void write(FILE *f, uint64_t val)
{
    fwrite(&val, sizeof(val), 1, f);
}

static void write(FILE *f, double val)
{
    fwrite(&val, sizeof(val), 1, f);
}

static void write(FILE *f, int val)
{
    fwrite(&val, sizeof(val), 1, f);
}

static void write(FILE *f, const char *val)
{
    while (*val) {
        fputc(*val, f);
        ++val;
    }
    fputc('\0', f);
}

static void task3()
{
    FILE *out = fopen("./file.bin", "wb");
    write(out, static_cast<uint64_t>(64));
    write(out, 22.22);
    write(out, 22);
    write(out, "Hello C++!");
    fclose(out);

    FILE *in = fopen("./file.bin", "rb");
    uint64_t val_u64;
    double val_d;
    int val_int;
    const char *val_str;
    read(in, val_u64);
    read(in, val_d);
    read(in, val_int);
    read(in, val_str);
    assert(val_u64 == 64);
    assert(val_d == 22.22);
    assert(val_int == 22);
    assert(!strcmp("Hello C++!", val_str));
    fclose(in);
    free(const_cast<char*>(val_str));
}

// == Task 4 ==
struct event_t
{
    typedef void* arg_t;
    typedef std::function<void(arg_t)> handler_t;

    void subscribe(handler_t handler);
    void fire(arg_t arg);
private:
    std::vector<handler_t> handlers_;
};

void event_t::subscribe(handler_t handler)
{
    handlers_.push_back(std::move(handler));
}

void event_t::fire(arg_t arg)
{
    for (auto &handler : handlers_)
        handler(arg);
}

static void task4()
{
    event_t ev;
    char harg[] = "Hello event!";
    bool h1 = false;
    bool h2 = false;

    ev.subscribe([&](void *arg) { h1 = true; assert(arg == harg); });
    ev.subscribe([&](void *arg) { h2 = true; assert(arg == harg); });
    ev.fire(harg);
    assert(h1 && h2);
}

int main()
{
    task1();
    task2();
    task3();
    task4();
    return 0;
}
