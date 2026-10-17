#pragma once
#include <cstddef>
#include <initializer_list>
#include <type_traits>
#include <string>

namespace data_struct
{
    template <typename T>
    class List
    {
    private:
        struct Node
        {
            T data_;
            Node* prev_;
            Node* next_;

            // constructor, copy controller
            Node();
            explicit Node(const T& data);
            explicit Node(T&& data) noexcept(std::is_nothrow_move_constructible_v<T>);

            Node(const Node&) = delete;
            Node(Node&&) = delete;
            Node& operator=(const Node&) = delete;
            Node& operator=(Node&&) = delete;
            ~Node() = default;
        };

    public:
        // constructor, copy controller
        List();
        List(std::initializer_list<T> init);
        List(std::size_t size, const T& init);

        List(const List& init);
        List(List&& init) noexcept;
        ~List();
        List& operator=(const List& init);
        List& operator=(List&& init) noexcept;

        // getter
        T& front();
        const T& front() const;
        T& back();
        const T& back() const;

        // iterator
        class const_iterator;
        class iterator
        {
        friend class List;
        friend class const_iterator;

        public:
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

        private:
            Node* node_;

            explicit iterator(Node* node)
                : node_(node)
            {
            }
        };  // class iterator

        class const_iterator
        {
        friend class List;
        public:
            const_iterator(const iterator& it)  // implicit conversion
                : node_(it.node_)
            {
            }
            
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

        private:
            const Node* node_;

            explicit const_iterator(const Node* node)
                : node_(node)
            {
            }
        };  // class const_iterator

        iterator begin() noexcept
        {
            return iterator(sentinel_.next_);
        }
        iterator end() noexcept
        {
            return iterator(&sentinel_);
        }
        const_iterator begin() const noexcept
        {
            return const_iterator(sentinel_.next_);
        }
        const_iterator end() const noexcept
        {
            return const_iterator(&sentinel_);
        }
        const_iterator cbegin() const noexcept
        {
            return const_iterator(sentinel_.next_);
        }
        const_iterator cend() const noexcept
        {
            return const_iterator(sentinel_);
        }

        // capacity
        std::size_t size() const noexcept
        {
            return size_;
        }
        bool empty() const noexcept
        {
            return (sentinel_.next_ == &sentinel_);
        }

        // mutator
        void push_front(const T& value);
        void push_front(T&& value);
        void push_back(const T& value);
        void push_back(T&& value);
        void pop_front();
        void pop_back();
        void clear();
        void splice(iterator pos, List& other) noexcept;
        void splice(iterator pos, List& other, iterator it) noexcept;
        void splice(iterator pos, List& other, iterator begin_it, iterator end_it) noexcept;
        iterator insert(iterator pos, const T& value);  // insert before pos
        iterator insert(iterator pos, T&& value);
        iterator erase(iterator pos);
        iterator erase(iterator start_it, iterator end_it);

        // algorithm
        iterator find(const T& value);
        const_iterator find(const T& value) const;
        void remove(const T& value);
        // template <typename Predicate>
        // void remove_if(Predicate pred);
        // void sort();
        // template <typename Compare>
        // void sort(Compare comp);

        // swap
        void swap(List& other) noexcept;

    private:
        Node sentinel_;
        std::size_t size_;

        // transfer [first, last) to the front of pos
        void transfer(iterator pos, iterator first, iterator last);
    };  // template class List

    template <typename T>
    bool operator==(const List<T>&, const List<T>&);

    template <typename T>
    void swap(List<T>& a, List<T>& b) noexcept
    {
        a.swap(b);
    }
}   // namespace data_struct

#include "List.tpp"

// extern template class data_struct::List<int>;
// extern template class data_struct::List<float>;
// extern template class data_struct::List<double>;
// extern template class data_struct::List<std::string>;