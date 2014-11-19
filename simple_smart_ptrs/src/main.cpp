#include <iostream>
#include <utility>
#include "scoped_ptr.h"
#include "unique_ptr.h"
#include "shared_ptr.h"
#include "pimpl_example.h"

struct foo
{
    int foo1;
    int foo2;
};

std::ostream& operator<<(std::ostream &out, const foo &f)
{
    return out << f.foo1 << " " << f.foo2;
}

void bool_1st_arg_func(bool b)
{
    (void)b;
}

int main()
{
    scoped_ptr<foo> foo_ptr(new foo {0, 111});
    //foo_ptr = std::move(foo_ptr); // - fails
    //auto foo_ptr2(foo_ptr);// - fails
    //foo_ptr = foo_ptr; // - fails
    std::cout << *foo_ptr << std::endl
        << foo_ptr->foo1 << std::endl
        << foo_ptr.get() << std::endl;
    if (foo_ptr)
    {
        foo_ptr->foo1 += 1;
    }
    foo_ptr.reset(nullptr);
    //bool_1st_arg_func(foo_ptr); // - fails
    decltype(foo_ptr)::element_type foo_stack { 999, 1000 };
    (void)foo_stack;

    pimpl_example pex;
    (void)pex;

    unique_ptr<foo> foo_uptr(new foo {666, 777});
    unique_ptr<foo> foo_uptr2 = std::move(foo_uptr); // - ok
    foo_uptr2 = std::move(foo_uptr2); // - ok
    std::cerr << foo_uptr2->foo1 << std::endl;
    //std::cerr << foo_uptr->foo1 << std::endl; //SIGSEGV
    //auto foo_uptr2(foo_uptr);// - fails

    shared_ptr<foo> foo_shptr(new foo {888, 999});
    auto foo_shptr2 = foo_shptr;
    std::cerr << foo_shptr.get() << std::endl;
    std::cerr << foo_shptr2.get() << std::endl;
    return 0;
}
