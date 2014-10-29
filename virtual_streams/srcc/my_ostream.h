#pragma once

typedef size_t type_id;

struct my_ostream;
typedef void (*my_ostream_print_int_func)(my_ostream*, int i);
typedef void (*my_ostream_print_double_func)(my_ostream*, double d);
typedef void (*my_ostream_print_string_func)(my_ostream*, const char *str);

struct my_ostream_vtable
{
    type_id id;
    my_ostream_print_int_func print_int;
    my_ostream_print_double_func print_double;
    my_ostream_print_string_func print_string;
};

struct my_ostream
{
    my_ostream_vtable *vtable;
};

void my_ostream_init(my_ostream *thiz);
void my_ostream_destroy(my_ostream *thiz);

my_ostream& operator<<(my_ostream &out, int i);
my_ostream& operator<<(my_ostream &out, double d);
my_ostream& operator<<(my_ostream &out, const char *str);
