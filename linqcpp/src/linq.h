#pragma once

#include <ostream>
#include <vector>
#include <functional>
#include <algorithm>
#include <initializer_list>

template<class ELEM_TYPE>
struct enumerable
{
    enumerable(const std::vector<ELEM_TYPE> &collection={});
    enumerable(const enumerable &src) = default;
    enumerable& operator=(const enumerable &src) = default;

    enumerable& print(std::ostream &out);
    enumerable where(std::function<bool(const ELEM_TYPE&)> pred) const;
    /*
    template<class DST_TYPE>
    enumerable<DST_TYPE> select(std::function<DST_TYPE(const ELEM_TYPE&)> transform) const;
    */
    template<class TRANS_FUNC>
    using transform_ret_type = enumerable<decltype((*static_cast<TRANS_FUNC*>(nullptr))(*static_cast<ELEM_TYPE*>(nullptr)))>;
    template<class TRANS_FUNC>
    auto select(TRANS_FUNC transform_func) const -> transform_ret_type<TRANS_FUNC>;

    size_t count() const;
    size_t count(std::function<bool(const ELEM_TYPE&)> pred) const;
    bool any() const;
    bool any(std::function<bool(const ELEM_TYPE&)> pred) const;
    std::vector<ELEM_TYPE> toVector() const;
private:
    std::vector<ELEM_TYPE> collection_;
};

template<class ELEM_TYPE>
enumerable<ELEM_TYPE> from(const std::vector<ELEM_TYPE> &collection)
{
    return enumerable<ELEM_TYPE>(collection);
}

template<class ELEM_TYPE>
enumerable<ELEM_TYPE> from(std::initializer_list<ELEM_TYPE> init_list)
{
    return from(std::vector<ELEM_TYPE>(init_list));
}

template<class ELEM_TYPE>
enumerable<ELEM_TYPE>::enumerable(const std::vector<ELEM_TYPE> &collection)
    : collection_(collection)
{}

template<class ELEM_TYPE>
auto enumerable<ELEM_TYPE>::print(std::ostream &out) -> enumerable&
{
    for (const ELEM_TYPE& elem : collection_) {
        out << elem << " ";
    }
    out << std::endl;
    return *this;
}

template<class ELEM_TYPE>
auto enumerable<ELEM_TYPE>::where(std::function<bool(const ELEM_TYPE&)> pred) const -> enumerable
{
    enumerable result(*this);
    auto first_to_remove = std::remove_if(
            result.collection_.begin(),
            result.collection_.end(),
            std::not1(pred)
    );
    result.collection_.erase(
            first_to_remove,
            result.collection_.end()
    );
    return result;
}

template<class ELEM_TYPE>
std::vector<ELEM_TYPE> enumerable<ELEM_TYPE>::toVector() const
{
    return collection_;
}

/*
template<class SRC_TYPE>
template<class DST_TYPE>
enumerable<DST_TYPE> enumerable<SRC_TYPE>::select(
        std::function<DST_TYPE(const SRC_TYPE&)> transform_func) const
{
    std::vector<DST_TYPE> result_collection;
    result_collection.reserve(result_collection.size());
    std::transform(
            collection_.begin(),
            collection_.end(),
            std::back_inserter(result_collection),
            transform_func
    );
    return enumerable<DST_TYPE>(result_collection);
}
*/

template<class ELEM_TYPE>
template<class TRANS_FUNC>
auto enumerable<ELEM_TYPE>::select(TRANS_FUNC transform_func) const
    -> transform_ret_type<TRANS_FUNC>
{
    using DST_TYPE = decltype(transform_func(*static_cast<ELEM_TYPE*>(nullptr)));
    std::vector<DST_TYPE> result_collection;
    result_collection.reserve(result_collection.size());
    std::transform(
            collection_.begin(),
            collection_.end(),
            std::back_inserter(result_collection),
            transform_func
    );
    return enumerable<DST_TYPE>(result_collection);
}

template<class ELEM_TYPE>
size_t enumerable<ELEM_TYPE>::count() const
{
    return collection_.size();
}

template<class ELEM_TYPE>
size_t enumerable<ELEM_TYPE>::count(std::function<bool(const ELEM_TYPE&)> pred) const
{
    return std::count_if(collection_.begin(), collection_.end(), pred);
}

template<class ELEM_TYPE>
bool enumerable<ELEM_TYPE>::any() const
{
    return !collection_.empty();
}

template<class ELEM_TYPE>
bool enumerable<ELEM_TYPE>::any(std::function<bool(const ELEM_TYPE&)> pred) const
{
    return std::any_of(collection_.begin(), collection_.end(), pred);
}
