#pragma once

#include <type_traits>
#include <cstdint>
#include <ostream>
#include <istream>

namespace serialization
{
    typedef uint64_t size_type;
    namespace type_traits
    {
        template <class T>
        struct is_container
        {
            template<class U>
            static std::true_type test(typename U::iterator *);
            template<class U>
            static std::false_type test(...);

            enum { value = decltype(test<T>(nullptr))::value };
            typedef std::integral_constant<bool, value> type;
        };

        template <class T>
        struct is_container_or_pod
        {
            enum { value = is_container<T>::value || std::is_pod<T>::value };
            typedef std::integral_constant<bool, value> type;
        };
    } // type_traits

    //////////////////////////////////////////////////////////////////////////
    // Forward declarations of all the non read-write specializations
    template<class stream, class type1, class type2>
    void serialize(stream &s, std::pair<type1, type2>& p);

    //////////////////////////////////////////////////////////////////////////
    // readers
    template<class type>
    void serialize(std::istream& is, type& obj,
            typename std::enable_if<std::is_pod<type>::value>::type* = nullptr)
    {
        is.read(reinterpret_cast<char*>(&obj), sizeof(obj));
    }

    template<class type>
    void serialize(std::istream& is, type& container,
            typename std::enable_if<type_traits::is_container<type>::value>::type* = nullptr)
    {
        size_type sz = 0;
        serialize(is, sz);

        container.clear();
        for (size_t i = 0; i < sz; ++i)
        {
            typename type::value_type obj;
            serialize(is, obj);
            container.insert(container.end(), std::move(obj));
        }
    }

    //////////////////////////////////////////////////////////////////////////
    // writers 
    template<class type>
    void serialize(std::ostream& os, type &obj,
            typename std::enable_if<std::is_pod<type>::value>::type* = nullptr)
    {
        os.write(reinterpret_cast<char*>(&obj), sizeof(obj));
    }

    template<class type>
    void serialize(std::ostream& os, type &container,
            typename std::enable_if<type_traits::is_container<type>::value>::type* = nullptr)
    {
        size_type size = container.size();
        serialize(os, size);

        for (auto it = container.begin(); it != container.end(); ++it)
            serialize(os, *it);
    }

    //////////////////////////////////////////////////////////////////////////
    // definitions of all the non read-write specializations
    template<class stream, class type1, class type2>
    void serialize(stream &s, std::pair<type1, type2>& p)
    {
        // to support non-pod map and unordered_map
        typedef typename std::remove_const<type1>::type first_type;
        typedef typename std::remove_const<type2>::type second_type;

        serialize(s, const_cast<first_type&>(p.first )); // to solve const map key problem
        serialize(s, const_cast<second_type&>(p.second)); // just to be simmetric 
    }

    // helper for user's code readability
    template<class type>
    void deserialize(std::istream& is, type& obj)
    {
        serialize(is, obj);
    }

    // We can place here more optimized serialize implementations.
    // Example: one-shot copy for continuous containers (vector, string)
} // namespace serialization

