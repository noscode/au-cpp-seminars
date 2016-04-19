#include <string>
#include <iostream>
#include <cassert>

#include "injector.h"

/* Tested dependencies tree:
 *
 * config1_str config2_int
 *       |      |
 * ----- service1------
 * |        |         |
 * |     service2  service3
 * |        |         |
 * |-----service4-----|
 */

// config1_int
static const std::string CONFIG1_INT_ID = "CONFIG1_INT";
static int *config1_int;

// config1_str
static const std::string CONFIG1_STR_ID = "CONFIG1_STR";
static std::string *config1_str;

// service1
struct service1_t
{
    service1_t(std::string &config)
        : config_(config)
    {}

    void say_hello()
    {
        std::cout << "Hello form service1 "
            << this
            << " I got " << config_ << std::endl;
    }
private:
    std::string &config_;
};

namespace service1_di
{
    static constexpr const char *ID = "SERVICE1";
    static const di::dep_decls_t DEPS = {
        di::make_dep<std::string>(CONFIG1_STR_ID)
    };

    static size_t create_count = 0;
    static service1_t* create(di::dep_insts_t &deps)
    {
        ++create_count;
        return new service1_t(deps.get<std::string>(CONFIG1_STR_ID));
    }
};

// service2
struct service2_t
{
    service2_t(service1_t &service1)
        : service1_(service1)
    {}

    void say_hello()
    {
        std::cout << "Hello form service2 "
            << this
            << " I got " << &service1_ << std::endl;
    }
private:
    service1_t &service1_;
};

namespace service2_di
{
    static constexpr const char *ID = "SERVICE2";
    static const di::dep_decls_t DEPS = {
        di::make_dep<service1_t>(service1_di::ID)
    };

    static size_t create_count = 0;
    static service2_t* create(di::dep_insts_t &deps)
    {
        ++create_count;
        return new service2_t(deps.get<service1_t>(service1_di::ID));
    }
};

// service3
struct service3_t
{
    service3_t(service1_t &service1)
        : service1_(service1)
    {}

    void say_hello()
    {
        std::cout << "Hello form service3 "
            << this
            << " I got " << &service1_ << std::endl;
    }
private:
    service1_t &service1_;
};

namespace service3_di
{
    static constexpr const char *ID = "SERVICE3";
    static const di::dep_decls_t DEPS = {
        di::make_dep<service1_t>(service1_di::ID)
    };

    static size_t create_count = 0;
    static service3_t* create(di::dep_insts_t &deps)
    {
        ++create_count;
        return new service3_t(deps.get<service1_t>(service1_di::ID));
    }
};

// service4
struct service4_t
{
    service4_t(service1_t &service1, service2_t &service2, service3_t &service3)
        : service1_(service1)
        , service2_(service2)
        , service3_(service3)
    {}

    void say_hello()
    {
        std::cout << "Hello form service4 "
            << this
            << " I got"
            << " " << &service1_
            << " " << &service2_
            << " " << &service3_
            << std::endl;
    }
private:
    service1_t &service1_;
    service2_t &service2_;
    service3_t &service3_;
};

namespace service4_di
{
    static constexpr const char *ID = "SERVICE4";
    static const di::dep_decls_t DEPS = {
        di::make_dep<service1_t>(service1_di::ID),
        di::make_dep<service2_t>(service2_di::ID),
        di::make_dep<service3_t>(service3_di::ID)
    };

    static size_t create_count = 0;
    static service4_t* create(di::dep_insts_t &deps)
    {
        ++create_count;
        return new service4_t(
            deps.get<service1_t>(service1_di::ID),
            deps.get<service2_t>(service2_di::ID),
            deps.get<service3_t>(service3_di::ID)
        );
    }
};

// This should fail in tests
namespace circular_di
{
    static constexpr const char *CIRCULAR1_ID = "cr1";
    static constexpr const char *CIRCULAR2_ID = "cr2";
    static constexpr const char *CIRCULAR3_ID = "cr3";
    static constexpr const char *CIRCULAR4_ID = "cr4";
    static constexpr const char *CIRCULAR5_ID = "cr5";

    static const di::dep_decls_t DEPS1 = {
        di::make_dep<service1_t>(service1_di::ID),
        di::make_dep<std::string>(CIRCULAR5_ID)
    };
    static const di::dep_decls_t DEPS2 = {
        di::make_dep<std::string>(CIRCULAR1_ID)
    };
    static const di::dep_decls_t DEPS3 = {
        di::make_dep<std::string>(CIRCULAR2_ID)
    };
    static const di::dep_decls_t DEPS4 = {
        di::make_dep<std::string>(CIRCULAR3_ID)
    };
    static const di::dep_decls_t DEPS5 = {
        di::make_dep<std::string>(CIRCULAR4_ID)
    };

    static int* create(di::dep_insts_t &deps)
    {
        return new int(777);
    }
};

namespace broken_factory_di
{
    static constexpr const char *ID = "broken_di";

    static const di::dep_decls_t DEPS = {};
 
    static std::string* create(di::dep_insts_t &deps)
    {
        return new std::string(deps.get<std::string>(CONFIG1_STR_ID));
    }
};

int main()
{
    // Part 1: only valid usage of injector
    di::injector_t &injector = di::injector_t::instance();
    injector.factory<service4_t>(
        service4_di::ID, service4_di::create, service4_di::DEPS
    );
    injector.factory<service2_t>(
        service2_di::ID, service2_di::create, service2_di::DEPS
    );
    injector.factory<service1_t>(
        service1_di::ID, service1_di::create, service1_di::DEPS
    );
    injector.factory<service3_t>(
        service3_di::ID, service3_di::create, service3_di::DEPS
    );
    config1_str = new std::string(
        "I am string config parameter taken from any place"
    );
    injector.value<std::string>(CONFIG1_STR_ID, config1_str);
    config1_int = new int(777);
    injector.value<int>(CONFIG1_INT_ID, config1_int);

    injector.get<service3_t>(service3_di::ID).say_hello();
    injector.get<service1_t>(service1_di::ID).say_hello();
    injector.get<service4_t>(service4_di::ID).say_hello();
    injector.get<service2_t>(service2_di::ID).say_hello();
    std::cout << injector.get<std::string>(CONFIG1_STR_ID) << std::endl;
    std::cout << injector.get<int>(CONFIG1_INT_ID) << std::endl;

    assert(service1_di::create_count == 1);
    assert(service2_di::create_count == 1);
    assert(service3_di::create_count == 1);
    assert(service4_di::create_count == 1);

    // part 2: circular dependencies
    try {
        injector.factory<int>(
            circular_di::CIRCULAR1_ID, &circular_di::create, circular_di::DEPS1
        );
        injector.factory<int>(
            circular_di::CIRCULAR2_ID, &circular_di::create, circular_di::DEPS2
        );
        injector.factory<int>(
            circular_di::CIRCULAR3_ID, &circular_di::create, circular_di::DEPS3
        );
        injector.factory<int>(
            circular_di::CIRCULAR4_ID, &circular_di::create, circular_di::DEPS4
        );
        injector.factory<int>(
            circular_di::CIRCULAR5_ID, &circular_di::create, circular_di::DEPS5
        );
        injector.get<int>(circular_di::CIRCULAR1_ID);
        assert(false);
    } catch(di::circular_dependency_exception_t&) {}

    // Part 3: invalid injector usage
    try {
        // Same instance is already registered
        injector.factory<service4_t>(
            service4_di::ID, service4_di::create, service4_di::DEPS
        );
        assert(false);
    } catch(...) {}

    try {
        // Unknown id
        injector.get<service3_t>("I don't exist");
        assert(false);
    } catch(...) {}

    try {
        // Access to not explicitly requested dependency
        injector.factory<std::string>(
            broken_factory_di::ID, broken_factory_di::create, broken_factory_di::DEPS
        );
        injector.get<std::string>(broken_factory_di::ID);
        assert(false);
    } catch(...) {}

    try {
        // Wrong type
        injector.get<int>(service3_di::ID);
        assert(false);
    } catch(...) {}
    return 0;
}
