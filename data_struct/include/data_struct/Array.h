#pragma once
#include <cstddef>
#include <initializer_list>
#include <algorithm>
#include <string>

/**
 * @file Array.h
 * @brief Array (array with dynamic allocation) declarations and inline
 *        implementations.
 * @ingroup sequence_containers
 * @author Nuchistone
 */

namespace data_struct
{
    /**
     * @brief Array with dynamic allocation, a sequence container.
     * @details This container supports operation on delicate allocation,
     *          including \ref data_struct::Array::reserve() and 
     *          data_struct::Array::shink_to_fit(), segregating allocation
     *          and construction.
     *          Randomly access runs at O(1) while insertion and deletion at
     *          O(n) on average, where n is the number of the elements in the
     *          array.
     *          Elements in this container are arranged as the order
     *          they are pushed in.
     *          Iterators of this container, including iterator and
     *          const_iterator, are supported.
     * @tparam T The template type parameter.
     * @invariant
     *  - `length_` equals the element number in the array, buffer in
     *    `[0, length_)` are CONSTRUCTED;
     *  - `capacity_` equals the allocated memory blocks of `sizeof(T)`,
     *    buffer in `[length_, capacity_)` are NOT CONSTRUCTED;
     *  - 'capacity_ >= length_' is always true.
     * @invariant Any operations modifies the number of the array or
     *            performed allocation should renew `length_` and `capacity_`
     *            synchronously.
     */
    template <typename T>
    class Array 
    {
    public:
        /**
         * @name Array Constructor & Copy Controller
         * @{
         */

        /**
         * @brief Construct an empty Array.
         */
        Array();

        /**
         * @brief Allocate a buffer of `length` default-constructed elements.
         * @details Allocate a buffer with size `length * sizeof(T)`, then
         *          construct defaultly; `capacity_` and `length` equals.
         * @tparam T The template type parameter.
         * @exception `std::bad_alloc` If allocation fails;
         *  
         *  Other guarantees:
         * 
         *  - Any exceptions thrown by `T()` will propagate.
         * 
         *  - The allocated buffer will be released if exceptions are thrown
         *  (strong exception guarantee).
         */
        explicit Array(std::size_t length);

        /**
         * @brief Constructed by an init list, elements arrange in list order.
         * @details Use the elements in the list to construct; `capacity_`
         *          and `length` equals.
         * @tparam T The template type parameter.
         * @param init The initializer_list with elements to put into the Array.
         * @exception `std::bad_alloc` If allocation fails;
         * 
         *  Other guarantees:
         * 
         *  - Any exceptions thrown by `T(const T&)` will propagate.
         * 
         *  - The allocated buffer will be released if exceptions are thrown
         *  (strong exception guarantee).
         */
        Array(std::initializer_list<T> init);

        /**
         * @brief Filling a new Array with `length` copies of `init`.
         * @details Use the `init` to construct; `capacity_` and `length`
         *          equals.
         * @tparam T The template type parameter of Array.
         * @exception `std::bad_alloc` If allocation fails;
         * 
         *  Other guarantees:
         * 
         *  - Any exceptions thrown by `T(const T&)` will propagate.
         * 
         *  - The allocated buffer will be released if exceptions are thrown
         *  (strong exception guarantee).
         */
        Array(std::size_t length, const T& init);

        /**
         * @brief Copy constructor.
         * @tparam T The template type parameter.
         * @exception `std::bad_alloc` If allocation fails;
         * 
         *  Other guarantees:
         * 
         *  - Any exceptions thrown by `T(const T&)` will propagate.
         * 
         *  - The allocated buffer will be released if exceptions are thrown
         *  (strong exception guarantee).
         */
        Array(const Array& init);

        Array(Array&& init) noexcept;
        ~Array();
        Array& operator=(const Array& init);
        Array& operator=(Array&& init) noexcept;

        /** @} */   // name Array Constructor & Copy Controller

        /**
         * @name Array Getter
         * @{
         */

        /**
         * @brief Subscript operator.
         * @details Gets the `pos` -th element in the Array.
         * @return Reference to the `pos` -th element. The reference will be
         *         invalidated if the element is removed or by any 
         *         operations that reallocates the Array.
         * @pre `pos` shall be within `[0, length_)`, otherwise the behavior
         *      is undefined.
         * @warning Accessing buffer in `[length_, capacity_)`,
         *          `[capacity, ...)` or `(..., 0)` through `operator[]`
         *          is UB.
         */
        T& operator[](std::size_t pos);

        /**
         * @brief Subscript operator, read-only.
         * @details Gets the `pos` -th element in the Array, const override so
         *          read only.
         * @return Const reference to the `pos` -th element. The reference will
         *         be invalidated if the element is removed or by any operations
         *         that reallocates the Array.
         * @pre `pos` shall be within `[0, length_)`, otherwise the behavior
         *      is undefined.
         * @warning Accessing buffer in `[length_, capacity_)`,
         *          `[capacity, ...)` or `(..., 0)` through `operator[]`
         *          is UB.
         */
        const T& operator[](std::size_t pos) const;

        /**
         * @brief Getter of pointer to the head address.
         * @details Gets the pointer pointing to the head address of the
         *          Array.
         * @tparam T The template type of Array.
         * @return Pointer to the head address, returns `nullptr` if empty.
         *         The pointer will be invalidated if
         *         the element is removed or by any operations that reallocates
         *         the Array.
         */
        T* data() noexcept
        {
            return data_;
        }

        /**
         * @brief Getter of pointer to the head address, read-only.
         * @details Gets the const pointer pointing to the head address
         *          of the Array.
         * @tparam T The template type of Array.
         * @return Const pointer to the head address, returns `nullptr` if 
         *         empty. The pointer will be invalidated if
         *         the element is removed or by any operations that reallocates
         *         the Array.
         */
        const T* data() const noexcept
        {
            return data_;
        }

        /**
         * @brief Accessor with out-of-bound check.
         * @details Gets the `pos` -th element in the Array, equipted
         *          with out-of-bound check.
         * @tparam T The template type of Array.
         * @return Reference to the `pos` -th element. The reference will be
         *         invalidated if the element is removed or by any 
         *         operations that reallocates the Array.
         * @exception `std::out_of_range` If `pos` is out-of-bound.
         */
        T& at(std::size_t pos);

        /**
         * @brief Accessor with out-of-bound check, read-only.
         * @details Gets the `pos` -th element in the Array, equipted
         *          with out-of-bound check.
         * @tparam T The template type of Array.
         * @return Const reference to the `pos` -th element. The reference will
         *         be invalidated if the element is removed or by any operations
         *         that reallocates the Array.
         * @exception `std::out_of_range` If `pos` is out-of-bound.
         */
        const T& at(std::size_t pos) const;

        /**
         * @brief Gets the first element; Array must not be empty.
         * @return Reference to the first element. The reference will be
         *         invalidated if the first element is removed or by any 
         *         operations that reallocates the Array.
         * @pre The Array must not be empty. Violating this precondition
         *      triggers an assertion in debug builds and is UB in release
         *      builds.
         */
        T& front();

        /**
         * @brief Gets the first element, read-only; Array must not be empty.
         * @return Const reference to the first element. The reference will
         *         be invalidated if the first element is removed or by any
         *         operations reallocates the Array.
         * @pre The Array must not be empty. Violating this precondition
         *      triggers an assertion in debug builds and is UB in release
         *      builds.
         */
        const T& front() const;

        /**
         * @brief Gets the first element; Array must not be empty.
         * @return Reference to the last element. The reference will be
         *         invalidated if the last element is removed or by any 
         *         operations that reallocates the Array.
         * @pre The Array must not be empty. Violating this precondition
         *      triggers an assertion in debug builds and is UB in release
         *      builds.
         */
        T& back();

        /**
         * @brief Gets the last element, read-only; Array must not be empty.
         * @return Const reference to the last element. The reference will
         *         be invalidated if the last element is removed or by any
         *         operations that reallocates the Array.
         * @pre The Array must not be empty. Violating this precondition
         *      triggers an assertion in debug builds and is UB in release
         *      builds.
         */
        const T& back() const;

        /** @} */   // name Array Getter

        /**
         * @name Array Iterator
         * @{
         */

        using iterator = T*;    /**< iterator type encapsulation  */
        using const_iterator = const T*;    /**< constant iterator type encapsulation*/

        /**
         * @brief Gets the head iterator.
         * @return Iterator to the first element. The iterator will be
         *         invalidated if the first element is removed or by any 
         *         operations that reallocates the Array.
         * @note Only using `cbegin()` explicitly can one assign a
         *       `const_iterator `to the non-const object, otherwise `begin()`
         *       will be matched.
         */
        iterator begin() noexcept
        {
            return data_;
        }

        /**
         * @brief Gets the after-tail iterator.
         * @return Iterator to the last element. The iterator will be
         *         invalidated if the last element is removed or by any 
         *         operations that reallocates the Array.
         * @note Only using `cend()` explicitly can one assign a
         *       `const_iterator `to the non-const object, otherwise `end()`
         *       will be matched.
         */
        iterator end() noexcept
        {
            return data_ ? data_ + length_ : data_;
        }

        /**
         * @brief Gets the head const iterator.
         * @return Const iterator to the first element. The iterator will be
         *         invalidated if the first element is removed or by any 
         *         operations that reallocates the Array.
         * @note Only using `cbegin()` explicitly can one assign a
         *       `const_iterator `to the non-const object, otherwise `begin()`
         *       will be matched.
         */
        const_iterator begin() const noexcept
        {
            return data_;
        }

        /**
         * @brief Gets the after-tail const iterator.
         * @return Const iterator to the last element. The iterator will be
         *         invalidated if the last element is removed or by any 
         *         operations that reallocates the Array.
         * @note Only using `cend()` explicitly can one assign a
         *       `const_iterator `to the non-const object, otherwise `end()`
         *       will be matched.
         */
        const_iterator end() const noexcept
        {
            return data_ ? data_ + length_ : data_;
        }

        /**
         * @brief Gets the head const iterator.
         * @return Const iterator to the first element. The iterator will be
         *         invalidated if the first element is removed or by any 
         *         operations that reallocates the Array.
         * @note Only using `cbegin()` explicitly can one assign a
         *       `const_iterator `to the non-const object, otherwise `begin()`
         *       will be matched.
         */
        const_iterator cbegin() const noexcept
        {
            return data_;
        }

        /**
         * @brief Gets the after-tail const iterator.
         * @return Const iterator to the last element. The iterator will be
         *         invalidated if the last element is removed or by any 
         *         operations that reallocates the Array.
         * @note Only using `cend()` explicitly can one assign a
         *       `const_iterator `to the non-const object, otherwise `end()`
         *       will be matched.
         */
        const_iterator cend() const noexcept
        {
            return data_ ? data_ + length_ : data_;
        }

        /** @} */   // name Array Iterator

        /**
         * @name Array Capacity
         * @{
         */

        std::size_t length() const noexcept
        {
            return length_;
        }
        std::size_t capacity() const noexcept
        {
            return capacity_;
        }
        bool empty() const noexcept
        {
            return length_ == 0;
        }

        /**
         * @brief Reallocate the Array to capacity `new_cap`.
         * @details Reallocate the Array to a new block of buffer if
         *          `new_cap` is greater than `capacity_`;
         *          do nothing if `new_cap` is less than `capacity_`.
         * @tparam T The template type parameter.
         * @exception `std::bad_alloc` If allocation fails;
         * 
         *  Other guarantees:
         * 
         *  - Any exceptions thrown by `T(T&&)` will propagate.
         *  
         *  - The allocated buffer will be released if exceptions are thrown
         *  (strong exception guarantee).
         * @post `capacity_` will be modified if succeeds.
         * @warning Iterators on this Array (including `pos`) will be invalidated
         *          if the Array is reallocated.
         */
        void reserve(std::size_t new_cap);

        /**
         * @brief Shrink `capacity` to `length`.
         * @tparam T The template type parameter.
         * @exception `std::bad_alloc` If allocation fails;
         * 
         *  Other guarantees:
         * 
         *  - Any exceptions thrown by `T(T&&)` will propagate.
         *  
         *  - The allocated buffer will be released if exceptions are thrown
         *  (strong exception guarantee).
         * @post `capacity_` will be modified if succeeds.
         */
        void shrink_to_fit();

        /** @} */   // name Arrat Capacity

        /**
         * @name Array Mutator
         * @{
         */

        /**
         * @brief Adds an element in the last position.
         * @details Add an element; reserve the `capacity_` to twice if full;
         *          O(1) on average.
         * @tparam T The template type parameter.
         * @exception `std::bad_alloc` If allocation fails;
         * 
         *  Other guarantees:
         * 
         *  - Any exceptions thrown by `T(T&&)` will propagate.
         *  
         *  - The allocated buffer will be released if exceptions are thrown
         *  (strong exception guarantee).
         * @post - `length_` plus 1 if succeed.
         * @post - `capacity_` will be modified if succeeds.
         * @warning Iterators on this Array (including `pos`) will be invalidated
         *          if the Array is reallocated.
         */
        void push_back(const T& value);

        /**
         * @brief Adds an element in the last position.
         * @details Add an element; reserve the `capacity_` to twice if full;
         *          O(1) on average.
         * @tparam T The template type parameter.
         * @exception `std::bad_alloc` If allocation fails;
         * 
         *  Other guarantees:
         * 
         *  - Any exceptions thrown by `T(T&&)` will propagate.
         *  
         *  - The allocated buffer will be released if exceptions are thrown
         *  (strong exception guarantee).
         * @post - `length_` plus 1 if succeed.
         * @post - `capacity_` will be modified if succeeds.
         * @warning Iterators on this Array (including `pos`) will be invalidated
         *          if the Array is reallocated.
         */
        void push_back(T&& value);

        /**
         * @brief Deletes an element in the last position; Array must not
         *        be empty.
         * @details Delete an element; do nothing on `capacity_`; O(1) on
         *          average.
         * @pre The Array must not be empty. Violating this precondition
         *      triggers an assertion in debug builds and is UB in release
         *      builds.
         * @post `length_` minus 1 if succeed.
         * @warning Iterators pointing at modified elements will
         *          be invalidated or will not act as expected.
         */
        void pop_back();

        /**
         * @brief Change the `length` to `new_length` and filling new 
         *        buffers with default constructor.
         * @details Reallocate with default-constructed object or deallocate,
         *          depending on `capacity_`.
         * @tparam T The template type parameter.
         * @exception `std::bad_alloc` If allocation fails;
         * 
         *  Other guarantees:
         * 
         *  - Any exceptions thrown by `T()` or `T(T&&)` will propagate.
         * 
         *  - The allocated buffer will be released if exceptions are thrown
         *  (strong exception guarantee).
         * @post - `length_` will be modified if succeeds.
         * @post - `capacity_` will be modified if succeeds.
         * @warning Iterators on this Array (including `pos`) will be invalidated
         *          if the Array is reallocated.
         */
        void resize(std::size_t new_length);

        /**
         * @brief Change the `length` to `new_length` and filling new 
         *        buffers with `init`.
         * @details Reallocate with default-constructed object or deallocate,
         *          depending on `capacity_`.
         * @tparam T The template type parameter.
         * @exception `std::bad_alloc` If allocation fails;
         * 
         * Other guarantees:
         * 
         *  - Any exceptions thrown by `T(const T&)` or `T(T&&)` will
         *    propagate.
         * 
         *  - The allocated buffer will be released if exceptions are thrown
         *  (strong exception guarantee).
         * @post - `length_` will be modified if succeeds.
         * @post - `capacity_` will be modified if succeeds.
         * @warning Iterators on this Array (including `pos`) will be invalidated
         *          if the Array is reallocated.
         */
        void resize(std::size_t new_length, const T& init);

        /**
         * @brief Set to an empty Array.
         * @post - `length_` will be set to 0.
         * @post - `capacity_` will be set to 0.
         * @warning Iterators of the caller will be invalidated.
         */
        void clear() noexcept;

        /**
         * @brief Insert `value` at position `pos`; `pos` must point to a
         *        constructed buffer.
         * @details Insert `value` at position `pos`, move the latter elements
         *          one by one. Reallocate if needed. O(n) on average, where n
         *          is the length of the Array.
         * @return Returns an `iterator` pointing at the newly inserted element.
         *         The iterator will be
         *         invalidated if the element is removed or by any
         *         operations that reallocates the Array.
         * @tparam T The template type parameter.
         * @exception `std::bad_alloc` If allocation fails;
         * 
         *  Other guarantees:
         * 
         *  - Any exceptions thrown by `T(const T&)` or `T(T&&)` will
         *    propagate.
         * 
         *  - The allocated buffer will be released if exceptions are thrown
         *  (strong exception guarantee).
         * @pre Iterator `pos` must be a valid iterator pointing to a CONSTRUCTED
         *      buffer. Violating this precondition triggers an assertion in debug
         *      builds and is UB in release builds.
         * @post - `length_` will plus 1 if succeeds.
         * @post - `capacity_` will be modified if succeeds.
         * @warning Iterators on this Array (including `pos`) will be invalidated
         *          if the Array is reallocated.
         */
        iterator insert(iterator pos, const T& value);

        /**
         * @brief Delete the value at `pos`; Array must not be empty; `pos` must
         *        point to a constructed buffer.
         * @details Delete the value at `pos`, move the latter elements one by one.
         *          O(n) on average.
         * @return Returns an `iterator` pointing at the deleted position.
         *         The iterator will be
         *         invalidated if the element is removed or by any
         *         operations that reallocates the Array.
         * @pre The Array must not be empty. Violating this precondition triggers
         *      an assertion in debug builds and is UB in release builds.
         * @pre Iterator `pos` must be a valid iterator pointing to a CONSTRUCTED
         *      buffer. Violating this precondition triggers an assertion in debug
         *      builds and is UB in release builds.
         * @post - `length_` will plus 1 if succeeds.
         * @warning Iterators pointing at modified elements (including `pos`) will
         *          be invalidated or will not act as expected.
         */
        iterator erase(iterator pos);

        /**
         * @brief Delete the value in `[start_it, end_it)`; Array must not be empty;
         *        `start_it` and `end_it` must point to constructed buffer.
         * @details Delete the value at in `[start_it, end_it)`, move the latter
         *          elements one by one. O(n) on average.
         * @return Returns an `iterator` pointing at the first deleted position.
         *         The iterator will be
         *         invalidated if the element is removed or by any
         *         operations that reallocates the Array.
         * @pre The Array must not be empty. Violating this precondition triggers
         *      an assertion in debug builds and is UB in release builds.
         * @pre Iterator `start_it` and `end_it` must be a valid iterator pointing
         *      to a CONSTRUCTED buffer. Violating this precondition triggers an
         *      assertion in debug builds and is UB in release builds.
         * @post - `length_` will plus 1 if succeeds.
         * @warning Iterators pointing at modified elements (including `start_it`
         *          and `end_it`) will be invalidated or will not act as expected.
         */
        iterator erase(iterator start_it, iterator end_it);

        /** @} */   // name Array Mutator

        /**
         * @name Array Swap
         * @{
         */

        /**
         * @brief Swap with another Array.
         * @details Swaps head pointer `data_`, `length_` and `capacity`. O(1).
         */
        void swap(Array& other) noexcept;

        /**
         * @brief Swap Arrays.
         * @details Swaps head pointer `data_`, `length_` and `capacity`. O(1).
         *          Calls the member method swap(), defined in BST.tpp.
         */
        friend void swap(Array<T>& a, Array<T>& b) noexcept
        {
            a.swap(b);
        }

        /** @} */   // name Array Swap
        
    private:
        std::size_t length_;    /**< numbers of constructed T */
        std::size_t capacity_;  /**< numbers of allocated blocks of T */
        T* data_;       /**< head pointer to the array */
    };  // template class Array

    /**
     * @name Array Operator
     * @{
     */

    /**
     * @brief Judges whether two Arrays has the same elements
     *        in the same order; DO NOT judges `capacity_`.
     * @tparam T The type of Array tempalte.
     * @relates data_struct::Array.
     */
    template <typename T>
    bool operator==(const Array<T>&, const Array<T>&);

    /**
     * @brief Judges whether two Arrays has different elements
     *        or has the same elements but not in the same order;
     *        DO NOT judges `capacity_`.
     * @tparam T The type of Array tempalte.
     * @relates data_struct::Array
     */
    template <typename T>
    bool operator!=(const Array<T>& lhs, const Array<T>& rhs)
    {
        return !(lhs == rhs);
    }

    /** @} */   // name Array Operator
}   // namespace data_struct

#include "Array.tpp"

// extern template class data_struct::Array<int>;
// extern template class data_struct::Array<float>;
// extern template class data_struct::Array<double>;
// extern template class data_struct::Array<std::string>;