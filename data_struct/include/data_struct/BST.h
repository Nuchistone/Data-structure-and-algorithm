#pragma once
#include <cstddef>
#include <initializer_list>
#include <type_traits>
#include "Array.h"

/**
 * @file BST.h
 * @brief BST (Binary Search Tree) interface declarations and inline
 *        implementations.
 * @ingroup tree_containers
 * @author Nuchistone
 */

namespace data_struct
{
    /**
     * @brief Binary Search Tree (BST), a tree container.
     * @details Each node in this tree satisfies that
     *          `left-child < node < right-child`. Duplicate
     *          insertion is not allowed in this implementation.
     *          Iterators are not supported for now.
     * @tparam T is the template type parameter; type `T` must support
     *          <, >, == and != operators.
     * @invariant
     *  - `size_` equals the actual node number in the tree rooted at
     *    `root_`;
     *  - Every node is owned by one and only one parent node.
     * @invariant Any operations modifies the number of the tree should
     *            renew the `size_` synchronously.
     * @todo
     * - [ ] Implement BST iterator.
     */
    template <typename T>
    class BST
    {
    private:
        /**
         * @brief Node class for BST.
         * @details Encapsuled data field and left/right child field;
         *          private to users of BST.
         */
        struct BSTNode
        {
            T data_;    /**< data field */
            BSTNode* left_;     /**< left child field */
            BSTNode* right_;    /**< right child field */

            // ==============================
            // constructor, copy controller
            // ==============================

            BSTNode();
            explicit BSTNode(const T& data);
            explicit BSTNode(T&& data) noexcept(std::is_nothrow_move_constructible_v<T>);

            // Copying the nodes shall not be allowed to preserve the semantics that a tree
            // owns its nodes.

            BSTNode(const BSTNode&) = delete;
            BSTNode(BSTNode&&) = delete;
            BSTNode& operator=(const BSTNode&) = delete;
            BSTNode& operator=(BSTNode&&) = delete;

            // Destructor of BSTNode destruct itself while it DO NOT the subtree. Destructing
            // the structure should be the tree's responsibility.
            ~BSTNode() = default;
        };

        // ==============================
        // private implementation of recursion
        // ==============================

        // constructor, copy controller

        BSTNode* clone_impl(BSTNode* current_subtree) const;
        void destroy_impl(BSTNode* current_subtree) const;

        // mutator

        BSTNode* insert_impl(BSTNode* current_subtree, const T& value, bool& is_inserted);
        BSTNode* remove_impl(BSTNode* current_subtree, const T& value, bool& is_removed);

        // traverse
        
        void pre_order_impl(BSTNode* current_subtree, Array<T>& result) const;
        void in_order_impl(BSTNode* current_subtree, Array<T>& result) const;
        void post_order_impl(BSTNode* current_subtree, Array<T>& result) const;

    public:
        /**
         * @name BST Constructor & Copy Controller
         * @{
         */

         /**
          * @brief Construct an empty BST.
          */
        BST();

        /**
         * @brief Constructed by an init list, elements will be inserted
         *        in the list order. 
         */
        BST(std::initializer_list<T> init);

        /**
         * @brief Copy constructor.
         * @details Implemented recursively.
         *          The recursive logic is delegated to a private method
         *          `clone_impl()` (declaration in BST.h, definition in BST.tpp).
         *          Runs in O(n) and O(h) stack space, where n is the number of
         *          nodes and h is the depth of the tree; h = O(log n) on
         *          average.
         * @exception `std::bad_alloc` if node allocation fails.
         * @warning May cause stack overflow on very deep trees.
         */
        BST(const BST& init);

        BST(BST&& init) noexcept;

        /**
         * @brief Destructor.
         * @details Implemented recursively
         *          The recursive logic is delegated to method clear(), and to
         *          a private method `destroy_impl()`
         *          (declaration in BST.h, definition in BST.tpp) furtherly.
         *          Runs in O(n) and O(h) stack space, where n is the number of
         *          nodes and h is the depth of the tree; h = O(log n) on
         *          average.
         * @warning May cause stack overflow on very deep trees.
         */
        ~BST();

        BST& operator=(const BST& init);
        BST& operator=(BST&& init) noexcept;

        /** @} */   // name BST Constructor & Copy Controller

        /**
         * @name BST Capacity Methods
         * @{
         */

        /**
         * @brief `size_` getter.
         */
        std::size_t size() const noexcept
        {
            return size_;
        }

        /**
         * @brief Judge whether the tree owns no element.
         */
        bool empty() const noexcept
        {
            return size_ == 0;
        }
        /** @} */   // name BST Capacity Methods

        /**
         * @name BST Mutator
         * @{
         */

        /**
         * @brief Inserts a node.
         * @details Implemented recursively.
         *          If the key already exists, returns false, insertion
         *          fails and the tree will not be modified. Implemented
         *          by a private method `insert_impl()` (declaration in
         *          BST.h, definition in BST.tpp). Complexity: runs in
         *          O(h), O(h) stack space, where h is the depth of the
         *          tree; O(log n) on average, O(n) worst case, where n
         *          is the number of nodes in the tree.
         * @return Returns a bool to indicate whether insertion succeed.
         *         False if atempt inserting an existing key or when
         *         exception are thrown before adding the new node.
         * @exception `std::bad_alloc` if the allocation of new node fails,
         *            the tree will not be modified (strong exception
         *            guarantee).
         * @post `size_` plus 1 if the insertion succeeds.
         * @warning May cause stack overflow on a very deep tree.
         */
        bool insert(const T& value);

        /**
         * @brief Remove a node.
         * @details Implemented recursively.
         *          If the key do not exists, returns false, remove
         *          fails and the tree will not be modified. Implemented
         *          by a private method `remove_impl()` (declaration in
         *          BST.h, definition in BST.tpp). Complexity: runs in
         *          O(h), O(h) stack space, where h is the depth of the
         *          tree; O(log n) on average, O(n) worst case, where n
         *          is the number of nodes in the tree.
         * @return Returns a bool to indicate whether remove succeed.
         *         False if atempt removing an unexist node.
         * @post `size_` minus 1 if the remove succeed.
         * @warning May cause stack overflow on a very deep tree.
         */
        bool remove(const T& value);

        /**
         * @brief Set the tree to empty.
         * @details Implemented recursively
         *          Recursive logic is delegated to a private method
         *          `destroy_impl()` (declaration in BST.h, definition
         *          in BST.tpp). Complexity: runs in O(n), O(h) stack
         *          space, where n is the number of nodes in the tree
         *          and h is the depth of the tree; h = O(log n) on
         *          average, O(n) worst case.
         * @pre Does nothing if is called on an empty tree.
         * @warning May cause stack overflow on a very deep tree.
         */
        void clear();

        /** @} */   // name BST Mutator

        /**
         * @name BST Algorithm
         * @{
         */

        /**
         * @brief Indicate whether a key is in the tree.
         * @details Indicate whether a key is in the tree; O(log n) on average,
         *          where n is the number of the nodes.
         * @return true if the key is found; false if not.
         */
        bool find(const T& value) const;

        /** @} */   // name BST Algorithm

        /**
         * @name BST Traverse
         * @{
         */

        /**
         * @brief Do the pre-order traversion on the BST.
         * @details Implemented recursively.
         *          Recursive logic is delegated to a private method
         *          `pre_order_impl()` (declaration in BST.h, definition in
         *          BST.tpp). Complexity: runs in O(n), O(h) stack space,
         *          where n is the number of the nodes and h is the depth
         *          of the tree; h = O(log n) on average, O(n) worst case.
         * @return Returns an Array to record the traversion
         *         result; returns an empty Array if the tree is empty.
         * @exception `std::bad_alloc` if allocation fails during manipulating
         *            the Array.
         * @pre Safe to call on empty tree; returns an empty Array in such case.
         * @warning May cause stack overflow on a very deep tree.
         */
        Array<T> pre_order() const;

        /**
         * @brief Do the in-order traversion on the BST.
         * @details Implemented recursively.
         *          Recursive logic is delegated to a private method
         *          `in_order_impl()` (declaration in BST.h, definition in
         *          BST.tpp). Complexity: runs in O(n), O(h) stack space,
         *          where n is the number of the nodes and h is the depth
         *          of the tree; h = O(log n) on average, O(n) worst case.
         * @return Returns an Array to record the traversion
         *         result; returns an empty Array if the tree is empty.
         * @exception `std::bad_alloc` if allocation fails during manipulating
         *            the Array.
         * @pre Safe to call on empty tree; returns an empty Array in such case.
         * @warning May cause stack overflow on a very deep tree.
         */
        Array<T> in_order() const;

        /**
         * @brief Do the post-order traversion on the BST.
         * @details Implemented recursively.
         *          Recursive logic is delegated to an private method
         *          `post_order_impl()` (declaration in BST.h, definition in
         *          BST.tpp). Complexity: runs in O(n), O(h) stack space,
         *          where n is the number of the nodes and h is the depth
         *          of the tree; h = O(log n) on average, O(n) worst case.
         * @return Returns an Array to record the traversion
         *         result; returns an empty Array if the tree is empty.
         * @exception `std::bad_alloc` if allocation fails during manipulating
         *            the Array.
         * @pre Safe to call on empty tree; returns an empty Array in such case.
         * @warning May cause stack overflow on a very deep tree.
         */
        Array<T> post_order() const;
        
        /**
         * @brief Do the level-order traversion on the BST.
         * @details Implemented non-recursively.
         *          Use Queue to manage the nodes; implemented non-recursively.
         * @return Returns an Array to record the traversion
         *         result; returns an empty Array if the tree is empty.
         * @exception `std::bad_alloc` if allocation fails during manipulating
         *            the Queue (List).
         * @pre Safe to call on empty tree; returns an empty Array in such case.
         * @warning May cause stack overflow on a very deep tree.
         */
        Array<T> level_order() const;

        /** @} */   // name BST Traverse

        /**
         * @name BST Swap
         * @{
         */

        /**
         * @brief Swap with another BST.
         * @details Swaps `root_` pointer and `size_`. O(1).
         */
        void swap(BST& other) noexcept;

        /**
         * @brief Swap BSTs.
         * @details Swaps `root_` pointer and `size_`, O(1); calls the member method
         *          swap(), defined in BST.tpp.
         */
        friend void swap(BST& a, BST& b) noexcept   // hidden friend
        {
            a.swap(b);
        }

        /** @} */   // name BST Swap

    private:
        BSTNode* root_;     /**< root node */
        std::size_t size_;  /**< number of nodes */
    };  // template class BST

    /**
     * @name BST Operator
     * @{
     */

    /**
     * @brief Judges whether two BSTs owns the same elements; DO NOT
     *        judges the sturcture.
     * @details Uses the result of in-order traversion to judge whether
     *          two BSTs owns the same elements. Trees with the same
     *          elements but different structures will be judged as
     *          equal.
     * @tparam T is the type of BST template.
     * @relates data_struct::BST
     */
    template <typename T>
    bool operator==(const BST<T>&, const BST<T>&);

    /**
     * @brief Judges whether two BSTs owns different elements.
     * @details Calls operator==().
     * @tparam T is the type of BST template.
     * @relates data_struct::BST
     */
    template <typename T>
    bool operator!=(const BST<T>& a, const BST<T>& b)
    {
        return !(a == b);
    }

    /** @} */   // name BST Operator
} // namespace data_struct

#include "BST.tpp"