#pragma once
#include <vector>
#include <initializer_list>
#include <stdexcept>
#include <functional>
#include <typeinfo>
#include <typeindex>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <memory>

namespace di {

    namespace details {
        struct rttied_base_t
        {
            virtual ~rttied_base_t() {}
        };

        template<class T>
        struct rttied_t : rttied_base_t
        {
            rttied_t(std::unique_ptr<T> &&ptr)
                : ptr_(std::move(ptr))
            {}

            rttied_t(rttied_t &&src) = default;

            T* value() { return ptr_.get(); }
        private:
            std::unique_ptr<T> ptr_;
        };

        template<class T>
        using rttied_type = rttied_t<typename std::remove_reference<T>::type>;

        template<class T>
        std::unique_ptr<rttied_type<T>> make_rttied(std::unique_ptr<T> ptr)
        {
            return std::unique_ptr<rttied_type<T>>(
                new rttied_type<T>(std::move(ptr))
            );
        }
    }

struct dep_t
{
    dep_t(const std::string &id, const std::type_index type)
        : id_(id)
        , type_(type)
    {}

    const std::string& id() const
    {
        return id_;
    }

    const std::type_index& type() const
    {
        return type_;
    }
private:
    std::string id_;
    std::type_index type_;
};

template<class T>
dep_t make_dep(const std::string &id)
{
    return dep_t(id, typeid(details::rttied_type<T>));
};

struct dep_decls_t
{
    dep_decls_t(std::initializer_list<dep_t> deps_list)
        : deps_(deps_list)
    {}

    const std::vector<dep_t>& deps() const
    {
        return deps_;
    }
private:
    std::vector<dep_t> deps_;
};

struct injector_t;
struct injector_exception_t: std::exception
{};

struct circular_dependency_exception_t: injector_exception_t
{
    virtual const char* what() const noexcept {
        return "Circular dependency was observed";
    }
};

struct no_such_dependency_exception_t: injector_exception_t
{
    virtual const char* what() const noexcept {
        return "No such dependency";
    }
};

struct invalid_dependency_type_exception_t: injector_exception_t
{
    virtual const char* what() const noexcept {
        return "Dependency has another type";
    }
};

struct already_defined_instance_exception_t: injector_exception_t
{
    virtual const char* what() const noexcept {
        return "Instance already defined";
    }
};

namespace details {
    struct dep_inst_t
    {
        std::string id_;
        details::rttied_base_t *rttied_value_;
    };
}

struct dep_insts_t
{
    template<class T>
    T& get(const std::string dep_id)
    {
        auto dep_it = insts_.find(dep_id);
        if (dep_it == insts_.end())
            throw no_such_dependency_exception_t();

        details::rttied_type<T> *rttied_value =
            dynamic_cast<details::rttied_type<T>*>(
                dep_it->second.rttied_value_
            );
        if (!rttied_value)
            throw invalid_dependency_type_exception_t();
        return *rttied_value->value();
    }

private:
    friend class injector_t;
    dep_insts_t(std::unordered_map<std::string, details::dep_inst_t> &&insts)
        : insts_(std::move(insts))
    {}
    std::unordered_map<std::string, details::dep_inst_t> insts_;
};

struct injector_t
{
    template<class T>
    using factory_func_t = std::function<T*(dep_insts_t&)>;

    template<class T>
    void factory(const std::string &id,
        factory_func_t<T> factory,
        const dep_decls_t& deps)
    {
        if (rttied_insts_.find(id) != rttied_insts_.end())
            throw already_defined_instance_exception_t();
        if (factories_.find(id) != factories_.end())
            throw already_defined_instance_exception_t();

        factory_and_dep_decls_t::factory_func_t base_factory =
            [factory](dep_insts_t &deps)
            { return new details::rttied_type<T>(std::unique_ptr<T>(factory(deps))); };
        factory_and_dep_decls_t *factory_and_dep_decls =
            new factory_and_dep_decls_t(base_factory, deps);
 
        factories_[id] =
            std::unique_ptr<factory_and_dep_decls_t>(factory_and_dep_decls);
    }

    template<class T>
    void value(const std::string &id, T *instance)
    {
        if (rttied_insts_.find(id) != rttied_insts_.end())
            throw already_defined_instance_exception_t();
        if (factories_.find(id) != factories_.end())
            throw already_defined_instance_exception_t();
        rttied_insts_[id] = details::make_rttied(std::unique_ptr<T>(instance));
    }

    template<class T>
    T& get(const std::string &id)
    {
        std::unordered_set<std::string> visited_set;
        details::rttied_base_t* base_ptr =
            get_recursive(id, typeid(details::rttied_type<T>), visited_set);
        details::rttied_type<T> *ptr =
            dynamic_cast<details::rttied_type<T>*>(base_ptr);
        if (!ptr)
            throw invalid_dependency_type_exception_t();
        return *ptr->value();
    }

    static injector_t& instance()
    {
        static injector_t instance;
        return instance;
    }
private:
    struct factory_and_dep_decls_t {
        typedef std::function<details::rttied_base_t*(dep_insts_t&)>
            factory_func_t;
        factory_and_dep_decls_t(
            factory_func_t factory, dep_decls_t dep_decls)
            : factory_(factory)
            , dep_decls_(dep_decls)
        {}

        factory_func_t factory_;
        dep_decls_t dep_decls_;
    };
    std::unordered_map<std::string,
        std::unique_ptr<factory_and_dep_decls_t>> factories_;
    std::unordered_map<std::string,
        std::unique_ptr<details::rttied_base_t>> rttied_insts_;

    details::rttied_base_t* get_recursive(const std::string &id,
            std::type_index type,
            std::unordered_set<std::string> &visited_dep_ids)
    {
        if (visited_dep_ids.find(id) != visited_dep_ids.end())
            throw circular_dependency_exception_t();
        visited_dep_ids.insert(id);

        if (rttied_insts_.find(id) != rttied_insts_.end())
        {
            details::rttied_base_t* inst =
                rttied_insts_.find(id)->second.get();
            if (std::type_index(typeid(*inst)) != type)
                throw invalid_dependency_type_exception_t();
            visited_dep_ids.erase(id);
            return inst;
        } else if(factories_.find(id) == factories_.end())
            throw no_such_dependency_exception_t();

        factory_and_dep_decls_t *factory_and_dep_decls =
            factories_.find(id)->second.get();

        std::unordered_map<std::string, details::dep_inst_t> factory_dep_insts_map;
        for(auto &dep_decl : factory_and_dep_decls->dep_decls_.deps())
        {
            details::dep_inst_t dep_inst;
            dep_inst.id_ = dep_decl.id();
            dep_inst.rttied_value_ = get_recursive(
                dep_decl.id(), dep_decl.type(), visited_dep_ids);
            factory_dep_insts_map[dep_decl.id()] = dep_inst;
        }
        dep_insts_t factory_dep_insts(std::move(factory_dep_insts_map));
        std::unique_ptr<details::rttied_base_t> inst =
            std::unique_ptr<details::rttied_base_t>(
                factory_and_dep_decls->factory_(factory_dep_insts));

        if (std::type_index(typeid(*inst.get())) != type)
                throw invalid_dependency_type_exception_t();

        rttied_insts_[id] = std::move(inst);
        visited_dep_ids.erase(id);
        return rttied_insts_[id].get();
    }
};

} // namespace di
