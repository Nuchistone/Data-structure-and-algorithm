#pragma once
#include <memory>
#include <utility>
#include <cassert>
#include <stdexcept>

namespace data_struct
{
    // constructor, copy controller
    template <typename T>
    Array<T>::Array()
        : length_(0)
        , capacity_(0)
        , data_(nullptr)
    {
    }

    template <typename T>
    Array<T>::Array(std::size_t length)
        : length_(0)
        , capacity_(length)
        , data_(static_cast<T*>(::operator new(length * sizeof(T))))
    {
        std::size_t i = 0;
        try
        {
            for ( ; i < length; ++i)
            {
                new (&data_[i]) T();
            }
            length_ = length;
        }
        catch (...)
        {
            for (std::size_t j = 0; j < i; ++j)
            {
                data_[j].~T();
            }
            ::operator delete(data_);
            throw;
        }
    }

    template <typename T>
    Array<T>::Array(std::initializer_list<T> init)
        : length_(0)
        , capacity_(init.size())
        , data_(static_cast<T*>(::operator new(init.size() * sizeof(T))))
    {
        std::size_t i = 0;
        std::size_t init_length = init.size();
        try
        {
            for (const T& item: init)
            {
                new (&data_[i++]) T(item);
            }
            length_ = init_length;
        }
        catch (...)
        {
            for (std::size_t j = 0; j < i; ++j)
            {
                data_[j].~T();
            }
            ::operator delete(data_);
            throw;
        }
    }

    template <typename T>
    Array<T>::Array(std::size_t length, const T& init)
        : length_(0)
        , capacity_(length)
        , data_(static_cast<T*>(::operator new(length * sizeof(T))))
    {
        std::size_t i = 0;
        try
        {
            for ( ; i < length; ++i)
            {
                new (&data_[i]) T(init);
            }
            length_ = length;
        }
        catch (...)
        {
            for (std::size_t j = 0; j < i; ++j)
            {
                data_[j].~T();
            }
            ::operator delete(data_);
            throw;
        }
    }

    template <typename T>
    Array<T>::Array(const Array<T>& init)
        : length_(0)
        , capacity_(init.length_)
        , data_(
            init.length_
            ? static_cast<T*>(::operator new(init.length_ * sizeof(T)))
            : nullptr
        )
    {
        std::size_t i = 0;
        try
        {
            for ( ; i < init.length_; ++i)
            {
                new (&data_[i]) T(init.data_[i]);
            }
            length_ = init.length_;
        }
        catch (...)
        {
            for (std::size_t j = 0; j < i; ++j)
            {
                data_[j].~T();
            }
            ::operator delete(data_);
            throw;
        }
    }

    template <typename T>
    Array<T>::Array(Array&& init) noexcept
        : length_(init.length_)
        , capacity_(init.capacity_)
        , data_(init.data_)
    {
        init.length_ = 0;
        init.capacity_ = 0;
        init.data_ = nullptr;
    }

    template <typename T>
    Array<T>::~Array()
    {
        for (std::size_t i = 0; i < length_; ++i)
        {
            data_[i].~T();
        }
        ::operator delete(data_);
    }

    template <typename T>
    void Array<T>::swap(Array& other) noexcept
    {
        std::swap(length_, other.length_);
        std::swap(capacity_, other.capacity_);
        std::swap(data_, other.data_);
    }

    template <typename T>
    Array<T>& Array<T>::operator=(const Array<T>& init)
    {
        if (this != &init)
        {
            Array<T> temp(init);
            swap(temp);
        }
        return *this;
    }

    template <typename T>
    Array<T>& Array<T>::operator=(Array<T>&& init) noexcept
    {
        if (this != &init)
        {
            for (std::size_t i = 0; i < length_; ++i)
            {
                data_[i].~T();
            }
            ::operator delete(data_);

            length_ = init.length_;
            capacity_ = init.capacity_;
            data_ = init.data_;

            init.length_ = 0;
            init.capacity_ = 0;
            init.data_ = nullptr;
        }
        return *this;
    }

    //getter
    template <typename T>
    T& Array<T>::operator[](std::size_t pos)
    {
        return data_[pos];
    }

    template <typename T>
    const T& Array<T>::operator[](std::size_t pos) const
    {
        return data_[pos];
    }

    template <typename T>
    T& Array<T>::at(std::size_t pos)
    {
        if (pos >= length_)
        {
            throw std::out_of_range("Array::at");
        }
        return data_[pos];
    }

    template <typename T>
    const T& Array<T>::at(std::size_t pos) const 
    {
        if (pos >= length_)
        {
            throw std::out_of_range("Array::at");
        }
        return data_[pos];
    }

    template <typename T>
    T& Array<T>::front()
    {
        assert(length_ > 0 && "data_struct::Array::front(): front on empty Array");
        return data_[0];
    }

    template <typename T>
    const T& Array<T>::front() const 
    {
        assert(length_ > 0 && "data_struct::Array::front(): front on empty Array"); 
        return data_[0];
    }

    template <typename T>
    T& Array<T>::back()
    {
        assert(length_ > 0 && "data_struct::Array::end(): end on empty Array");
        return data_[length_ - 1];
    }

    template <typename T>
    const T& Array<T>::back() const 
    {
        assert(length_ > 0 && "data_struct::Array::end(): end on empty Array");
        return data_[length_ - 1];
    }

    //capacity
    template <typename T>
    void Array<T>::reserve(std::size_t new_cap)
    {
        // static_assert(
        // std::is_nothrow_move_constructible_v<T> || std::is_copy_constructible_v<T>,
        //     "data_struct::Array::reserve: reserve requires T to be nothrow-movable or copyable for strong exception safety"
        // );

        if (capacity_ >= new_cap)
        {
            return;
        }

        T* new_data = static_cast<T*>(::operator new(new_cap * sizeof(T)));
        std::size_t i = 0;
        try
        {
            for ( ; i < length_; ++i)
            {
                new (&new_data[i]) T(std::move_if_noexcept(data_[i]));
            }
        }
        catch(...)
        {
            for (std::size_t j = 0; j < i; ++j)
            {
                new_data[j].~T();
            }
            ::operator delete(new_data);
            throw;
        }

        for (i = 0; i < length_; ++i)
        {
            data_[i].~T();
        }
        ::operator delete(data_);

        data_ = new_data;
        capacity_ = new_cap;
    }

    template <typename T>
    void Array<T>::shrink_to_fit()
    {
        if (capacity_ == length_)
        {
            return;
        }

        if (length_ == 0)
        {
            ::operator delete(data_);
            data_ = nullptr;
            capacity_ = 0;
            return;
        }

        T* new_data = static_cast<T*>(::operator new(length_ * sizeof(T)));
        std::size_t i = 0;
        try
        {
            for ( ; i < length_; ++i)
            {
                new (&new_data[i]) T(std::move_if_noexcept(data_[i]));
            }
        }
        catch(...)
        {
            for (std::size_t j = 0; j < i; ++j)
            {
                new_data[j].~T();
            }
            ::operator delete(new_data);
            throw;
        }

        for (i = 0; i < length_; ++i)
        {
            data_[i].~T();
        }
        ::operator delete(data_);

        data_ = new_data;
        capacity_ = length_;
    }

    template <typename T>
    void Array<T>::clear() noexcept
    {
        for (std::size_t i = 0; i < length_; ++i)
        {
            data_[i].~T();
        }

        length_ = 0;
    }

    template <typename T>
    void Array<T>::push_back(const T& value)
    {
        if (length_ == capacity_)
        {
            reserve(capacity_ ? capacity_ * 2 : 1);
        }

        new (&data_[length_++]) T(value);
    }

    template <typename T>
    void Array<T>::push_back(T&& value)
    {
        if (length_ == capacity_)
        {
            reserve(capacity_ ? capacity_ * 2 : 1);
        }

        new (&data_[length_++]) T(std::move(value));
    }

    template <typename T>
    void Array<T>::pop_back()
    {
        assert(length_ > 0 && "data_struct::Array::pop_back(): pop_back on empty array");
        data_[--length_].~T();
    }

    template <typename T>
    void Array<T>::resize(std::size_t new_length)
    {
        if (new_length > capacity_)
        {
            reserve(new_length);
        }

        if (length_ < new_length)
        {
            std::size_t i = length_;
            try
            {
                for ( ; i < new_length; ++i)
                {
                    new (&data_[i]) T();
                }
            }
            catch(...)
            {
                for (std::size_t j = length_; j < i; ++j)
                {
                    data_[j].~T();
                }
                throw;
            }
        }
        else
        {
            for (std::size_t i = new_length; i < length_; ++i)
            {
                data_[i].~T();
            }
        }

        length_ = new_length;
    }

    template <typename T>
    void Array<T>::resize(std::size_t new_length, const T& init)
    {
        if (new_length > capacity_)
        {
            reserve(new_length);
        }

        if (length_ < new_length)
        {
            std::size_t i = length_;
            try
            {
                for ( ; i < new_length; ++i)
                {
                    new (&data_[i]) T(init);
                }
            }
            catch(...)
            {
                for (std::size_t j = length_; j < i; ++j)
                {
                    data_[j].~T();
                }
                throw;
            }
        }
        else
        {
            for (std::size_t i = new_length; i < length_; ++i)
            {
                data_[i].~T();
            }
        }

        length_ = new_length;
    }

    template <typename T>
    typename Array<T>::iterator
        Array<T>::insert(Array<T>::iterator pos, const T& value)
    {
        assert(pos >= data_ && pos <= data_ + length_ && "data_struct::Array::insert: invalid iterator");

        std::size_t index = pos - data_;

        if (length_ == capacity_)
        {
            reserve(capacity_ ? capacity_ * 2 : 1);
        }

        if (index == length_)
        {
            new (&data_[length_]) T(value);
        }
        else
        {
            new (&data_[length_]) T(std::move(data_[length_ - 1]));
            for (std::size_t i = length_ - 1; i > index; --i)
            {
                data_[i] = std::move(data_[i - 1]);
            }
            data_[index] = value;
        }
        ++length_;
        return data_ + index;
    }

    template <typename T>
    typename Array<T>::iterator
        Array<T>::erase(Array<T>::iterator pos)
    {
        assert(length_ > 0 && "data_struct::Array::erase: erase on empty Array");
        assert(pos >= data_ && pos < data_ + length_ && "data_struct::Array::erase: invalid iterator");

        std::size_t index = pos - data_;

        for (std::size_t i = index; i < length_ - 1; ++i)
        {
            data_[i] = std::move(data_[i + 1]);
        }
        data_[--length_].~T();
        return data_ + index;
    }

    template <typename T>
    typename Array<T>::iterator
        Array<T>::erase(Array<T>::iterator start_it, Array<T>::iterator end_it)
    {
        assert(start_it >= data_ && start_it <= data_ + length_ && "data_struct::Array::erase: invalid start iterator");
        assert(end_it >= data_ && end_it <= data_ + length_ && "data_struct::Array::erase: invalid end iterator");
        assert(start_it <= end_it && "data_struct::Array::erase: start > end");

        std::size_t start_index = start_it - data_;
        std::size_t end_index = end_it - data_;
        std::size_t count = end_index - start_index;

        if (count == 0)
        {
            return start_it;
        }

        for (std::size_t i = start_index; i < length_ - count; ++i)
        {
            data_[i] = std::move(data_[i + count]);
        }

        for (std::size_t i = length_ - count; i < length_; ++i)
        {
            data_[i].~T();
        }

        length_ -= count;
        return data_ + start_index;
    }
}