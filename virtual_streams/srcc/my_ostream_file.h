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

/*
 * Required auxilary functions
 */
//Constructor
void my_ostream_file_init(my_ostream_file *thiz, const char* path);
//Operator new for only 1 existing public constructor
my_ostream_file* my_ostream_file_new(const char* path);
//Copy constructors and assignment operators are ommited
//Destructor is virtual and is stored in vtable
//Operator delete
void my_ostream_file_delete(my_ostream_file *thiz);
//static_cast<my_ostream*>(my_ostream_file*)
my_ostream* my_ostream_file_upcast(my_ostream_file *thiz);
//dynamic_cast<my_ostream_file*>(my_ostream*)
my_ostream_file* downcast(my_ostream *thiz);
