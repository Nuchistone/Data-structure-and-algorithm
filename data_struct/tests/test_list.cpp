#include "List.h"
#include "test_utils.h"
#include <string>
#include <utility>

using data_struct::List;

// util function, turn List into string
template <typename T>
std::string to_string(const List<T>& list)
{
    std::string result;
    for (const auto& item : list)
    {
        result += std::to_string(item) + " ";
    }
    return result;
}

// List invariant check
template <typename T>
void check_list_invariants_impl(const List<T>& list)
{
    if (list.empty())
    {
        CHECK_EQUAL(list.size(), 0u);
        CHECK(list.begin() == list.end());
    }
    else
    {
        CHECK(list.size() > 0u);
        CHECK(list.begin() != list.end());
    }
}

#define CHECK_LIST_INVARIANTS(list) \
    check_list_invariants_impl((list))

// construct
TEST_CASE(test_list_default_construct)
{
    List<int> a;
    CHECK(a.empty());
    CHECK_EQUAL(a.size(), 0u);
    CHECK(a.begin() == a.end());
    CHECK_LIST_INVARIANTS(a);
}

TEST_CASE(test_list_init_list_construct)
{
    List<int> a{1, 2, 3, 4, 5};
    CHECK_EQUAL(a.size(), 5u);
    CHECK_EQUAL(a.front(), 1);
    CHECK_EQUAL(a.back(), 5);
    CHECK_EQUAL(to_string(a), std::string("1 2 3 4 5 "));
    CHECK_LIST_INVARIANTS(a);
}

TEST_CASE(test_list_fill_construct)
{
    List<int> a(4, 9);
    CHECK_EQUAL(a.size(), 4u);
    CHECK_EQUAL(to_string(a), std::string("9 9 9 9 "));
    CHECK_LIST_INVARIANTS(a);
}

TEST_CASE(test_list_copy_construct)
{
    List<int> a{1, 2, 3};
    List<int> b(a);

    CHECK_EQUAL(b.size(), 3u);
    CHECK_EQUAL(to_string(b), std::string("1 2 3 "));

    // deep copy
    b.front() = 100;
    CHECK_EQUAL(a.front(), 1);
    CHECK_EQUAL(b.front(), 100);
}

TEST_CASE(test_list_move_construct)
{
    List<int> a{1, 2, 3};
    List<int> b(std::move(a));

    CHECK_EQUAL(b.size(), 3u);
    CHECK_EQUAL(to_string(b), std::string("1 2 3 "));

    // a is empty after move
    CHECK(a.empty());
    CHECK_EQUAL(a.size(), 0u);
    CHECK(a.begin() == a.end());
}

// assign
TEST_CASE(test_list_copy_assign)
{
    List<int> a{1, 2, 3};
    List<int> b{4, 5};
    b = a;

    CHECK_EQUAL(b.size(), 3u);
    CHECK_EQUAL(to_string(b), std::string("1 2 3 "));

    // self assigning
    b = b;
    CHECK_EQUAL(to_string(b), std::string("1 2 3 "));
}

TEST_CASE(test_list_move_assign)
{
    List<int> a{1, 2, 3};
    List<int> b{4, 5};
    b = std::move(a);

    CHECK_EQUAL(b.size(), 3u);
    CHECK_EQUAL(to_string(b), std::string("1 2 3 "));
    CHECK(a.empty());
}

TEST_CASE(test_list_swap)
{
    List<int> a{1, 2, 3};
    List<int> b{4, 5};

    a.swap(b);
    CHECK_EQUAL(to_string(a), std::string("4 5 "));
    CHECK_EQUAL(to_string(b), std::string("1 2 3 "));

    swap(a, b);
    CHECK_EQUAL(to_string(a), std::string("1 2 3 "));
    CHECK_EQUAL(to_string(b), std::string("4 5 "));
}

// iterator
TEST_CASE(test_list_iterator)
{
    List<int> a{1, 2, 3};

    // traverse forward
    int sum = 0;
    for (int x : a)
    {
        sum += x;
    }
    CHECK_EQUAL(sum, 6);

    // modifying element
    for (auto it = a.begin(); it != a.end(); ++it)
    {
        *it *= 2;
    }
    CHECK_EQUAL(to_string(a), std::string("2 4 6 "));

    // traverse backward
    int reverse_sum = 0;
    for (auto it = a.end(); it != a.begin(); )
    {
        --it;
        reverse_sum = reverse_sum * 10 + *it;
    }
    CHECK_EQUAL(reverse_sum, 642); // 6, 4, 2

    // const iterator implicit convert
    const List<int>& ca = a;
    auto cit = ca.begin();
    CHECK_EQUAL(*cit, 2);
}

// mutator
TEST_CASE(test_list_push_pop)
{
    List<int> a;
    a.push_back(1);  // 1
    a.push_back(2);  // 1 2
    a.push_front(0); // 0 1 2
    a.push_front(-1);// -1 0 1 2

    CHECK_EQUAL(to_string(a), std::string("-1 0 1 2 "));

    a.pop_back();    // -1 0 1
    a.pop_front();   // 0 1
    CHECK_EQUAL(to_string(a), std::string("0 1 "));
}

TEST_CASE(test_list_insert_erase)
{
    List<int> a{1, 2, 3, 4};

    // insert
    auto it = a.begin();
    ++it; // *it = 2
    it = a.insert(it, 99);   // a = 1 99 2 3 4
    CHECK_EQUAL(*it, 99);
    CHECK_EQUAL(to_string(a), std::string("1 99 2 3 4 "));

    // delete
    it = a.erase(it); // a = 1 2 3 4
    CHECK_EQUAL(*it, 2);
    CHECK_EQUAL(to_string(a), std::string("1 2 3 4 "));

    // range delete
    auto first = a.begin();
    ++first; // 2
    auto last = a.end();
    --last; // 4
    a.erase(first, last);
    CHECK_EQUAL(to_string(a), std::string("1 4 "));
}


// Splice
TEST_CASE(test_list_splice_all)
{
    List<int> a{1, 2, 3};
    List<int> b{4, 5};

    a.splice(a.end(), b);
    CHECK_EQUAL(to_string(a), std::string("1 2 3 4 5 "));
    CHECK(b.empty());

    // test if b is still valid
    b.push_back(99);
    CHECK_EQUAL(to_string(b), std::string("99 "));
}

TEST_CASE(test_list_splice_single)
{
    List<int> a{1, 2, 3};
    List<int> b{4, 5, 6};

    // 1. b: 5 -> a, before 2
    auto pos = a.begin();
    ++pos; // 2
    auto it = b.begin();
    ++it;   // 5
    a.splice(pos, b, it);
    CHECK_EQUAL(to_string(a), std::string("1 5 2 3 "));
    CHECK_EQUAL(to_string(b), std::string("4 6 "));

    // 2. same List
    pos = a.begin(); // 1
    it = a.end();
    --it; // 3
    a.splice(pos, a, it);
    CHECK_EQUAL(to_string(a), std::string("3 1 5 2 "));
}

TEST_CASE(test_list_splice_range)
{
    List<int> a{1, 2, 3};
    List<int> b{4, 5, 6, 7};

    // 1. b: [5, 7) -> a, before 3
    auto first = b.begin();
    ++first;    // 5
    auto last = b.end();
    --last;     // 7
    auto pos = a.end();
    --pos;      // 3

    a.splice(pos, b, first, last);  // a = {1 2 5 6 3}, b = {4 7}
    CHECK_EQUAL(to_string(a), std::string("1 2 5 6 3 "));
    CHECK_EQUAL(to_string(b), std::string("4 7 "));

    // 2. transfer the tail of b: [7, b.end()), end boundary test
    first = b.begin();
    ++first; // 7
    a.splice(a.begin(), b, first, b.end()); // a = {7 1 2 5 6 3}, b = {4}
    CHECK_EQUAL(to_string(a), std::string("7 1 2 5 6 3 "));
    CHECK_EQUAL(to_string(b), std::string("4 "));
    
    // 3. same List
    first = a.begin();          // 7
    auto last2 = a.begin(); 
    ++last2;
    ++last2;
    ++last2;  // 5
    
    a.splice(a.end(), a, first, last2); // a = {5 6 7 1 2 3}
    CHECK_EQUAL(to_string(a), std::string("5 6 3 7 1 2 "));
}

// algorithm
TEST_CASE(test_list_find_remove)
{
    List<int> a{1, 2, 3, 2, 4};

    auto it = a.find(3);
    CHECK(it != a.end());
    CHECK_EQUAL(*it, 3);

    auto it_not_found = a.find(99);
    CHECK(it_not_found == a.end());

    a.remove(2);
    CHECK_EQUAL(to_string(a), std::string("1 3 4 "));
}


// lifetime
TEST_CASE(test_list_lifetime)
{
    Tracker::reset();
    {
        List<Tracker> a;
        a.push_back(Tracker(1));
        a.push_back(Tracker(2));
        a.push_front(Tracker(0));

        CHECK_EQUAL(a.size(), 3u);
    }
    Tracker::check_no_leak();
}

// exception
TEST_CASE(test_list_exception_safety)
{
    CopyThrower::reset();
    {
        List<CopyThrower> a;
        a.push_back(CopyThrower(1));
        a.push_back(CopyThrower(2));

        CopyThrower::reset();
        CopyThrower::throw_after_copy_ = 0;

        CopyThrower t(3);
        CHECK_THROWS(a.push_back(t), std::runtime_error);

        CHECK_EQUAL(a.size(), 2u);
    }
}
 
// int main()
// {
//     return run_all_tests();
// }