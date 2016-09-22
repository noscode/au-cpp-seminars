#include <iostream>
#include <cassert>
#include <cstring>

static char *stack_addr(size_t n)
{
    char a;
    char *aaddr;
    if (n == 0) {
        aaddr = &a;
        return aaddr;
    }
    return stack_addr(n - 1);
}

static void check_addrs_order()
{
    assert(stack_addr(10) > stack_addr(11));
    assert(stack_addr(1) > stack_addr(3));

    int *ap = new int[100000];
    int *bp = new int[100000];
    int *cp = new int[100000];
    assert(ap < bp);
    assert(bp < cp);

    delete[] ap;
    delete[] bp;
    delete[] cp;
}

static size_t stair_steps_count()
{
    const size_t N = 21;
    static size_t stair_steps[N] = {};

    memset(stair_steps, 0, sizeof(stair_steps));
    stair_steps[0] = 1;
    stair_steps[1] = 2;

    for (size_t i = 2; i < N; ++i)
        stair_steps[i] = stair_steps[i - 1] + stair_steps[i - 2];

    return stair_steps[N - 1];
}

static const char* concat(const char a[], const size_t a_size,
        const char b[], const size_t b_size, size_t &concat_size)
{
    char *car = new char[a_size + b_size];
    memcpy(car, a, a_size);
    memcpy(car + a_size, b, b_size);
    concat_size = a_size + b_size;
    return car;
}

int **create_matrix(size_t m, size_t n)
{
    int *mem = new int[m * n];
    int **matrix = new int*[m];

    for (size_t i = 0; i < m; ++i)
        matrix[i] = mem + i * n;

    return matrix;
}

void delete_matrix(int **matrix)
{
    delete[] matrix[0];
    delete[] matrix;
}

int main()
{
    check_addrs_order();

    std::cout << "stair steps count: " <<
        stair_steps_count() << std::endl;

    const char ara[] = { 'H', 'e', 'l', 'l', 'o', ' ' };
    const char arb[] = { 'w', 'o', 'r', 'l', 'd', '\0' };
    size_t arc_size = 0;
    const char *arc = concat(ara, sizeof(ara), arb, sizeof(arb), arc_size);
    assert(arc_size == 12);
    assert(!memcmp(arc, "Hello world", 12));
    delete[] arc;

    int **m = create_matrix(10, 5);
    for (size_t i = 0; i < 10; ++i)
        for (size_t j = 0; j < 5; ++j)
            m[i][j] = i + j;
    delete_matrix(m);

    std::cout << "Ok!" << std::endl;
    return 0;
}
