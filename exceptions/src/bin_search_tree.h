#pragma once
#include <memory>
#include <stdexcept>
#include <cassert>
#include <string>
#include <sstream>
#include <iterator>
#include <vector>
#include <utility>

namespace details
{

template<class KEY, class VALUE>
struct node
{
    node(const KEY &key, const VALUE &val)
        : key_(key)
        , val_(val)
        , parent_(nullptr)
    {}

    ~node() noexcept
    {}

    node(node &&src) = default;
    node& operator=(node &&src) = default;

    node(const node &src) = delete;
    node& operator=(const node &src) = delete;

    void putLeft(std::unique_ptr<node> &&nptr) noexcept
    {
        left_ptr_ = std::move(nptr);
    }

    std::unique_ptr<node> takeLeft() noexcept
    {
        return std::move(left_ptr_);
    }

    void putRight(std::unique_ptr<node> &&nptr) noexcept
    {
        right_ptr_ = std::move(nptr);
    }

    std::unique_ptr<node> takeRight() noexcept
    {
        return std::move(right_ptr_);
    }

    node*& parent() noexcept
    {
        return parent_;
    }

    node* left() noexcept
    {
        return left_ptr_.get();
    }

    node* right() noexcept
    {
        return right_ptr_.get();
    }

    KEY& key() noexcept
    {
        return key_;
    }

    VALUE &value() noexcept
    {
        return val_;
    }
private:
    KEY key_;
    VALUE val_;
    node *parent_;
    std::unique_ptr<node> left_ptr_;
    std::unique_ptr<node> right_ptr_;
};

} // namespace details

struct bst_key_exists_exception: std::logic_error
{
    bst_key_exists_exception()
        : std::logic_error("Key already exists")
    {}
};

template<class KEY>
struct bst_verbose_key_exists_exception: bst_key_exists_exception
{
    bst_verbose_key_exists_exception(const KEY &key)
        : key_(key)
    {
        std::stringstream ss;
        ss << "key " << key << " already exists" << std::endl;
        what_ = ss.str();
    }

    virtual const char* what() const noexcept override 
    {
        return what_.c_str();
    }

    const KEY& key() const noexcept
    {
        return key;
    }
private:
    KEY key_;
    std::string what_;
};

struct bst_key_absent_exception: std::logic_error
{
    bst_key_absent_exception()
        : std::logic_error("Binary search tree key is absent")
    {}
};

// KEY should have operator <, <<, () constructor
template<class KEY, class VALUE>
struct bin_search_tree
{
    bin_search_tree() = default;

    // throws bst_key_exists_exception
    void insert(const KEY &key, const VALUE &val);

    // throws bst_key_exists_exception
    template<class KIT, class VIT>
    void insert(KIT kbegin, KIT kend, VIT vbegin);

    // throws bst_key_absent_exception
    VALUE& find(const KEY &key);
    bool contains(const KEY &key);
private:
    typedef details::node<KEY, VALUE> node;
    node* find_nearest_node(const KEY &key);
    // throws bst_key_exists_exception
    void insert_node(std::unique_ptr<node> &&node_ptr);
    void reinsert_node(std::unique_ptr<node> &&node_ptr) noexcept;
    void erase_node(node *node) noexcept;
    static bool should_go_right(node *parent, const KEY &key);

    std::unique_ptr<node> root_ptr_;
};

template<class KEY, class VALUE>
auto bin_search_tree<KEY, VALUE>::find_nearest_node(const KEY &key) -> node*
{
    if (!root_ptr_)
        return nullptr;

    node *cur_node = root_ptr_.get();
    for(;;)
    {
        if (cur_node->key() == key)
            break;

        node *next_child = should_go_right(cur_node, key)
            ? cur_node->right()
            : cur_node->left();

        if (!next_child)
            break;
        cur_node = next_child;
    }
    assert(cur_node);
    return cur_node;
}

template<class KEY, class VALUE>
bool bin_search_tree<KEY, VALUE>::should_go_right(node *parent, const KEY &key) {
    return key > parent->key();
};

template<class KEY, class VALUE>
void bin_search_tree<KEY, VALUE>::insert(const KEY &key, const VALUE &value)
{
    std::unique_ptr<node> new_node_ptr(new node(key, value));
    insert_node(std::move(new_node_ptr));
}

template<class KEY, class VALUE>
void bin_search_tree<KEY, VALUE>::insert_node(std::unique_ptr<node> &&new_node_ptr)
{
    node *nearest_node = find_nearest_node(new_node_ptr->key());
    if (!nearest_node)
    {
        root_ptr_ = std::move(new_node_ptr);
        return;
    }

    if (nearest_node->key() == new_node_ptr->key())
        throw bst_verbose_key_exists_exception<KEY>(new_node_ptr->key());

    new_node_ptr->parent() = nearest_node;
    if (should_go_right(nearest_node, new_node_ptr->key()))
        nearest_node->putRight(std::move(new_node_ptr));
    else
        nearest_node->putLeft(std::move(new_node_ptr));
}

template<class KEY, class VALUE>
void bin_search_tree<KEY, VALUE>::reinsert_node(std::unique_ptr<node> &&new_node_ptr) noexcept
{
    try
    {
        insert_node(std::move(new_node_ptr));
    }
    catch(const bst_key_exists_exception &ex)
    {
        assert(false);
    }
}

template<class KEY, class VALUE>
template<class KIT, class VIT>
void bin_search_tree<KEY, VALUE>::insert(KIT kbegin, KIT kend, VIT vbegin)
{
    const size_t nodes_cnt = std::distance(kbegin, kend);
    std::vector<std::unique_ptr<node>> node_ptrs;
    node_ptrs.reserve(nodes_cnt);

    auto kit = kbegin;
    auto vit = vbegin;
    for(size_t node_ix = 0; node_ix < nodes_cnt; ++node_ix, ++kit, ++vbegin)
    {
        std::unique_ptr<node> node_ptr(new node(*kit, *vit));
        node_ptrs.emplace_back(std::move(node_ptr));
    }
    // at this point all the nodes are allocated
    // and will be deleted if exception is thrown in code below
    for(auto nptr_it = node_ptrs.begin(); nptr_it != node_ptrs.end(); ++nptr_it)
    {
        try
        {
            insert_node(std::move(*nptr_it));
        }
        catch(...)
        {
            kit = kbegin;
            for(auto nptr_del_it = node_ptrs.begin(); nptr_del_it != nptr_it; ++nptr_del_it, ++kit)
            {
                erase_node(find_nearest_node(*kit));
            }
            throw;
        }
    }
}

template<class KEY, class VALUE>
void bin_search_tree<KEY, VALUE>::erase_node(node *enode) noexcept
{
    node *parent = enode->parent();
    std::unique_ptr<node> left_ptr = enode->takeLeft();
    std::unique_ptr<node> right_ptr = enode->takeRight();

    if (parent)
    {
        if (should_go_right(parent, enode->key()))
        {
            parent->putRight(nullptr);
        }
        else
        {
            parent->putLeft(nullptr);
        }
    }
    else
    {
        root_ptr_ = nullptr;
    }
    reinsert_node(std::move(left_ptr));
    reinsert_node(std::move(right_ptr));
}

template<class KEY, class VALUE>
VALUE& bin_search_tree<KEY, VALUE>::find(const KEY &key)
{
    node *nearest_node = find_nearest_node(key);
    if (!nearest_node)
        throw bst_key_absent_exception();
    if (nearest_node->key() != key)
        throw bst_key_absent_exception();
    return nearest_node->value();
}

template<class KEY, class VALUE>
bool bin_search_tree<KEY, VALUE>::contains(const KEY &key)
{
    try
    {
        find(key);
        return true;
    }
    catch(const bst_key_absent_exception &ex)
    {
        return false;
    }
}
