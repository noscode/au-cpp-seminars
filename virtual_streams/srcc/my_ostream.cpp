#include <cstdlib>
#include "my_ostream.h"

static void pure_virt_call_int_func(my_ostream*, int i)
{
    abort();
}

static void pure_virt_call_double_func(my_ostream*, double d)
{
    abort();
}

static void pure_virt_call_string_func(my_ostream*, const char *str)
{
    abort();
}

const type_id MY_OSTREAM_TYPE_ID = 1;

static my_ostream_vtable my_ostream_vtable_inst =
{
    MY_OSTREAM_TYPE_ID,
    pure_virt_call_int_func,
    pure_virt_call_double_func,
    pure_virt_call_string_func
};

void my_ostream_init(my_ostream *thiz)
{
    thiz->vtable = &my_ostream_vtable_inst;
}

void my_ostream_destroy(my_ostream *thiz)
{
    // Nothing to destroy
    (void)thiz;
}

my_ostream& operator<<(my_ostream &out, int i)
{
    out.vtable->print_int(&out, i);
    return out;
}

my_ostream& operator<<(my_ostream &out, double d)
{
    out.vtable->print_double(&out, d);
    return out;
}

my_ostream& operator<<(my_ostream &out, const char *str)
{
    out.vtable->print_string(&out, str);
    return out;
}
