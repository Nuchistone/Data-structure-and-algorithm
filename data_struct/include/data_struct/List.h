#pragma once
#include <cstddef>
#include <initializer_list>
#include <type_traits>
#include <string>

/**
 * @file List.h
 * @brief List (doubly linked list) declarations and inilne
 *        implementations.
 * @ingroup sequence_containers
 * @author Nuchistone
 */

namespace data_struct
{
    /**
     * @brief Douby linked list, a sequence container.
     * @details A list where each node has one data field and two pointer
     *          field pointing to its next node and previous node.
     *          Insertion and deletion at arbitrary takes O(1) time while
     *          finding takes O(n) on average, where n is the length of
     *          the list.
     *          Use a sentinel to manage the list, thus boundary conditions
     *          could be aggregated to the normal ones.
     *          Elements in this container are arranged as the order
     *          they are pushed in.
     *          Iterators of this container, including iterator and
     *          const_iterator, are supported.
     * @tparam T The template type paarmeter.
     * @invariant
     *  - `size_` equals the node number in the list; `sentinel_` are
     *    excluded.
     *  - The `next_` of `sentinel_` is the head node while the `prev_` of it
     *    is the tail node.
     *  - In an empty list, the `next_` and `prev_` of `sentinel_` are both
     *    the sentinel itself.
     * @invariant Any operations modifies the number of the list should renew
     *            `size_` synchronously.
     * @todo
     *  - [ ] More generic algorithms in List.
     */
    template <typename T>
    class List
    {
    private:
        /**
         * @brief Node class for List.
         * @details Encapsuled data field and prev/next node field; private to
         *          users of List.
         */
        struct Node
        {
            T data_;        /**< data field */
            Node* prev_;    /**< previous node field */
            Node* next_;    /** next node field */

            // ==============================
            // constructor, copy controller
            // ==============================

            Node();
            explicit Node(const T& data);
            explicit Node(T&& data) noexcept(std::is_nothrow_move_constructible_v<T>);

            // Copying the nodes shall not be allowed to preserve the semantics
            // that a list owns its nodes.

            Node(const Node&) = delete;
            Node(Node&&) = delete;
            Node& operator=(const Node&) = delete;
            Node& operator=(Node&&) = delete;

            // Destructor of Node destructs the node itself while DO NOT destruct
            // its previous node and next node. Destructing the structure should
            // be the list's responsibility.
            ~Node() = default;
        };

    public:
        /**
         * @name List Constructor & Copy Controller
         * @{
         */

        /**
         * @brief Construct an empty List; sentinel will be initialized.
         */
        List();

        /**
         * @brief Constructd by an init list, elements will be inserted
         *        in the list order.
         * @tparam T The template type parameter of List.
         * @exception `std::bad_alloc` If allocation fails.
         * 
         *  Other guarantees:
         * 
         *  - Any exceptions thrown by `T(const T&)` will propagate.
         * 
         *  - The List will not be modified if exceptions are thrown (strong
         *  exception guarantee).
         */
        List(std::initializer_list<T> init);

        /**
         * @brief Filling a new List with `size` copies of `init`.
         * @tparam T The template type parameter of List.
         * @exception `std::bad_alloc` If allocation fails.
         * 
         *  Other guarantees:
         * 
         *  - Any exceptions thrown by `T(const T&)` will propagate.
         * 
         *  - The List will not be modified if exceptions are thrown (strong
         *  exception guarantee).
         */
        List(std::size_t size, const T& init);

        /**
         * @brief Copy constructor.
         * @tparam T The template type parameter of List.
         * @exception `std::bad_alloc` If allocation fails.
         * 
         *  Other guarantees:
         * 
         *  - Any exceptions thrown by `T(const T&)` will propagate.
         * 
         *  - The List will not be modified if exceptions are thrown (strong
         *  exception guarantee).
         */
        List(const List& init);

        List(List&& init) noexcept;
        ~List();
        List& operator=(const List& init);
        List& operator=(List&& init) noexcept;

        /** @} */   // name List Constructor & Copy Controller

        /**
         * @name List Getter
         * @{
         */

        /**
         * @brief Gets the first element; List must not be empty.
         * @return Returns reference to the first element. The reference will be
         *         invalidated if the first element is removed.
         * @warning List must not be empty. Violating this precondition
         *          triggers an assertion in debug builds and is UB in
         *          release builds.
         */
        T& front();

        /**
         * @brief Gets the first element, read-only; List must not be empty.
         * @return Returns const reference to the first element. The reference will
         *         be invalidated if the first element is removed.
         * @warning List must not be empty. Violating this precondition
         *          triggers an assertion in debug builds and is UB in
         *          release builds.
         */
        const T& front() const;

        /**
         * @brief Gets the last element; List must not be emtpy.
         * @return Returns reference to the last element. The reference will be
         *         invalidated if the last element is removed.
         * @warning List must not be empty. Violating this precondition
         *          triggers an assertion in debug builds and is UB in
         *          release builds.
         */
        T& back();

        /**
         * @brief Gets the last element, read-only; List must not be empty.
         * @return Returns const reference to the last element. The reference will
         *         be invalidated if the last element is removed.
         * @warning List must not be empty. Violating this precondition
         *          triggers an assertion in debug builds and is UB in
         *          release builds.
         */
        const T& back() const;

        /** @} */   // name List Getter

        /**
         * @name List Iterator
         * @{
         */

        class const_iterator;

        /**
         * @brief List Iterator class encapsulation
         * @details Encapsulation of Node*, supports iterator operators.
         */
        class iterator
        {
        friend class List;

        // `const_iterator` is a friend of `iterator` to support type
        // conversion from non-const to const;
        friend class const_iterator;

        public:

            /**
             * @name List Itertor Operator
             * @{
             */

            T& operator*() const
            {
                return node_->data_;
            }
            T* operator->() const
            {
                return &node_->data_;
            }

            bool operator==(const iterator& other) const
            {
                return node_ == other.node_;
            }
            bool operator!=(const iterator& other) const
            {
                return !(*this == other);
            }
            
            iterator& operator++()
            {
                node_ = node_->next_;
                return *this;
            }
            iterator& operator--()
            {
                node_ = node_->prev_;
                return *this;
            }
            iterator operator++(int)
            {
                iterator temp = *this;
                ++(*this);
                return temp;
            }
            iterator operator--(int)
            {
                iterator temp = *this;
                --(*this);
                return temp;
            }

            /** @} */   // name List Iterator Operator

        private:
            Node* node_;

            explicit iterator(Node* node)
                : node_(node)
            {
            }
        };  // class iterator

        class const_iterator
        {
        // `iterator` is not a friend of `const_iterator` to exhibit type
        // conversion from const to non-const;
        friend class List;

        public:

            /**
             * @brief Implicit type conversion from non-const to const.
             */
            const_iterator(const iterator& it)
                : node_(it.node_)
            {
            }

            /**
             * @name List Const_iterator Operator
             * @{
             */

            const T& operator*() const
            {
                return node_->data_;
            }
            const T* operator->() const
            {
                return &node_->data_;
            }

            bool operator==(const const_iterator& other) const
            {
                return node_ == other.node_;
            }
            bool operator!=(const const_iterator& other) const
            {
                return !(*this == other);
            }
            
            const_iterator& operator++()
            {
                node_ = node_->next_;
                return *this;
            }
            const_iterator& operator--()
            {
                node_ = node_->prev_;
                return *this;
            }
            const_iterator operator++(int)
            {
                const_iterator temp = *this;
                ++(*this);
                return temp;
            }
            const_iterator operator--(int)
            {
                const_iterator temp = *this;
                --(*this);
                return temp;
            }

            /** @} */   // name List Const_iterator Operator

        private:
            const Node* node_;

            explicit const_iterator(const Node* node)
                : node_(node)
            {
            }
        };  // class const_iterator

        /**
         * @brief Gets the head iterator.
         * @return Returns iterator to the first element. Will be
         *         invalidated if the first element is removed; semantics is
         *         not guaranteed if inserting elements at front.
         * @note Only using `cbegin()` explicitly can one assign a
         *       `const_iterator `to the non-const object, otherwise `begin()`
         *       will be matched.
         */
        iterator begin() noexcept
        {
            return iterator(sentinel_.next_);
        }

        /**
         * @brief Gets the after-tail iterator (sentinel).
         * @return Returns iterator to the sentinel. Will be invalidated if the
         *         List is destructed.
         * @note Only using `cend()` explicitly can one assign a
         *       `const_iterator `to the non-const object, otherwise `end()`
         *       will be matched.
         */
        iterator end() noexcept
        {
            return iterator(&sentinel_);
        }

        /**
         * @brief Gets the head const iterator.
         * @return Returns const iterator to the first element. Will be
         *         invalidated if the first element is removed; semantics is
         *         not guaranteed if inserting elements at front.
         * @note Only using `cbegin()` explicitly can one assign a
         *       `const_iterator `to the non-const object, otherwise `begin()`
         *       will be matched.
         */
        const_iterator begin() const noexcept
        {
            return const_iterator(sentinel_.next_);
        }

        /**
         * @brief Gets the after-tail const iterator (sentinel).
         * @return Returns const iterator to sentinel. Will be invalidated if the
         *         List is destructed.
         * @note Only using `cend()` explicitly can one assign a
         *       `const_iterator `to the non-const object, otherwise `end()`
         *       will be matched.
         */
        const_iterator end() const noexcept
        {
            return const_iterator(&sentinel_);
        }

        /**
         * @brief Gets the head const iterator.
         * @return Returns const iterator to the first element. Will be
         *         invalidated if the first element is removed; semantics is
         *         not guaranteed if inserting elements at front.
         * @note Only using `cbegin()` explicitly can one assign a
         *       `const_iterator `to the non-const object, otherwise `begin()`
         *       will be matched.
         */
        const_iterator cbegin() const noexcept
        {
            return const_iterator(sentinel_.next_);
        }

        /**
         * @brief Gets the after-tail const iterator (sentinel).
         * @return Returns const iterator to the sentinel. Will be invalidated if the
         *         List is destructed.
         * @note Only using `cend()` explicitly can one assign a
         *       `const_iterator `to the non-const object, otherwise `end()`
         *       will be matched.
         */
        const_iterator cend() const noexcept
        {
            return const_iterator(sentinel_);
        }

        /** @} */   // name List Iterator

        /**
         * @name List Capacity
         * @{
         */

        std::size_t size() const noexcept
        {
            return size_;
        }
        bool empty() const noexcept
        {
            return (sentinel_.next_ == &sentinel_);
        }

        /** @} */   // name List Capacity

        /**
         * @name List Mutator
         * @{
         */

        /**
         * @brief Insert in the front.
         * @details Insert an element in the front. O(1).
         * @tparam T The template type param of List.
         * @exception `std::bad_alloc` If allocation fails.
         * 
         *  Other guarantees:
         * 
         *  - Any exceptions thrown by `T(const T&)` will propagate.
         * 
         *  - The List will not be modified if exceptions are thrown (strong
         *  exception guarantee).
         * @post `size_` plus 1 if succeeds.
         * @warning Semantics of head iterators will not be guaranteed if succeeds.
         */
        void push_front(const T& value);

        /**
         * @brief Insert in the front.
         * @details Insert an element in the front. O(1).
         * @tparam T The template type param of List.
         * @exception `std::bad_alloc` If allocation fails.
         * 
         *  Other guarantees:
         * 
         *  - Any exceptions thrown by `T(const T&)` will propagate.
         * 
         *  - The List will not be modified if exceptions are thrown (strong
         *  exception guarantee).
         * @post `size_` plus 1 if succeeds.
         * @warning Semantics of head iterators will not be guaranteed if succeeds.
         */
        void push_front(T&& value);

        /**
         * @brief Insert in the back.
         * @details Insert an element in the back. O(1).
         * @tparam T The template type param of List.
         * @exception `std::bad_alloc` If allocation fails.
         * 
         *  Other guarantees:
         * 
         *  - Any exceptions thrown by `T(const T&)` will propagate.
         * 
         *  - The List will not be modified if exceptions are thrown (strong
         *  exception guarantee).
         * @post `size_` plus 1 if succeeds.
         */
        void push_back(const T& value);

        /**
         * @brief Insert in the back.
         * @details Insert an element in the back. O(1).
         * @tparam T The template type param of List.
         * @exception `std::bad_alloc` If allocation fails.
         * 
         *  Other guarantees:
         * 
         *  - Any exceptions thrown by `T(T&&)` will propagate.
         * 
         *  - The List will not be modified if exceptions are thrown (strong
         *  exception guarantee).
         * @post `size_` plus 1 if succeeds.
         */
        void push_back(T&& value);

        /**
         * @brief Remove the first element; List must not be empty.
         * @details Remove the first element. O(1).
         * @post `size_` minus 1 if succeeds.
         * @warning - List must not be empty. Violating this precondition
         *          triggers an assertion in debug builds and is UB in
         *          release builds.
         * @warning - Semantics of head iterators will not be guaranteed.
         */
        void pop_front();

        /**
         * @brief Remove the last element; List must not be empty.
         * @details Remove the last element. O(1).
         * @post `size_` minus 1 if succeeds.
         * @warning - List must not be empty. Violating this precondition
         *          triggers an assertion in debug builds and is UB in
         *          release builds.
         */
        void pop_back();

        /**
         * @brief Clear to an empty List.
         * @post `size_` will be set to 0.
         * @warning Iterators (except after-tail iterators) of the caller
         *          will be invalidated.
         */
        void clear();

        /**
         * @brief Cut `other` to position before `pos`.
         * @details Move all the elements in `other` to a different List. O(n),
         *          where n is the size of `other`.
         * @return Does nothing if `other` is same as the caller or `other`
         *         is empty.
         * @post `size_` of both Lists will be modified if succeeds.
         */
        void splice(iterator pos, List& other) noexcept;

        /**
         * @brief Cut `[it, it + 1)` in `other` to position before `pos`.
         * @details Move `[it, it + 1)` in `other` to position before `pos`; `other` could be the
         *          same as the caller. O(1).
         * @return Does nothing if `other` is empty, or `it` or `it + 1` coincides with `pos`.
         * @post `size_` of both Lists will be modified if succeeds.
         */
        void splice(iterator pos, List& other, iterator it) noexcept;

        /**
         * @brief Cut `[begin_it, end_it)` in `other` to position before `pos`.
         * @details Move `[begin_it, end_it)` in `other` to position before `pos`; `other` could be
         *          same as the caller. O(m), where m is the length of the moved range.
         * @return Does nothing if the transfered range is empty, or `other` is empty, or `end_it`
         *         coinsides with `pos`.
         * @post `size_` of both Lists will be modified if succeeds.
         */
        void splice(iterator pos, List& other, iterator begin_it, iterator end_it) noexcept;

        /**
         * @brief Insert before `pos`.
         * @details Insert before `pos`. O(1).
         * @tparam T The template type param of List.
         * @return Returns iterator pointing to the newly inserted element.
         * @exception `std::bad_alloc` If allocation fails.
         * 
         *  Other guarantees:
         * 
         *  - Any exceptions thrown by `T(const T&)` will propagate.
         * 
         *  - The List will not be modified if exceptions are thrown (strong
         *  exception guarantee).
         * @post `size_` plus 1 if succeeds.
         * @warning Inserting in the front makes the semantics of head iterators not guaraunteed.
         */
        iterator insert(iterator pos, const T& value);

        /**
         * @brief Insert before `pos`.
         * @details Insert before `pos`. O(1).
         * @tparam T The template type param of List.
         * @return Returns iterator pointing to the newly inserted element.
         * @exception `std::bad_alloc` If allocation fails.
         * 
         *  Other guarantees:
         * 
         *  - Any exceptions thrown by `T(T&&)` will propagate.
         * 
         *  - The List will not be modified if exceptions are thrown (strong
         *  exception guarantee).
         * @post `size_` plus 1 if succeeds.
         * @warning Inserting in the front makes the semantics of head iterators not guaraunteed.
         */
        iterator insert(iterator pos, T&& value);

        /**
         * @brief Remove element at `pos`; pos must not be after-tail iterator.
         * @details Remove element at `pos`; pos must not be after-tail iterator. O(1).
         * @return Returns iterator pointing to the element after `pos`.
         * @pre `post` must not be an after-tail iterator. Violating this precondition
         *      triggers an assertion in debug builds and is UB in
         *      release builds.
         */
        iterator erase(iterator pos);

        /**
         * @brief Remove element in `[start_it, end_it)`; both `start_it` and `end_it` must not be
         *        after-tail iterator.
         * @details Remove element in `[start_it, end_it)`; both `start_it` and `end_it` must not be
         *          after-tail iterator. O(m), where m is the length of the moved range.
         * @return Returns iterator pointing to the element after `end_it`.
         * @pre Both `start_it` and `end_it` must not be
         *      after-tail iterator. Violating this precondition
         *      triggers an assertion in debug builds and is UB in
         *      release builds.
         * @warning Iterators in the removed range might be invalidated or might not act as expected.
         */
        iterator erase(iterator start_it, iterator end_it);

        /** @} */   // name List Mutator

        /**
         * @name List Algorithm
         * @{
         */

        /**
         * @brief Find in the List.
         * @details Tranverse from the head, thus runs in O(n) on average,
         *          where n is the node number.
         * @return Returns a iterator pointing to the first found `value`;
         *         returns after-tail iterator if not found.
         */
        iterator find(const T& value);

        /**
         * @brief Find a node with key `value`.
         * @details Tranverse from the head, thus runs in O(n) on average,
         *          where n is the node number.
         * @return Returns a iterator pointing to the first found `value`;
         *         returns after-tail iterator if not found.
         */
        const_iterator find(const T& value) const;

        /**
         * @brief Remove all nodes with key `value`.
         */
        void remove(const T& value);

        // template <typename Predicate>
        // void remove_if(Predicate pred);
        // void sort();
        // template <typename Compare>
        // void sort(Compare comp);

        /** @} */   // name List Algorithm

        /**
         * @name List Swap
         * @{
         */

        /**
         * @brief Swap with another List.
         * @details Swaps sentinel's node fields and `size_`, O(1).
         */
        void swap(List& other) noexcept;

        /**
         * @brief Swap Lists.
         * @details Swaps sentinel's node fields and `size_`, O(1). Calls the member method swap().
         */
        friend void swap(List& a, List& b) noexcept // hidden friend
        {
            a.swap(b);
        }

        /** @} */   // name List Swap

    private:
        Node sentinel_;     /**< sentinel of the list */
        std::size_t size_;  /**< node number of the list */

        // cut [first, last) to the front of pos, private implementation
        // of silce() methods
        void transfer(iterator pos, iterator first, iterator last);
    };  // template class List

    /**
     * @name List Operator
     * @{
     */

    /**
     * @brief Judges whether two Lists has the same elements
     *        in the same order.
     * @tparam T The type of List tempalte.
     * @relates data_struct::List
     */
    template <typename T>
    bool operator==(const List<T>&, const List<T>&);

    /**
     * @brief Judges whether two Lists has different elements
     *        or has the same elements but not in the same order.
     * @tparam T The type of List tempalte.
     * @relates data_struct::List
     */
    template <typename T>
    bool operator!=(const List<T>& lhs, const List<T>& rhs)
    {
        return !(lhs == rhs);
    }

    /** @} */   // name List Operator
}   // namespace data_struct

#include "List.tpp"

// extern template class data_struct::List<int>;
// extern template class data_struct::List<float>;
// extern template class data_struct::List<double>;
// extern template class data_struct::List<std::string>;