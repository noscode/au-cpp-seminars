#include <iostream>
#include <cassert>
#include <vector>
#include <set>
#include <functional>
#include <unordered_set>
#include <list>
#include <string>
#include <iterator>
#include <cctype>

template<class fwd_it>
void print(fwd_it begin, fwd_it end)
{
    for (auto it = begin; it != end; ++it)
    {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
}

template<class fwd_it, class comparator = std::less<typename fwd_it::value_type>>
void tree_sort(fwd_it begin, fwd_it end)
{
    typedef typename fwd_it::value_type type;
    std::multiset<type, comparator> tree;

    for (auto it = begin; it != end; ++it)
        tree.insert(*it);

    auto it = begin;
    for (auto const& v : tree)
        *it++ = v;

    assert(it == end);
}

template<class T, class pred = std::equal_to<T>>
void remove_duplicates(std::list<T>& seq)
{
    std::unordered_set<T, std::hash<T>, pred> s;
    for (auto it = seq.begin(); it != seq.end();)
    {
        auto res = s.insert(*it);
        if (!res.second)
            it = seq.erase(it);
        else
            ++it;
    }
}

template<class cbd_it, class pred
    = std::equal_to<typename std::iterator_traits<cbd_it>::value_type>>
bool is_palindrome(cbd_it begin, cbd_it end)
{
    pred equals;
    auto rbegin = std::reverse_iterator<cbd_it>(end);
    for (auto it = begin, rit = rbegin; it != end; ++it, ++rit)
        if (!equals(*it, *rit))
            return false;
    return true;
}

template<class FWD_IT>
struct alpha_iterator:
    std::iterator<std::forward_iterator_tag,
                  typename std::iterator_traits<FWD_IT>::value_type>
{
    using base = std::iterator<std::forward_iterator_tag,
                  typename std::iterator_traits<FWD_IT>::value_type>;

    alpha_iterator(FWD_IT begin, FWD_IT end)
        : it_(begin)
        , endit_(end)
    {
        if (!isalpha(*it_))
            goToNextAlpha();
    }

    alpha_iterator(FWD_IT end)
        : it_(end)
        , endit_(end)
    {}

    alpha_iterator() = delete;
    alpha_iterator(const alpha_iterator &src) = default;
    alpha_iterator& operator=(const alpha_iterator &src) = default;

    alpha_iterator operator++()
    {
        goToNextAlpha();
        return it_;
    }

    alpha_iterator operator++(int)
    {
        auto it_copy = it_;
        goToNextAlpha();
        return it_copy;
    }

    bool operator==(const alpha_iterator &other)
    {
        return it_ == other.it_;
    }

    bool operator!=(const alpha_iterator &other)
    {
        return !this->operator==(other);
    }

    typename base::reference operator*()
    {
        return *it_;
    }

    typename base::pointer operator->()
    {
        return &(*it_);
    }

private:
    void goToNextAlpha()
    {
        while(it_ != endit_) {
            it_++;
            if (isalpha(*it_))
                break;
        }
    }
    FWD_IT it_;
    FWD_IT endit_;
};

template<class FWD_IT>
alpha_iterator<FWD_IT> make_alpha_iterator(FWD_IT begin, FWD_IT end)
{
    return alpha_iterator<FWD_IT>(begin, end);
}

template<class FWD_IT>
alpha_iterator<FWD_IT> make_alpha_iterator(FWD_IT end)
{
    return alpha_iterator<FWD_IT>(end);
}

int main()
{
    std::vector<int> vec = { 10, -1, 0, 23, 44, 11, 1, 0, -1000, 3333 };
    print(vec.begin(), vec.end());
    tree_sort(vec.begin(), vec.end());
    print(vec.begin(), vec.end());

    std::list<decltype(vec)::value_type> lst(vec.begin(), vec.end());
    print(lst.begin(), lst.end());
    remove_duplicates(lst);
    print(lst.begin(), lst.end());

    std::string polystr = "asdfghjhgfdsa";
    std::string nonpolystr = "asdfghj1hgfdsa";
    assert(is_palindrome(polystr.cbegin(), polystr.cend()));
    assert(!is_palindrome(nonpolystr.cbegin(), nonpolystr.cend()));

    std::string characters = "asd\1\1asd\2\2\2asd\3\3\3\3asd"
        "\4\4\4\4\4asd\5\5\5\5\5\5asd\6\6\6\6\6\6\6\6";
    print(characters.begin(), characters.end());
    auto anum_begin = make_alpha_iterator(
        characters.begin(), characters.end()
    );
    auto anum_end = make_alpha_iterator(characters.end());
    print(anum_begin, anum_end);
    swap(anum_begin, anum_end);
    // print(anum_begin, anum_end); :)

    return 0;
}
