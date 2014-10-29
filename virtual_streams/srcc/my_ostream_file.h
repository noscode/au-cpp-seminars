#pragma once
#include <cstdio>
#include "my_ostream.h"

struct my_ostream_file;
typedef void (*my_ostream_file_destroy_func)(my_ostream_file*);

struct my_ostream_file_vtable
{
    type_id id;
    my_ostream_file_destroy_func destroy;
};

struct my_ostream_file
{
    my_ostream_file_vtable *vtable;
    my_ostream ostream;
    FILE *file_;
};

void my_ostream_file_init(my_ostream_file *thiz, const char* path);
my_ostream* my_ostream_file_upcast(my_ostream_file *thiz);
