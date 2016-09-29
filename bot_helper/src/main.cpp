#include <cstring>
#include <cstdio>
#include <iostream>
#include <limits>

#include <unistd.h>
#include <sys/mman.h>
#include <errno.h>

using namespace std;

static int calc_res = 0;

static void cmd_calc_add_int()
{
    int num;

    cin >> num;
    calc_res += num;
}

static void cmd_calc_sub_int()
{
    int num;

    cin >> num;
    calc_res -= num;
}

static void cmd_calc_print_res()
{
    cout << ">> " << calc_res << std::endl;
}

static void cmd_calc_add_float()
{
    float num;

    cin >> num;
    calc_res += static_cast<int>(num);
}

static void cmd_google()
{
    char input[512];

    cin.getline(input, sizeof(input));
    cout << "https://www.google.ru/search?q=";

    // -1: skip last '\n' char
    for (streamsize i = 0; i < cin.gcount() - 1; ++i) {
        if (input[i] == ' ')
            cout << "%20";
        else
            cout << input[i];
    }
    cout << endl;
}

static bool is_palindrome(char *str, size_t len)
{
    for (size_t i = 0; i < len / 2; ++i)
        if (str[i] != str[len - 1 - i])
            return false;

    return true;
}

static void cmd_max_palindrome()
{
    char input[512];
    size_t input_len = 0;
    size_t pali_max_start_ix = 0;
    size_t pali_max_end_ix = 0;
    size_t pali_max_len = 1;

    cin.getline(input, sizeof(input));
    input_len = static_cast<size_t>(cin.gcount());
    if (input_len == 0)
        return;

    for (size_t i = 0; i < input_len; ++i)
        for (size_t j = 0; j < input_len; ++j) {
            char * const start = input + i;
            const size_t len = j - i + 1;

            if ((len > pali_max_len) && is_palindrome(start, len)) {
                pali_max_start_ix = i;
                pali_max_end_ix = j;
                pali_max_len = len;
            }
        }
    input[pali_max_end_ix + 1] = '\0';
    cout << ">> " << input + pali_max_start_ix << endl;
}

static void print_mem(char *mem, size_t size)
{
    for (size_t i = 0; i < size; ++i)
        cout << mem[i];
    cout << endl;
}

static void cmd_mem_print()
{
    void *ptr = nullptr;
    size_t size = 0;

    cin >> ptr;
    cin >> size;
    print_mem(static_cast<char*>(ptr), size);
}

static void cmd_mem_print_valid()
{
    const size_t PAGE_SIZE = 4096;
    void *ptr = nullptr;
    size_t size = 0;

    cin >> ptr;
    cin >> size;
    ptr = (void*)((size_t)ptr / PAGE_SIZE * PAGE_SIZE); // align to page address
    const size_t PAGES_CNT = (size + PAGE_SIZE - 1) / PAGE_SIZE;

    char *valid_vma_begin = static_cast<char*>(ptr);
    size_t valid_vma_pages = 0;
    for (size_t pg = 0; pg < PAGES_CNT; ++pg) {
        unsigned char vec;
        char *page = valid_vma_begin + valid_vma_pages * PAGE_SIZE;

        if (mincore(page, PAGE_SIZE, &vec) == -1) {
            if (errno == ENOMEM) {
                // Page is not mapped - it is invalid.
                // Doesn't work if page is mapped but not readable.
                // To fix this we need to use /proc/self/maps or SIGSEGV handler.
                if (valid_vma_pages) {
                    cout << valid_vma_begin << " " << valid_vma_pages * PAGE_SIZE << ": ";
                    print_mem(valid_vma_begin, valid_vma_pages * PAGE_SIZE);
                }
                valid_vma_begin += (valid_vma_pages + 1) * PAGE_SIZE;
                valid_vma_pages = 0;
            } // else some error has occured
        } else
            valid_vma_pages += 1;
    }

    if (valid_vma_pages)
        print_mem(valid_vma_begin, valid_vma_pages * PAGE_SIZE);
}

static void run_helper_bot()
{
    char cmd;

    do {
        cin.clear();
        cin >> cmd;
        cin.ignore(1); // ignore next space delimeter

        switch (cmd) {
            case 'a':
                cmd_calc_add_int();
                break;
            case 's':
                cmd_calc_sub_int();
                break;
            case 'r':
                cmd_calc_print_res();
                break;
            case 'f':
                cmd_calc_add_float();
                break;
            case 'g':
                cmd_google();
                break;
            case 'p':
                cmd_max_palindrome();
                break;
            case 'm':
                cmd_mem_print();
                break;
            case 'v':
                cmd_mem_print_valid();
                break;
            default:
                cerr << "Unknown command!" << std::endl;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        cin.clear();
    } while (true);
}

int main()
{
    run_helper_bot();
    return 0;
}
