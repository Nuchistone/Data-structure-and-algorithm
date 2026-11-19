#include "Array.h"
#include "test_utils.h"

using data_struct::Array;

// construct
TEST_CASE(test_default_construct)
{
    Array<int> a;

    CHECK_EQUAL(a.length(), 0u);
    CHECK_EQUAL(a.capacity(), 0u);
    CHECK(a.empty());
    CHECK_EQUAL(a.begin(), a.end());
    
    CHECK_CONTAINER_INVARIANTS(a);
}

TEST_CASE(test_length_construct)
{
    Array<int> a(3);

    CHECK_EQUAL(a.length(), 3u);
    CHECK_EQUAL(a.capacity(), 3u);
    CHECK(!a.empty());
    for (std::size_t i = 0; i < 3; ++i)
    {
        CHECK_EQUAL(a[i], 0);
    }

    CHECK_CONTAINER_INVARIANTS(a);
}

TEST_CASE(test_list_construct)
{
    Array<int> a{0, 1, 2};

    CHECK_EQUAL(a.length(), 3u);
    CHECK_EQUAL(a.capacity(), 3u);
    CHECK(!a.empty());
    for (std::size_t i = 0; i < 3; ++i)
    {
        CHECK_EQUAL(a[i], static_cast<int>(i));
    }

    CHECK_CONTAINER_INVARIANTS(a);
}

TEST_CASE(test_fill_construct)
{
    Array<int> a(3, 5);

    CHECK_EQUAL(a.length(), 3u);
    CHECK_EQUAL(a.capacity(), 3u);
    CHECK(!a.empty());
    for (std::size_t i = 0; i < 3; ++i)
    {
        CHECK_EQUAL(a[i], 5);
    }

    CHECK_CONTAINER_INVARIANTS(a);
}

TEST_CASE(test_copy_construct)
{
    Array<int> a{0, 1, 2};
    Array<int> b(a);

    CHECK_EQUAL(b.length(), 3u);
    CHECK_EQUAL(b.capacity(), 3u);
    CHECK(!b.empty());
    for (std::size_t i = 0; i < 3; ++i)
    {
        CHECK_EQUAL(b[i], static_cast<int>(i));
    }

    // a is not supposed to be assigned
    b[0] = 10;
    CHECK_EQUAL(a[0], 0);
    CHECK_EQUAL(b[0], 10);

    CHECK_CONTAINER_INVARIANTS(b);
}

TEST_CASE(test_move_construct)
{
    Array<int> a{0, 1, 2};
    Array<int> b(std::move(a));

    CHECK_EQUAL(b.length(), 3u);
    CHECK_EQUAL(b.capacity(), 3u);
    CHECK(!b.empty());
    for (std::size_t i = 0; i < 3; ++i)
    {
        CHECK_EQUAL(b[i], static_cast<int>(i));
    }

    // a is now empty
    CHECK_EQUAL(a.length(), 0u);
    CHECK_EQUAL(a.capacity(), 0u);
    CHECK(a.data() == nullptr);

    CHECK_CONTAINER_INVARIANTS(a);
    CHECK_CONTAINER_INVARIANTS(b);
}

// assign
TEST_CASE(test_copy_assign)
{
    Array<int> a{0, 1, 2};
    Array<int> b;
    b = a;

    CHECK_EQUAL(b.length(), 3u);
    CHECK_EQUAL(b.capacity(), 3u);
    for (std::size_t i = 0; i < 3; ++i)
    {
        CHECK_EQUAL(b[i], static_cast<int>(i));
    }

    // self assigning
    b = b;
    CHECK_EQUAL(b.length(), 3u);
    CHECK_EQUAL(b[2], 2);

    // a is not supposed to be assigned
    b[0] = 10;
    CHECK_EQUAL(a[0], 0);
    CHECK_EQUAL(b[0], 10);
}

TEST_CASE(test_move_assign)
{
    Array<int> a{0, 1, 2};
    Array<int> b;
    b = std::move(a);

    CHECK_EQUAL(b.length(), 3u);
    CHECK_EQUAL(b.capacity(), 3u);
    for (std::size_t i = 0; i < 3; ++i)
    {
        CHECK_EQUAL(b[i], static_cast<int>(i));
    }

    // a is now empty
    CHECK_EQUAL(a.length(), 0u);
}

TEST_CASE(test_swap)
{
    Array<int> a{0, 1, 2};
    Array<int> b{3, 4};

    // member
    a.swap(b);
    CHECK_EQUAL(a.length(), 2u);
    CHECK_EQUAL(a[0], 3);
    CHECK_EQUAL(b.length(), 3u);
    CHECK_EQUAL(b[2], 2);

    // non-member
    swap(a, b);
    CHECK_EQUAL(a.length(), 3u);
    CHECK_EQUAL(a[2], 2);
    CHECK_EQUAL(b.length(), 2u);
    CHECK_EQUAL(b[0], 3);
}

//getter
TEST_CASE(test_access)
{
    Array<int> a{0, 1, 2};

    CHECK_EQUAL(a[1], 1);
    CHECK_EQUAL(a.at(1), 1);
    CHECK_EQUAL(a.front(), 0);
    CHECK_EQUAL(a.back(), 2);

    a[0] = 10;
    a.at(1) = 20;       // a = {10, 20, 2}
    CHECK_EQUAL(a[0], 10);
    CHECK_EQUAL(a[1], 20);

    a.front() = 100;
    a.back() = 200;     //a = {100, 20, 200}
    CHECK_EQUAL(a[0], 100);
    CHECK_EQUAL(a[2], 200);

    Array<int>& ref = a;
    CHECK_EQUAL(ref[2], 200);
    ref[1] = 5;         // a = {100, 5, 200}
    CHECK_EQUAL(ref.at(1), 5);
    CHECK_EQUAL(a[1], 5);

    const Array<int>& cref = a;
    CHECK_EQUAL(cref[0], 100);
    CHECK_EQUAL(cref.at(1), 5);

    CHECK_THROWS(a.at(100), std::out_of_range);
    CHECK_THROWS(ref.at(100), std::out_of_range);
}

// iterator
TEST_CASE(test_iterator)
{
    Array<int> a{0, 1, 2, 3, 4};

    int i = 0;
    for (auto x : a)
    {
        CHECK_EQUAL(x, i++);
    }

    const Array<int>& ref = a;
    int sum = 0;
    for (auto iter = ref.cbegin(); iter != ref.cend(); ++iter)
    {
        sum += *iter;
    }
    CHECK_EQUAL(sum, 10);

    for (auto iter = a.begin(); iter != a.end(); ++iter)
    {
        *iter *= 2;
    }
    for (std::size_t i = 0; i < 5; ++i)
    {
        CHECK_EQUAL(a[i], static_cast<int>(i * 2));
    }
}

// capacity
TEST_CASE(test_reserve)
{
    Array<int> a;
    a.reserve(10);
    CHECK_EQUAL(a.length(), 0u);
    CHECK_EQUAL(a.capacity(), 10u);

    // param < cap: not change
    std::size_t cap = a.capacity();
    a.reserve(5);
    CHECK_EQUAL(cap, a.capacity());
}

TEST_CASE(test_shrink_to_fit)
{
    Array<int> a(10);
    for (std::size_t i = 0; i < 5; ++i)
    {
        a[i] = static_cast<int>(i);
    }

    a.resize(5);
    CHECK_EQUAL(a.length(), 5u);
    CHECK_EQUAL(a.capacity(), 10u);

    a.shrink_to_fit();
    CHECK_EQUAL(a.length(), 5u);
    CHECK_EQUAL(a.capacity(), 5u);
}

// mutator
TEST_CASE(test_push_back)
{
    Array<int> a;

    // push_back(T&&)
    a.push_back(0);
    a.push_back(1);
    CHECK_EQUAL(a.length(), 2u);

    //push_back(const T&)
    for (int i = 2; i < 5; ++i)
    {
        a.push_back(i);
    }
    CHECK_EQUAL(a.length(), 5u);

    CHECK_CONTAINER_INVARIANTS(a);
}

TEST_CASE(test_pop_back)
{
    Array<int> a{0, 1, 2};

    a.pop_back();
    CHECK_EQUAL(a.capacity(), 3u);
    CHECK_EQUAL(a.length(), 2u);

    a.pop_back();
    a.pop_back();
    CHECK_EQUAL(a.length(), 0u);
    CHECK(a.empty());
}

TEST_CASE(test_resize)
{
    Array<int> a{0, 1, 2, 3, 4};

    // resize(std::size_t new_length)
    a.resize(10);
    CHECK_EQUAL(a.length(), 10u);
    CHECK_EQUAL(a[8], 0);
    CHECK_EQUAL(a.capacity(), 10u);

    a.resize(3);
    CHECK_EQUAL(a.length(), 3u);
    CHECK_EQUAL(a.capacity(), 10u);

    // resize(std::size_t new_length, const T& init)
    a.resize(5, 9);
    CHECK_EQUAL(a.length(), 5u);
    CHECK_EQUAL(a[4], 9);
}

TEST_CASE(test_clear)
{
    Array<int> a(5, 5);
    
    a.clear();
    CHECK_EQUAL(a.length(), 0u);
    CHECK_EQUAL(a.capacity(), 5u);
}

TEST_CASE(test_insert)
{
    Array<int> a{0, 1, 2, 3, 4};

    // insert in the middle
    a.insert(a.begin() + 1, 10);  // a = {0, 10, 1, 2, 3, 4}
    CHECK_EQUAL(a[1], 10);
    CHECK_EQUAL(a[2], 1);

    // insert before head
    a.insert(a.begin(), 20);      // a = {20, 0, 10, 1, 2, 3, 4}
    CHECK_EQUAL(a[0], 20);
    CHECK_EQUAL(a[1], 0);

    // insert after tail
    a.insert(a.end(), 30);        // a = {20, 0, 10, 1, 2, 3, 4, 30}
    CHECK_EQUAL(a[a.length() - 1], 30);
    CHECK_EQUAL(a[a.length() - 2], 4);

    // insert in empty array
    Array<int> b;
    b.insert(b.begin(), 100);
    CHECK_EQUAL(b.length(), 1u);
    CHECK_EQUAL(b[0], 100);
}

TEST_CASE(test_erase)
{
    Array<int> a{0, 1, 2, 3, 4};

    // erase in the middle
    a.erase(a.begin() + 1);     // a = {0, 2, 3, 4}
    CHECK_EQUAL(a.length(), 4u);
    CHECK_EQUAL(a[1], 2);

    // erase the head
    a.erase(a.begin());         // a = {2, 3, 4}
    CHECK_EQUAL(a.length(), 3u);
    CHECK_EQUAL(a[0], 2);

    // erase the tail
    a.erase(a.end() - 1);           // a = {2, 3}
    CHECK_EQUAL(a.length(), 2u);
    CHECK_EQUAL(a[a.length() - 1], 3);

    // erase range
    a.push_back(4);
    a.push_back(5);             // a = {2, 3, 4, 5}
    a.erase(a.begin() + 1, a.end());    // a = {2}
    CHECK_EQUAL(a.length(), 1u);
    CHECK_EQUAL(a[0], 2);

    // erase range, begin == end
    a.erase(a.begin(), a.begin());
    CHECK_EQUAL(a.length(), 1u);
    CHECK_EQUAL(a[0], 2);
}

// lifetime
TEST_CASE(test_lifetime_push_pop)
{
    Tracker::reset();
    {
        Array<Tracker> a;
        a.push_back(Tracker(0));
        a.push_back(Tracker(1));
        a.push_back(Tracker(2));
        CHECK_EQUAL(a.length(), 3u);

        a.pop_back();
        CHECK_EQUAL(a.length(), 2u);
    }
    Tracker::check_no_leak();
}

TEST_CASE(test_lifetime_copy)
{
    Tracker::reset();
    {
        Array<Tracker> a;
        a.push_back(Tracker(0));
        a.push_back(Tracker(1));
        a.push_back(Tracker(2));
        CHECK_EQUAL(a.length(), 3u);

        Array<Tracker> b(a);
        CHECK_EQUAL(a.length(), 3u);
    }
    Tracker::check_no_leak();
}

TEST_CASE(test_lifetime_move)
{
    Tracker::reset();
    {
        Array<Tracker> a;
        a.push_back(Tracker(0));
        a.push_back(Tracker(1));
        a.push_back(Tracker(2));
        CHECK_EQUAL(a.length(), 3u);

        Array<Tracker> b(std::move(a));
        CHECK_EQUAL(a.length(), 0u);
    }
    Tracker::check_no_leak();
}

TEST_CASE(test_lifetime_resize)
{
    Tracker::reset();
    {
        Array<Tracker> a(3);
        a.resize(6);
        CHECK_EQUAL(a.length(), 6u);
        a.resize(4);
        CHECK_EQUAL(a.length(), 4u);
    }
    Tracker::check_no_leak();
}

TEST_CASE(test_lifetime_reserve)
{
    Tracker::reset();
    {
        Array<Tracker> a(3);
        a.reserve(6);
        CHECK_EQUAL(a.capacity(), 6u);
        a.reserve(4);
        CHECK_EQUAL(a.capacity(), 6u);
        CHECK_EQUAL(a.length(), 3u);
    }
    Tracker::check_no_leak();
}

// move
TEST_CASE(test_move_construct_no_copy)
{
    Tracker::reset();
    {
        Array<Tracker> a(3);

        Array<Tracker> b(std::move(a));

        CHECK_EQUAL(Tracker::copy_count_, 0);
        CHECK_EQUAL(Tracker::move_count_, 0);
    }
    Tracker::check_no_leak();
}

TEST_CASE(test_reserve_no_copy)
{
    Tracker::reset();
    {
        Array<Tracker> a(3);
        
        Tracker::reset();
        a.reserve(10);
        CHECK_EQUAL(Tracker::copy_count_, 0);
        CHECK_EQUAL(Tracker::move_count_, 3);
    }
}

// std::string
TEST_CASE(test_string)
{
    Array<std::string> a{"a", "b", "c"};
    CHECK_EQUAL(a.length(), 3u);
    CHECK_EQUAL(a.front(), "a");
    CHECK_EQUAL(a.at(1), "b");
    CHECK_EQUAL(*(a.end() - 1), "c");

    a.push_back("! ");
    a.erase(a.begin() + 1);
    a.insert(a.begin() + 1, "d");    // adc!
    CHECK_EQUAL(a.length(), 4u);

    Array<std::string>& b = a;
    b[0] = "aa";
    b[1] = "bb";
    b[2] = "cc";
    b[3] = "! ";
    std::string verity = "";
    for (auto s : a)
    {
        verity += s;
    }
    CHECK_EQUAL(verity, "aabbcc! ");

    a.reserve(7);
    a.resize(10);
    a[0] = "PHUC! ";
    for (int i = 0; i < 9; ++i)
    {
        a.pop_back();
    }
    CHECK_EQUAL(a.length(), 1u);

    a.clear();

    a.push_back("aaa");
    Array<std::string> c(a);
    c[0] = "ccc";
    CHECK_EQUAL(a[0], "aaa");
    CHECK_EQUAL(c[0], "ccc");
}

// exception safety
TEST_CASE(test_exception_safety_reserve_copy)
{
    CopyThrower::reset();

    Array<CopyThrower> a(5);
    for (std::size_t i = 0; i < 5; ++i)
    {
        a[i].value_ = static_cast<int>(i);
    }

    // throw when copying the 3rd data
    CopyThrower::reset();
    CopyThrower::throw_after_copy_ = 2;  // move is delete in CopyThrower, thus calls copy
    CHECK_THROWS(a.reserve(168), std::runtime_error);

    // verify that a is safe to use
    for (std::size_t i = 0; i < 5; ++i)
    {
        CHECK_EQUAL(a[i].value_, static_cast<int>(i));
    }
}

TEST_CASE(test_exception_safety_copy_assign)
{
    CopyThrower::reset();

    Array<CopyThrower> a(5);
    for (std::size_t i = 0; i < 5; ++i)
    {
        a[i].value_ = static_cast<int>(i);
    }

    // throw when copying the 3rd data
    CopyThrower::reset();
    CopyThrower::throw_after_copy_ = 2;
    Array<CopyThrower> b;
    CHECK_THROWS(b = a, std::runtime_error);

    // verify that a is safe to use
    for (std::size_t i = 0; i < 5; ++i)
    {
        CHECK_EQUAL(a[i].value_, static_cast<int>(i));
    }
}

TEST_CASE(test_exception_safety_reserve_move)
{
    MoveThrower::reset();

    Array<MoveThrower> a(5);
    
    MoveThrower::reset();
    MoveThrower::throw_after_move_ = 2;
    CHECK_THROWS(a.reserve(10), std::runtime_error);

    CHECK_EQUAL(a.length(), 5u);
}

int main()
{
    return run_all_tests();
}