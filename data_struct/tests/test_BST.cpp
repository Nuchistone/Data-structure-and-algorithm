#include "BST.h"
#include "test_utils.h"
#include <utility>
#include <string>

using data_struct::Array;
using data_struct::BST;

// ==============================
// util functions
// ==============================

// check if an Array is in order
template <typename T>
bool is_ordered(const Array<T> array)
{
    for (std::size_t i = 0; i < array.length() - 1; ++i)
    {
        if (array[i] > array[i + 1])
        {
            return false;
        }
    }
    return true;
}

// check if two trees has the same structure and data
template <typename T>
void check_bst_equal_impl(
    const BST<T>& a,
    const BST<T>& b,
    const char* a_str,
    const char* b_str,
    const char* file,
    int line
)
{
    Array<T> a_pre = a.pre_order();
    Array<T> a_in = a.in_order();
    Array<T> b_pre = b.pre_order();
    Array<T> b_in = b.in_order();
    bool bst_equal = (a_pre == b_pre) && (a_in == b_in);
    test_util_details::fail(
        bst_equal,
        std::string("CHECK_BST_EQUAL(") + a_str + ", " + b_str + ")",
        file,
        line
    );
}

#define CHECK_BST_EQUAL(a, b)  \
    check_bst_equal_impl((a), (b), #a, #b, __FILE__, __LINE__)

// BST invariant check
template <typename T>
void check_bst_invariants_impl(const BST<T>& bst)
{
    if (bst.empty())
    {
        CHECK_EQUAL(bst.size(), 0u);
    }
    else
    {
        CHECK(bst.size() > 0u);
    }
}

#define CHECK_BST_INVARIANTS(bst)   \
    check_bst_invariants_impl((bst))

// ==============================
// construct
// ==============================

TEST_CASE(test_bst_default_construct)
{
    BST<int> a;
    CHECK(a.empty());
    CHECK_EQUAL(a.size(), 0u);
}

TEST_CASE(test_bst_init_list_construct)
{
    BST<int> a{8, 3, 5, 6, 1};
    CHECK_EQUAL(a.size(), 5u);
    Array<int> a_in_order = a.in_order();
    CHECK(is_ordered(a_in_order));
}

TEST_CASE(test_bst_copy_construct)
{
    BST<int> a{5, 3, 7, 4, 6, 2, 8};
    BST<int> a2{5, 3, 7, 4, 6, 2, 8};   // a2 == a
    /*
    a, a2:
          5
       /    \
      3      7
     / \    / \
    2   4  6   8
    pre: 5 3 2 4 7 6 8
    in : 2 3 4 5 6 7 8
    */
    Array<int> a_pre{5, 3, 2, 4, 7, 6, 8};
    Array<int> a_in{2, 3, 4, 5, 6, 7, 8};
    CHECK_EQUAL(a.pre_order(), a_pre);
    CHECK_EQUAL(a.in_order(), a_in);

    BST<int> b(a);
    CHECK_EQUAL(b.size(), 7u);
    CHECK_BST_EQUAL(b, a2);

    // deep copy
    b.insert(10);
    CHECK(b.find(10));  // true
    CHECK(!a.find(10));  // false
}

TEST_CASE(test_bst_move_construct)
{
    BST<int> a{5, 3, 7, 4, 6, 2, 8};
    BST<int> a2{5, 3, 7, 4, 6, 2, 8};   // a2 == a
    /*
    a, a2:
          5
       /    \
      3      7
     / \    / \
    2   4  6   8
    pre: 5 3 2 4 7 6 8
    in : 2 3 4 5 6 7 8
    */
    Array<int> a_pre{5, 3, 2, 4, 7, 6, 8};
    Array<int> a_in{2, 3, 4, 5, 6, 7, 8};
    CHECK_EQUAL(a.pre_order(), a_pre);
    CHECK_EQUAL(a.in_order(), a_in);
   
    BST<int> b(std::move(a));
    CHECK_EQUAL(b.size(), 7u);
    CHECK_BST_EQUAL(b, a2);

    // a is empty after move
    CHECK(a.empty());
    CHECK_EQUAL(a.size(), 0u);
}

// ==============================
// assign
// ==============================

TEST_CASE(test_bst_copy_assign)
{
    BST<int> a{5, 3, 7, 4, 6, 2, 8};
    /*
    a:
          5
       /    \
      3      7
     / \    / \
    2   4  6   8
    */

    BST<int> b{2, 1, 3};
    /*
    b:
      2
     / \
    1   3
    */

    b = a;
    CHECK_BST_EQUAL(b, a);

    // self assigning
    b = b;
    CHECK_BST_EQUAL(b, a);
}

TEST_CASE(test_bst_move_assign)
{
    BST<int> a{5, 3, 7, 4, 6, 2, 8};
    BST<int> a2{5, 3, 7, 4, 6, 2, 8};   // a2 == a
    /*
    a, a2:
          5
       /    \
      3      7
     / \    / \
    2   4  6   8
    pre: 5 3 2 4 7 6 8
    in : 2 3 4 5 6 7 8
    */
    Array<int> a_pre{5, 3, 2, 4, 7, 6, 8};
    Array<int> a_in{2, 3, 4, 5, 6, 7, 8};
    CHECK_EQUAL(a.pre_order(), a_pre);
    CHECK_EQUAL(a.in_order(), a_in);

    BST<int> b{2, 1, 3};
    /*
    b:
      2
     / \
    1   3
    */

    b = std::move(a);
    CHECK_EQUAL(b.size(), 7u);
    CHECK_BST_EQUAL(b, a2);

    CHECK(a.empty());
}

TEST_CASE(test_bst_swap)
{
    BST<int> a{5, 3, 7, 4, 6, 2, 8};
    /*
    a:
          5
       /    \
      3      7
     / \    / \
    2   4  6   8
    pre: 5 3 2 4 7 6 8
    in : 2 3 4 5 6 7 8
    */
    Array<int> a_pre{5, 3, 2, 4, 7, 6, 8};
    Array<int> a_in{2, 3, 4, 5, 6, 7, 8};

    BST<int> b{2, 1, 3};
    /*
    b:
      2
     / \
    1   3
    pre: 2 1 3
    in : 1 2 3
    */
    Array<int> b_pre{2, 1, 3};
    Array<int> b_in{1, 2, 3};

    a.swap(b);
    CHECK_EQUAL(a.pre_order(), b_pre);
    CHECK_EQUAL(a.in_order(), b_in);
    CHECK_EQUAL(b.pre_order(), a_pre);
    CHECK_EQUAL(b.in_order(), a_in);

    swap(a, b);
    CHECK_EQUAL(a.pre_order(), a_pre);
    CHECK_EQUAL(a.in_order(), a_in);
    CHECK_EQUAL(b.pre_order(), b_pre);
    CHECK_EQUAL(b.in_order(), b_in);
}

// ==============================
// mutator
// ==============================

TEST_CASE(test_bst_insert)
{
    // empty tree
    BST<int> empty_bst;
    CHECK(empty_bst.empty());

    empty_bst.insert(5);
    Array<int> empty_bst_pre{5};
    Array<int> empty_bst_in{5};
    CHECK_EQUAL(empty_bst.pre_order(), empty_bst_pre);
    CHECK_EQUAL(empty_bst.in_order(), empty_bst_in);

    // non-empty tree
    BST<int> a{5, 3, 7, 4, 6};
    /*
    a:
          5
       /    \
      3      7
       \    /
        4  6
    pre: 5 3 4 7 6
    in : 3 4 5 6 7
    */
    Array<int> a_pre{5, 3, 4, 7, 6};
    Array<int> a_in{3, 4, 5, 6, 7};

    // insert 5
    // 5 already existed, so insert shall fail,
    // and the tree will not be modified
    CHECK(!a.insert(5));
    CHECK_EQUAL(a.pre_order(), a_pre);
    CHECK_EQUAL(a.in_order(), a_in);

    // insert 2
    /*
    a:
          5
       /    \
      3      7
     / \    /
    2   4  6
    pre: 5 3 2 4 7 6
    in : 2 3 4 5 6 7
    */
    Array<int> a_pre2{5, 3, 2, 4, 7, 6};
    Array<int> a_in2{2, 3, 4, 5, 6, 7};
    CHECK(a.insert(2));
    CHECK_EQUAL(a.pre_order(), a_pre2);
    CHECK_EQUAL(a.in_order(), a_in2);

    // insert 8
    /*
    a:
          5
       /    \
      3      7
     / \    / \
    2   4  6   8
    pre: 5 3 2 4 7 6 8
    in : 2 3 4 5 6 7 8
    */
    Array<int> a_pre8{5, 3, 2, 4, 7, 6, 8};
    Array<int> a_in8{2, 3, 4, 5, 6, 7, 8};
    CHECK(a.insert(8));
    CHECK_EQUAL(a.pre_order(), a_pre8);
    CHECK_EQUAL(a.in_order(), a_in8);
}

TEST_CASE(test_bst_remove)
{
    // empty tree
    BST<int> empty_bst;
    CHECK(empty_bst.empty());

    CHECK(!empty_bst.remove(0));
    CHECK(empty_bst.empty());

    // non-empty tree
    BST<int> a{5, 3, 7, 4, 6, 2, 9, 8, 10};
    /*
    a:
          5
       /    \
      3      7
     / \    / \
    2   4  6   9
              / \
             8  10
    pre: 5 3 2 4 7 6 9 8 10
    in : 2 3 4 5 6 7 8 9 10
    */
    Array<int> a_pre{5, 3, 2, 4, 7, 6, 9, 8, 10};
    Array<int> a_in{2, 3, 4, 5, 6, 7, 8, 9, 10};

    // remove 11
    // 11 does not exist, so the remove shall fail,
    // and the tree will not be modified
    CHECK(!a.remove(11));
    CHECK_EQUAL(a.pre_order(), a_pre);
    CHECK_EQUAL(a.in_order(), a_in);

    // remove a leaf -- 4
    /*
    a:
          5
       /    \
      3      7
     /      / \
    2      6   9
              / \
             8  10
    pre: 5 3 2 7 6 9 8 10
    in : 2 3 5 6 7 8 9 10
    */
    Array<int> a_pre4{5, 3, 2, 7, 6, 9, 8, 10};
    Array<int> a_in4{2, 3, 5, 6, 7, 8, 9, 10};
    CHECK(a.remove(4));
    CHECK_EQUAL(a.pre_order(), a_pre4);
    CHECK_EQUAL(a.in_order(), a_in4);

    // remove a node with only one child -- 3
    /*
    a:
        5
     /    \
    2      7
          / \
         6   9
            / \
           8  10
    pre: 5 2 7 6 9 8 10
    in : 2 5 6 7 8 9 10
    */
    Array<int> a_pre3{5, 2, 7, 6, 9, 8, 10};
    Array<int> a_in3{2, 5, 6, 7, 8, 9, 10};
    CHECK(a.remove(3));
    CHECK_EQUAL(a.pre_order(), a_pre3);
    CHECK_EQUAL(a.in_order(), a_in3);

    // remove a node with two children -- 9
    // case: min_parent == current_subtree (in remove_impl)
    /*
    a:
        5
     /    \
    2      7
          / \
         6  10
            /
           8
    pre: 5 2 7 6 10 8
    in : 2 5 6 7 8 10
    */
    Array<int> a_pre9{5, 2, 7, 6, 10, 8};
    Array<int> a_in9{2, 5, 6, 7, 8, 10};
    CHECK(a.remove(9));
    CHECK_EQUAL(a.pre_order(), a_pre9);
    CHECK_EQUAL(a.in_order(), a_in9);

    // remove a node with two children -- 7
    // case: min_parent != current_subtree (in remove_impl)
    /*
    a:
        5
     /    \
    2      8
          / \
         6  10
    pre: 5 2 8 6 10
    in : 2 5 6 8 10
    */
    Array<int> a_pre7{5, 2, 8, 6, 10};
    Array<int> a_in7{2, 5, 6, 8, 10};
    CHECK(a.remove(7));
    CHECK_EQUAL(a.pre_order(), a_pre7);
    CHECK_EQUAL(a.in_order(), a_in7);

    // remove the root -- 5
    /*
    a:
        6
     /    \
    2      8
            \
            10
    pre: 6 2 8 10
    in : 2 6 8 10
    */
    Array<int> a_pre5{6, 2, 8, 10};
    Array<int> a_in5{2, 6, 8, 10};
    CHECK(a.remove(5));
    CHECK_EQUAL(a.pre_order(), a_pre5);
    CHECK_EQUAL(a.in_order(), a_in5);
}

TEST_CASE(test_bst_clear)
{
    // empty tree
    BST<int> empty_bst;
    CHECK(empty_bst.empty());
    empty_bst.clear();
    CHECK(empty_bst.empty());

    // non-empty tree
    BST<int> a{5, 3, 7, 4, 6, 2, 8};
    a.clear();
    CHECK(a.empty());
}

// algorithm
TEST_CASE(test_bst_find)
{
    // empty tree
    BST<int> empty_bst;
    CHECK(empty_bst.empty());
    CHECK(!empty_bst.find(10));

    // non-empty tree
    BST<int> a{5, 3, 7, 4, 6, 2, 8};
    BST<int> a2{5, 3, 7, 4, 6, 2, 8};   // a2 == a
    /*
    a, a2:
          5
       /    \
      3      7
     / \    / \
    2   4  6   8
    */

    // find 10
    // 10 does not exist, so the finding shall fail,
    // and the tree will not be modified
    CHECK(!a.find(10));
    CHECK_BST_EQUAL(a, a2);

    // find the root -- 5
    CHECK(a.find(5));
    CHECK_BST_EQUAL(a, a2);

    // find a normal node -- 7
    CHECK(a.find(7));
    CHECK_BST_EQUAL(a, a2);

    // find a leaf -- 4
    CHECK(a.find(4));
    CHECK_BST_EQUAL(a, a2);
}

// ==============================
// traverse
// ==============================

TEST_CASE(text_bst_pre_order)
{
    // empty tree
    BST<int> empty_bst;
    CHECK(empty_bst.empty());

    Array<int> empty_pre = empty_bst.pre_order();
    CHECK(empty_pre.empty());

    // non-empty tree
    BST<int> a{5, 3, 7, 4, 6, 2, 9, 8, 10};
    /*
    a:
          5
       /    \
      3      7
     / \    / \
    2   4  6   9
              / \
             8  10
    pre: 5 3 2 4 7 6 9 8 10
    */
    Array<int> a_pre{5, 3, 2, 4, 7, 6, 9, 8, 10};
    CHECK_EQUAL(a.pre_order(), a_pre);
}

TEST_CASE(test_bst_in_order)
{
    // empty tree
    BST<int> empty_bst;
    CHECK(empty_bst.empty());

    Array<int> empty_in = empty_bst.in_order();
    CHECK(empty_in.empty());

    // non-empty tree
    BST<int> a{5, 3, 7, 4, 6, 2, 9, 8, 10};
    /*
    a:
          5
       /    \
      3      7
     / \    / \
    2   4  6   9
              / \
             8  10
    in: 2 3 4 5 6 7 8 9 10
    */
    Array<int> a_in{2, 3, 4, 5, 6, 7, 8, 9, 10};
    CHECK_EQUAL(a.in_order(), a_in);
}

TEST_CASE(test_bst_post_order)
{
    // empty tree
    BST<int> empty_bst;
    CHECK(empty_bst.empty());

    Array<int> empty_post = empty_bst.post_order();
    CHECK(empty_post.empty());

    // non-empty tree
    BST<int> a{5, 3, 7, 4, 6, 2, 9, 8, 10};
    /*
    a:
          5
       /    \
      3      7
     / \    / \
    2   4  6   9
              / \
             8  10
    post: 2 4 3 6 8 10 9 7 5
    */
    Array<int> a_post{2, 4, 3, 6, 8, 10, 9, 7, 5};
    CHECK_EQUAL(a.post_order(), a_post);
}

TEST_CASE(test_bst_level_order)
{
    // empty tree
    BST<int> empty_bst;
    CHECK(empty_bst.empty());

    Array<int> empty_level = empty_bst.level_order();
    CHECK(empty_level.empty());

    // non-empty tree
    BST<int> a{5, 3, 7, 4, 6, 2, 9, 8, 10};
    /*
    a:
          5
       /    \
      3      7
     / \    / \
    2   4  6   9
              / \
             8  10
    level: 5 3 7 2 4 6 9 8 10
    */
    Array<int> a_level{5, 3, 7, 2, 4, 6, 9, 8, 10};
    CHECK_EQUAL(a.level_order(), a_level);
}

// ==============================
// operator
// ==============================

TEST_CASE(test_bst_operator_equal)
{
    BST<int> a1{1, 2, 3, 4, 5};
    BST<int> a2{3, 1, 2, 5, 4};
    BST<int> a3{5, 4, 3, 2, 1};
    BST<int> a4{4, 5, 3, 2};

    // reflexivity
    CHECK_EQUAL(a1, a1);

    // symmetry
    CHECK_EQUAL(a1, a2);
    CHECK_EQUAL(a2, a1);

    // transitivity
    CHECK_EQUAL(a1, a2);
    CHECK_EQUAL(a2, a3);
    CHECK_EQUAL(a1, a3);

    // not equal
    CHECK_NOT_EQUAL(a1, a4);
}

// ==============================
// lifetime
// ==============================

TEST_CASE(test_bst_lifetime)
{
    Tracker::reset();
    {
        BST<Tracker> a;
        a.insert(Tracker(10));
        a.insert(Tracker(5));
        a.insert(Tracker(15));

        Tracker t1(7);
        Tracker t2(13);
        a.insert(t1);
        a.insert(t2);

        CHECK_EQUAL(a.size(), 5u);
        CHECK(a.find(Tracker(5)));
        CHECK(!a.find(Tracker(314)));
        a.remove(Tracker(5));
        a.remove(Tracker(7));

        CHECK_EQUAL(a.size(), 3u);
    }
    Tracker::check_no_leak();
}

// ==============================
// exception
// ==============================

TEST_CASE(test_bst_copy_exception_safety)
{
    CopyThrower::reset();
    {   // throw at root -- 5
        BST<CopyThrower> a;
        a.insert(CopyThrower(5));
        a.insert(CopyThrower(10));
        a.insert(CopyThrower(3));
        a.insert(CopyThrower(1));
        /*
        a:
              5
           /    \
          3     10
         /
        1
        pre: 10 5 7 15 13
        in: 5 7 10 13 15
        */

        CopyThrower::reset();
        CopyThrower::throw_after_copy_ = 0;

        CHECK_THROWS(BST<CopyThrower>(a), std::runtime_error);

        CopyThrower::throw_after_copy_ = -1;
        CHECK_EQUAL(a.size(), 4u);
        CHECK(a.find(CopyThrower(5)));
    }
    {   // throw at left -- 3
        BST<CopyThrower> a;
        a.insert(CopyThrower(5));
        a.insert(CopyThrower(10));
        a.insert(CopyThrower(3));
        a.insert(CopyThrower(1));
        /*
        a:
              5
           /    \
          3     10
         /
        1
        pre: 10 5 7 15 13
        in: 5 7 10 13 15
        */

        CopyThrower::reset();
        CopyThrower::throw_after_copy_ = 1;

        CHECK_THROWS(BST<CopyThrower>(a), std::runtime_error);

        CopyThrower::throw_after_copy_ = -1;
        CHECK_EQUAL(a.size(), 4u);
        CHECK(a.find(CopyThrower(5)));
    }
    {   // throw at right -- 10
        BST<CopyThrower> a;
        a.insert(CopyThrower(5));
        a.insert(CopyThrower(10));
        a.insert(CopyThrower(3));
        a.insert(CopyThrower(1));
        /*
        a:
              5
           /    \
          3     10
         /
        1
        pre: 10 5 7 15 13
        in: 5 7 10 13 15
        */

        CopyThrower::reset();
        CopyThrower::throw_after_copy_ = 3;

        CHECK_THROWS(BST<CopyThrower>(a), std::runtime_error);

        CopyThrower::throw_after_copy_ = -1;
        CHECK_EQUAL(a.size(), 4u);
        CHECK(a.find(CopyThrower(5)));
    }
}

TEST_CASE(test_bst_assign_exception_safety)
{
    CopyThrower::reset();
    {   // throw at root -- 40
        BST<CopyThrower> a;
        a.insert(CopyThrower(40));
        a.insert(CopyThrower(50));
        a.insert(CopyThrower(30));

        BST<CopyThrower> b;
        b.insert(CopyThrower(10));
        b.insert(CopyThrower(5));
        b.insert(CopyThrower(7));
        b.insert(CopyThrower(15));
        b.insert(CopyThrower(13));
        /*
        b:
             10
           /    \
          5     15
           \    /
            7  13
        pre: 10 5 7 15 13
        in: 5 7 10 13 15
        */
        BST<CopyThrower> b2{
            CopyThrower(10),
            CopyThrower(5),
            CopyThrower(7),
            CopyThrower(15),
            CopyThrower(13)
        };

        CopyThrower::reset();
        CopyThrower::throw_after_copy_ = 0;

        CHECK_THROWS(b = a, std::runtime_error);

        CopyThrower::throw_after_copy_ = -1;
        CHECK_BST_EQUAL(b, b2);
        CHECK(!b.find(CopyThrower(30)));
        CHECK(!b.find(CopyThrower(50)));
    }
    {   // throw at left -- 30
        BST<CopyThrower> a;
        a.insert(CopyThrower(40));
        a.insert(CopyThrower(50));
        a.insert(CopyThrower(30));

        BST<CopyThrower> b;
        b.insert(CopyThrower(10));
        b.insert(CopyThrower(5));
        b.insert(CopyThrower(7));
        b.insert(CopyThrower(15));
        b.insert(CopyThrower(13));
        /*
        b:
             10
           /    \
          5     15
           \    /
            7  13
        pre: 10 5 7 15 13
        in: 5 7 10 13 15
        */
        BST<CopyThrower> b2{
            CopyThrower(10),
            CopyThrower(5),
            CopyThrower(7),
            CopyThrower(15),
            CopyThrower(13)
        };

        CopyThrower::reset();
        CopyThrower::throw_after_copy_ = 1;

        CHECK_THROWS(b = a, std::runtime_error);

        CopyThrower::throw_after_copy_ = -1;
        CHECK_BST_EQUAL(b, b2);
        CHECK(!b.find(CopyThrower(30)));
        CHECK(!b.find(CopyThrower(50)));
    }
    {   // throw at right -- 40
        BST<CopyThrower> a;
        a.insert(CopyThrower(40));
        a.insert(CopyThrower(50));
        a.insert(CopyThrower(30));

        BST<CopyThrower> b;
        b.insert(CopyThrower(10));
        b.insert(CopyThrower(5));
        b.insert(CopyThrower(7));
        b.insert(CopyThrower(15));
        b.insert(CopyThrower(13));
        /*
        b:
             10
           /    \
          5     15
           \    /
            7  13
        pre: 10 5 7 15 13
        in: 5 7 10 13 15
        */
        BST<CopyThrower> b2(b);

        CopyThrower::reset();
        CopyThrower::throw_after_copy_ = 2;

        CHECK_THROWS(b = a, std::runtime_error);

        CopyThrower::throw_after_copy_ = -1;
        CHECK_BST_EQUAL(b, b2);
        CHECK(!b.find(CopyThrower(30)));
        CHECK(!b.find(CopyThrower(50)));
    }
}

TEST_CASE(test_bst_insert_exception_safety)
{
    CopyThrower::reset();
    {
        BST<CopyThrower> a;
        a.insert(CopyThrower(20));
        a.insert(CopyThrower(10));
        a.insert(CopyThrower(40));
        BST<CopyThrower> a2(a);

        CopyThrower thrower(5);

        CopyThrower::reset();
        CopyThrower::throw_after_copy_ = 0;

        CHECK_THROWS(CopyThrower(thrower), std::runtime_error);

        CopyThrower::throw_after_copy_ = -1;
        CHECK_EQUAL(thrower.value_, 5);
        CHECK_BST_EQUAL(a, a2);
        CHECK(!a.find(CopyThrower(5)));
    }
}

int main()
{
    return run_all_tests();
}