#pragma once
#include <cstddef>
#include <initializer_list>
#include <algorithm>
#include <string>

namespace data_struct
{
    template <typename T>
    class Array 
    {
    public:
        // constructor, copy controller
        Array();
        explicit Array(std::size_t length);
        Array(std::initializer_list<T> init);
        Array(std::size_t length, const T& init);
        
        Array(const Array& init);
        Array(Array&& init) noexcept;
        ~Array();
        Array& operator=(const Array& init);
        Array& operator=(Array&& init) noexcept;

        // getter
        T& operator[](std::size_t pos);     // "pos" for position
        const T& operator[](std::size_t pos) const;

        T* data() noexcept
        {
            return data_;
        }
        const T* data() const noexcept
        {
            return data_;
        }
        
        T& at(std::size_t pos);
        const T& at(std::size_t pos) const;

        T& front();
        const T& front() const;
        T& back();
        const T& back() const;

        // iterator
        using iterator = T*;
        using const_iterator = const T*;

        iterator begin() noexcept
        {
            return data_;
        }
        iterator end() noexcept
        {
            return data_ ? data_ + length_ : data_;
        }
        const_iterator begin() const noexcept
        {
            return data_;
        }
        const_iterator end() const noexcept
        {
            return data_ ? data_ + length_ : data_;
        }
        const_iterator cbegin() const noexcept
        {
            return data_;
        }
        const_iterator cend() const noexcept
        {
            return data_ ? data_ + length_ : data_;
        }

        // capacity
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
        void reserve(std::size_t new_cap);
        void shrink_to_fit();

        // mutator
        void push_back(const T& value);
        void push_back(T&& value);
        void pop_back();
        void resize(std::size_t new_length);
        void resize(std::size_t new_length, const T& init);
        void clear() noexcept;
        iterator insert(iterator pos, const T& value);
        iterator erase(iterator pos);
        iterator erase(iterator start_it, iterator end_it);

        // swap
        void swap(Array& other) noexcept;
        
    private:
        std::size_t length_;
        std::size_t capacity_;
        T* data_;
    };  // template class Array

    template <typename T>
    void swap(Array<T>& a, Array<T>& b) noexcept
    {
        a.swap(b);
    }
}   // namespace data_struct

#include "Array.tpp"

// extern template class data_struct::Array<int>;
// extern template class data_struct::Array<float>;
// extern template class data_struct::Array<double>;
// extern template class data_struct::Array<std::string>;