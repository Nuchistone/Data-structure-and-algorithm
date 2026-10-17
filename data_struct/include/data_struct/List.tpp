#pragma once
#include "List.h"
#include <memory>
#include <utility>
#include <cassert>

namespace data_struct
{
    // Node
    template <typename T>
    List<T>::Node::Node()
        : data_()
        , prev_(nullptr)
        , next_(nullptr)
    {
    }

    template <typename T>
    List<T>::Node::Node(const T& data)
        : data_(data)
        , prev_(nullptr)
        , next_(nullptr)
    {
    }

    template <typename T>
    List<T>::Node::Node(T&& data) noexcept(std::is_nothrow_move_constructible_v<T>)
        : data_(std::move(data))
        , prev_(nullptr)
        , next_(nullptr)
    {
    }

    // List
    // constructor, copy controller
    template <typename T>
    List<T>::List()
        : sentinel_()
        , size_(0)
    {
        sentinel_.next_ = sentinel_.prev_ = &sentinel_;
    }

    template <typename T>
    List<T>::List(std::initializer_list<T> init)
        : List()
    {
        for (const T& item : init)
        {
            push_back(item);
        }
    }

    template <typename T>
    List<T>::List(std::size_t size, const T& init)
        : List()
    {
        for (std::size_t i = 0; i < size; ++i)
        {
            push_back(init);
        }
    }

    template <typename T>
    List<T>::List(const List& init)
        : List()
    {
        for (const T& item : init)
        {
            push_back(item);
        }
    }

    template <typename T>
    List<T>::List(List&& init) noexcept
        : sentinel_()
        , size_(init.size())
    {
        if (init.empty())
        {
            sentinel_.next_ = sentinel_.prev_ = &sentinel_;
        }
        else
        {
            sentinel_.prev_ = init.sentinel_.prev_;
            sentinel_.next_ = init.sentinel_.next_;
            sentinel_.next_->prev_ = &sentinel_;
            sentinel_.prev_->next_ = &sentinel_;
        }

        init.sentinel_.next_ = init.sentinel_.prev_ = &init.sentinel_;
        init.size_ = 0;
    }

    template <typename T>
    List<T>::~List()
    {
        clear();
    }

    template <typename T>
    List<T>& List<T>::operator=(const List& init)
    {
        if (this != &init)
        {
            List<T> temp(init);
            swap(temp);
        }
        return *this;
    }

    template <typename T>
    List<T>& List<T>::operator=(List&& init) noexcept
    {
        if (this != &init)
        {
            List<T> temp(std::move(init));
            swap(temp);
        }
        return *this;
    }

    // getter
    template <typename T>
    T& List<T>::front()
    {
        assert(size_ > 0 && "data_struct::List::front(): front on empty List");
        
        return sentinel_.next_->data_;
    }

    template <typename T>
    const T& List<T>::front() const
    {
        assert(size_ > 0 && "data_struct::List::front(): front on empty List");
        
        return sentinel_.next_->data_;
    }

    template <typename T>
    T& List<T>::back()
    {
        assert(size_ > 0 && "data_struct::List::back(): back on empty List");
        
        return sentinel_.prev_->data_;
    }
    
    template <typename T>
    const T& List<T>::back() const
    {
        assert(size_ > 0 && "data_struct::List::back(): back on empty List");
        
        return sentinel_.prev_->data_;
    }

    // mutator
    template <typename T>
    void List<T>::push_front(const T& value)
    {
        insert(begin(), value);
    }

    template <typename T>
    void List<T>::push_front(T&& value)
    {
        insert(begin(), std::move(value));
    }

    template <typename T>
    void List<T>::push_back(const T& value)
    {
        insert(end(), value);
    }

    template <typename T>
    void List<T>::push_back(T&& value)
    {
        insert(end(), std::move(value));
    }

    template <typename T>
    void List<T>::pop_front()
    {
        assert(size_ > 0 && "data_struct::List::pop_front(): pop on empty List");
        erase(begin());
    }

    template <typename T>
    void List<T>::pop_back()
    {
        assert(size_ > 0 && "data_struct::List::pop_back(): pop on empty List");
        erase(--end());
    }

    template <typename T>
    void List<T>::clear()
    {
        Node* current = sentinel_.next_;
        while(current != &sentinel_)
        {
            Node* next = current->next_;
            delete current;
            current = next;
        }
        sentinel_.next_ = sentinel_.prev_ = &sentinel_;

        size_ = 0;
    }

    template <typename T>
    void List<T>::splice(typename List<T>::iterator pos, List& other) noexcept
    {
        if (this == &other || other.empty())
        {
            return;
        }

        transfer(pos, other.begin(), other.end());

        size_ += other.size_;
        other.size_ = 0;
    }

    template <typename T>
    void List<T>::splice(
        typename List<T>::iterator pos,
        List& other,
        List<T>::iterator it
    ) noexcept
    {
        iterator next_it = it;
        ++next_it;

        if (other.empty()
            || it == pos
            || next_it == pos)
        {
            return;
        }

        transfer(pos, it, next_it);

        ++size_;
        --other.size_;
    }

    template <typename T>
    void List<T>::splice(
        typename List<T>::iterator pos,
        List& other,
        List<T>::iterator begin_it,
        List<T>::iterator end_it
    ) noexcept
    {
        if (begin_it == end_it || other.empty() || end_it == pos)
        {
            return;
        }

        std::size_t count = 0;
        for (auto it = begin_it; it != end_it; ++it)
        {
            ++count;
        }

        transfer(pos, begin_it, end_it);

        size_ += count;
        other.size_ -= count;
    }

    template <typename T>
    typename List<T>::iterator
        List<T>::insert(List<T>::iterator pos, const T& value)
    {
        Node* newNode = new Node(value);
        Node* current = pos.node_;

        newNode->next_ = current;
        newNode->prev_ = current->prev_;
        current->prev_->next_ = newNode;
        current->prev_ = newNode;

        ++size_;
        
        return iterator(newNode);
    }

    template <typename T>
    typename List<T>::iterator
        List<T>::insert(List<T>::iterator pos, T&& value)
    {
        Node* newNode = new Node(std::move(value));
        Node* current = pos.node_;

        newNode->next_ = current;
        newNode->prev_ = current->prev_;
        current->prev_->next_ = newNode;
        current->prev_ = newNode;

        ++size_;
        
        return iterator(newNode);
    }

    template <typename T>
    typename List<T>::iterator
        List<T>::erase(List<T>::iterator pos)
    {
        assert(pos != end() && "data_struct::List::erase: erase end()");
        
        Node* current = pos.node_;
        current->prev_->next_ = current->next_;
        current->next_->prev_ = current->prev_;

        Node* next = current->next_;
        delete current;
        --size_;
        return iterator(next);
    }

    template <typename T>
    typename List<T>::iterator
        List<T>::erase(List<T>::iterator start_it, List<T>::iterator end_it)
    {
        if (start_it == end_it)
        {
            return end_it;
        }

        Node* startNode = start_it.node_;
        Node* endNode = end_it.node_;
        startNode->prev_->next_ = endNode;
        endNode->prev_ = startNode->prev_;

        Node* current = startNode;
        while (current != endNode)
        {
            Node* next = current->next_;
            delete current;
            current = next;
            --size_;
        }

        return end_it;
    }

    // algorithm
    template <typename T>
    typename List<T>::iterator
        List<T>::find(const T& value)
    {
        iterator it = begin();
        for ( ; it != end(); ++it)
        {
            if (*it == value)
            {
                break;
            }
        }
        return it;
    }

    template <typename T>
    typename List<T>::const_iterator
        List<T>::find(const T& value) const
    {
        const_iterator it = cbegin();
        for ( ; it != cend(); ++it)
        {
            if (*it == value)
            {
                break;
            }
        }
        return it;
    }

    template <typename T>
    void List<T>::remove(const T& value)
    {
        auto it = begin();
        while (it != end())
        {
            if (*it == value)
            {
                it = erase(it);
            }
            else
            {
                ++it;
            }
        }
    }

    // swap
    template <typename T>
    void List<T>::swap(List& other) noexcept
    {
        if (this == &other)
        {
            return;
        }

        std::swap(sentinel_.next_, other.sentinel_.next_);
        std::swap(sentinel_.prev_, other.sentinel_.prev_);
        std::swap(size_, other.size_);

        sentinel_.prev_->next_ = sentinel_.next_->prev_ = &sentinel_;
        other.sentinel_.prev_->next_ = other.sentinel_.next_->prev_ = &other.sentinel_;
    }

    // transfer
    template <typename T>
    void List<T>::transfer(
        typename List<T>::iterator pos,
        iterator first,
        iterator last
    )
    {
        if (first == last)
        {
            return;
        }

        Node* posNode = pos.node_;
        Node* firstNode = first.node_;
        Node* lastNode = last.node_;
        Node* lastPrev = last.node_->prev_;

        firstNode->prev_->next_ = lastNode;
        lastNode->prev_ = firstNode->prev_;

        firstNode->prev_ = posNode->prev_;
        lastPrev->next_ = posNode;
        posNode->prev_->next_ = firstNode;
        posNode->prev_ = lastPrev;
    }

    template <typename T>
    bool operator==(const List<T>& lhs, const List<T>& rhs)
    {
        if (lhs.size() != rhs.size())
        {
            return false;
        }

        auto it1 = lhs.cbegin();
        auto it2 = rhs.cbegin();
        while(it1 != lhs.end())
        {
            if (!(*it1++ == *it2++))
            {
                return false;
            }
        }

        return true;
    }
}