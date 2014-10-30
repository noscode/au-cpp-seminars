#include <cstddef>
#include <cstdlib>
#include "my_ostream_file.h"

my_ostream_file* downcast(my_ostream *thiz)
{
    char *ostream_char = reinterpret_cast<char*>(thiz);
    char *ostream_file_char =
        ostream_char - offsetof(my_ostream_file, ostream);
    return reinterpret_cast<my_ostream_file*>(ostream_file_char);
}

my_ostream* my_ostream_file_upcast(my_ostream_file *thiz)
{
    return &thiz->ostream;
}

static void my_ostream_file_print_int(my_ostream *thiz, int i)
{
    fprintf(downcast(thiz)->file_, "%d", i);
}

static void my_ostream_file_print_double(my_ostream *thiz, double d)
{
    fprintf(downcast(thiz)->file_, "%f", d);
}

static void my_ostream_file_print_string(my_ostream *thiz, const char *str)
{
    fprintf(downcast(thiz)->file_, "%s", str);
}

static void my_ostream_file_destroy(my_ostream_file *thiz)
{
    fclose(thiz->file_);
    my_ostream_destroy(&thiz->ostream);
}

const type_id MY_OSTREAM_FILE_TYPE_ID = 2;

static my_ostream_vtable my_ostream_vtable_inst =
{
    MY_OSTREAM_FILE_TYPE_ID,
    my_ostream_file_print_int,
    my_ostream_file_print_double,
    my_ostream_file_print_string
};

static my_ostream_file_vtable my_ostream_file_vtable_inst =
{
    MY_OSTREAM_FILE_TYPE_ID,
    my_ostream_file_destroy
};

void my_ostream_file_init(my_ostream_file *thiz, const char* path)
{
    my_ostream_init(&thiz->ostream);
    thiz->ostream.vtable = &my_ostream_vtable_inst;
    thiz->vtable = &my_ostream_file_vtable_inst;
    thiz->file_ = fopen(path, "w");
}


my_ostream_file* my_ostream_file_new(const char* path)
{
    my_ostream_file *thiz =
        (my_ostream_file*)malloc(sizeof(*thiz));
    my_ostream_file_init(thiz, path);
    return thiz;
}

void my_ostream_file_delete(my_ostream_file *thiz)
{
    my_ostream_file_destroy(thiz);
    free(thiz);
}
