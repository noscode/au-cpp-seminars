#pragma once
#include <functional>
#include <utility>
#include <map>
#include <typeinfo>
#include <typeindex>

template<typename BASE_TYPE, typename TAG=void>
struct double_dispatcher
{
    template<typename ARG1, typename ARG2>
    using func_type = std::function<void(ARG1&, ARG2&)>;

    template<typename ARG1, typename ARG2>
    static void reg(func_type<ARG1, ARG2> func)
    {
        type_ix_pair_type tp(typeid(ARG1), typeid(ARG2));
        dd_map[tp] = [func](BASE_TYPE &arg1, BASE_TYPE &arg2) {
            func(dynamic_cast<ARG1&>(arg1), dynamic_cast<ARG2&>(arg2));
        };

        type_ix_pair_type tp_rev(typeid(ARG2), typeid(ARG1));
        dd_map[tp_rev] = [func](BASE_TYPE &arg2, BASE_TYPE &arg1) {
            func(dynamic_cast<ARG1&>(arg1), dynamic_cast<ARG2&>(arg2));
        };
    }

    template<typename ARG1, typename ARG2>
    static void reg(void (*func)(ARG1&, ARG2&))
    {
        reg<ARG1, ARG2>(func_type<ARG1, ARG2>(func));
    }

    static void call(BASE_TYPE &arg1, BASE_TYPE &arg2)
    {
        type_ix_pair_type tp(typeid(arg1), typeid(arg2));
        typename dd_map_type::iterator func_it = dd_map.find(tp);
        if (func_it == dd_map.end())
        {
            throw std::bad_cast();
        }
        func_it->second(arg1, arg2);
    }

private:
    typedef std::pair<std::type_index, std::type_index> type_ix_pair_type;
    typedef std::function<void(BASE_TYPE&, BASE_TYPE&)> stored_func_type;
    typedef std::map<type_ix_pair_type, stored_func_type> dd_map_type;
    static dd_map_type dd_map;
};

template<typename BASE_TYPE, typename TAG>
typename double_dispatcher<BASE_TYPE, TAG>::dd_map_type
double_dispatcher<BASE_TYPE, TAG>::dd_map;
