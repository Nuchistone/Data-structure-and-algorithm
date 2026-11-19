#pragma once
#include "BST.h"
#include "Queue.h"
#include <memory>
#include <utility>

namespace data_struct
{
    // ==============================
    // BSTNode
    // ==============================

    // Default constructor, default constructor of T
    // will be called explicitly.
    template <typename T>
    BST<T>::BSTNode::BSTNode()
        : data_()
        , left_(nullptr)
        , right_(nullptr)
    {
    }

    template <typename T>
    BST<T>::BSTNode::BSTNode(const T& data)
        : data_(data)
        , left_(nullptr)
        , right_(nullptr)
    {
    }

    template <typename T>
    BST<T>::BSTNode::BSTNode(T&& data) noexcept(std::is_nothrow_move_constructible_v<T>)
        : data_(std::move(data))
        , left_(nullptr)
        , right_(nullptr)
    {
    }

    // ==============================
    // implementation of recursion
    // ==============================

    // constructor, copy controller

    // Deep copies a subtree; is called by BST copy controller.
    template <typename T>
    typename BST<T>::BSTNode*
    BST<T>::clone_impl(BSTNode* current_subtree) const
    {
        // Clone the subtree recursively. Returns a BSTNode* to renew
        // the child node of the previous recursion caller -- a subtree.

        // Base case: leaf.
        if (current_subtree == nullptr)
        {
            return nullptr;
        }

        // Exceptions might be thrown when cloning left and right children,
        // so we need try-catch to guarantee that under such situation,
        // constructed new node are properly destructed, namely to
        // guarantee strong exceptional safety.
        BSTNode* cloned_subtree = new BSTNode(current_subtree->data_);
        try
        {
            cloned_subtree->left_ = clone_impl(current_subtree->left_);
            cloned_subtree->right_ = clone_impl(current_subtree->right_);
        }
        catch(...)
        {
            destroy_impl(cloned_subtree);
            throw;
        }

        return cloned_subtree;
    }

    // Destroy a subtree; is called when BST destruction is needed.
    template <typename T>
    void BST<T>::destroy_impl(typename BST<T>::BSTNode* current_subtree) const
    {
        // Destroy a subtree recursively.

        // Base case: leaf.
        if (current_subtree == nullptr)
        {
            return;
        }

        destroy_impl(current_subtree->left_);
        destroy_impl(current_subtree->right_);
        delete current_subtree;
    }

    // mutator

    // Use a bool [out] param to bring out the result, returns false if
    // inserting an existing node or exception are thrown before the
    // insertion succeeds.
    template <typename T>
    typename BST<T>::BSTNode*
    BST<T>::insert_impl(
        BSTNode* current_subtree,
        const T& value,
        bool& is_inserted
    )
    {
        // The insertion logic is based on the definition. If the value to
        // be inserted is less than the current node, goes left; greater,
        // goes right; equal, do not modify the tree, insertion fails;
        // reaching the child of leaves (not found), insert the node,
        // insertion succeeds.

        // Base case 1: reaching the child of leaves, insertion succeed.
        if (current_subtree == nullptr)
        {
            // unique_ptr will destruct the node if construction throws
            auto newNode = std::make_unique<BSTNode>(value);
            is_inserted = true;
            
            return newNode.release();
        }

        if (value < current_subtree->data_)
        {
            current_subtree->left_ = insert_impl(
                current_subtree->left_,
                value,
                is_inserted
            );
        }
        else if (value > current_subtree->data_)
        {
            current_subtree->right_ = insert_impl(
                current_subtree->right_,
                value,
                is_inserted
            );
        }
        else    // Base case 2: already in the tree, insertion fails.
        {
            is_inserted = false;
        }

        return current_subtree;
    }

    // Use a bool [out] param to bring out the result, returns
    // false if removing a node doesn't exist.
    template <typename T>
    typename BST<T>::BSTNode*
    BST<T>::remove_impl(
        BSTNode* current_subtree,
        const T& value,
        bool& is_removed
    )
    {
        // The deletion logic is based on the definition. If the value to
        // be deleted is less than the current node, goes left; greater,
        // goes right; equal, delete the node, deletion succeeds; reaching
        // the child of leaves (not found), do not modify the tree,
        // deletion fails.

        // Base case 1: reaching the child of leaves, deletion fails.
        if (current_subtree == nullptr)
        {
            is_removed = false;
            return nullptr;
        }

        if (value < current_subtree->data_)
        {
            current_subtree->left_ = remove_impl(
                current_subtree->left_,
                value,
                is_removed
            );
            return current_subtree;
        }
        else if (value > current_subtree->data_)
        {
            current_subtree->right_ = remove_impl(
                current_subtree->right_,
                value,
                is_removed
            );
            return current_subtree;
        }
        else    // Base case 2: founded, perform the deletion.
        {
            is_removed = true;

            // The deletion has to reconnect the children of the
            // the deleted node to the tree properly.

            // Including leaves.
            if (current_subtree->left_ == nullptr)
            {
                BSTNode* new_subtree = current_subtree->right_;
                delete current_subtree;
                return new_subtree;
            }
            else if (current_subtree->right_ == nullptr)
            {
                BSTNode* new_subtree = current_subtree->left_;
                delete current_subtree;
                return new_subtree;
            }
            else
            {
                // If both of the children is nontrivial, search in the
                // right child for the minimum. Substitute the value
                // with the minimum, then delete the minimum node. By
                // following this procedure, the modified tree is still
                // a BST.

                // The minimum must be a node without the left child,
                // so preform the procedure removing a subtree without
                // the left child.
                
                BSTNode* min_in_right = current_subtree->right_;
                BSTNode* min_parent = current_subtree;
                while (min_in_right->left_ != nullptr)
                {
                    min_parent = min_in_right;
                    min_in_right = min_in_right->left_;
                }

                current_subtree->data_ = min_in_right->data_;
                
                if (min_parent == current_subtree)
                {
                    min_parent->right_ = min_in_right->right_;
                }
                else
                {
                    min_parent->left_ = min_in_right->right_;
                }
                delete min_in_right;

                return current_subtree;
            }
        }
    }

    // traverse

    // Do the pre-order traversion on a subtree, using an
    // Array [out] param to bring out the result; is called
    // by BST pre_order method.
    template <typename T>
    void BST<T>::pre_order_impl(
        typename BST<T>::BSTNode* current_subtree,
        Array<T>& result
    ) const
    {
        if (current_subtree == nullptr)
        {
            return;
        }

        result.push_back(current_subtree->data_);
        pre_order_impl(current_subtree->left_, result);
        pre_order_impl(current_subtree->right_, result);
    }

    // Do the in-order traversion on a subtree, using an
    // Array [out] param to bring out the result; is called
    // by BST in_order method.
    template <typename T>
    void BST<T>::in_order_impl(
        typename BST<T>::BSTNode* current_subtree,
        Array<T>& result
    ) const
    {
        if (current_subtree == nullptr)
        {
            return;
        }

        in_order_impl(current_subtree->left_, result);
        result.push_back(current_subtree->data_);
        in_order_impl(current_subtree->right_, result);
    }

    // Do the post-order traversion on a subtree, using an
    // Array [out] param to bring out the result; is called
    // by BST post_order method.
    template <typename T>
    void BST<T>::post_order_impl(
        typename BST<T>::BSTNode* current_subtree,
        Array<T>& result
    ) const
    {
        if (current_subtree == nullptr)
        {
            return;
        }

        post_order_impl(current_subtree->left_, result);
        post_order_impl(current_subtree->right_, result);
        result.push_back(current_subtree->data_);
    }

    // ==============================
    // BST
    // ==============================

    // ==============================
    // constructor, copy controller
    // ==============================

    template <typename T>
    BST<T>::BST()
        : root_(nullptr)
        , size_(0)
    {
    }

    template <typename T>
    BST<T>::BST(std::initializer_list<T> init)
        : root_(nullptr)
        , size_(0)
    {
        for (const T& item : init)
        {
            insert(item);
        }
    }

    template <typename T>
    BST<T>::BST(const BST& init)
        : root_(nullptr)
        , size_(init.size_)
    {
        // Deep copies init.root_; see
        // BST<T>::clone_impl (declaration in BST.h,
        // definition in BST.tpp)
        root_ = clone_impl(init.root_);
    }

    template <typename T>
    BST<T>::BST(BST&& init) noexcept
        : root_(nullptr)
        , size_(init.size_)
    {
        if (init.root_ != nullptr)
        {
            root_ = init.root_;

            init.root_ = nullptr;
            init.size_ = 0;
        }
    }

    template <typename T>
    BST<T>::~BST()
    {
        clear();
    }

    template <typename T>
    BST<T>& BST<T>::operator=(const BST& init)
    {
        if (this != &init)
        {
            BST<T> temp(init);
            swap(temp);
        }
        return *this;
    }

    template <typename T>
    BST<T>& BST<T>::operator=(BST&& init) noexcept
    {
        if (this != &init)
        {
            BST<T> temp(std::move(init));
            swap(temp);
        }
        return *this;
    }

    // ==============================
    // mutator
    // ==============================

    template <typename T>
    bool BST<T>::insert(const T& value)
    {
        bool is_inserted = false;
        root_ = insert_impl(
            root_,
            value,
            is_inserted
        );

        if (is_inserted)
        {
            ++size_;
        }
        return is_inserted;
    }

    template <typename T>
    bool BST<T>::remove(const T& value)
    {
        bool is_removed = false;
        root_ = remove_impl(
            root_,
            value,
            is_removed
        );
        
        if (is_removed)
        {
            --size_;
        }
        return is_removed;
    }

    template <typename T>
    void BST<T>::clear()
    {
        destroy_impl(root_);
        root_ = nullptr;
        size_ = 0;
    }

    // ==============================
    // algorithm
    // ==============================

    template <typename T>
    bool BST<T>::find(const T& value) const
    {
        BSTNode* current_subtree = root_;
        bool is_found = false;
        
        while(current_subtree != nullptr)
        {
            if (value < current_subtree->data_)
            {
                current_subtree = current_subtree->left_;
            }
            else if (value > current_subtree->data_)
            {
                current_subtree = current_subtree->right_;
            }
            else
            {
                is_found = true;
                break;
            }
        }
        return is_found;
    }

    // ==============================
    // traverse
    // ==============================

    template <typename T>
    Array<T> BST<T>::pre_order() const
    {
        Array<T> result;
        pre_order_impl(root_, result);
        return result;
    }

    template <typename T>
    Array<T> BST<T>::in_order() const
    {
        Array<T> result;
        in_order_impl(root_, result);
        return result;
    }

    template <typename T>
    Array<T> BST<T>::post_order() const
    {
        Array<T> result;
        post_order_impl(root_, result);
        return result;
    }

    template <typename T>
    Array<T> BST<T>::level_order() const
    {
        Array<T> result;

        Queue<BSTNode*> node_queue;
        node_queue.push(root_);
        while (!node_queue.empty())
        {
            BSTNode* current_node = node_queue.front();

            if (current_node == nullptr)
            {
                node_queue.pop();
            }
            else
            {
                node_queue.push(current_node->left_);
                node_queue.push(current_node->right_);
                result.push_back(current_node->data_);
                node_queue.pop();
            }
        }

        return result;
    }

    // ==============================
    // swap
    // ==============================

    template <typename T>
    void BST<T>::swap(BST& other) noexcept
    {
        std::swap(size_, other.size_);
        std::swap(root_, other.root_);
    }

    template <typename T>
    bool operator==(const BST<T>& a, const BST<T>& b)
    {
        Array<T> a_in(a.in_order());
        Array<T> b_in(b.in_order());
        return a_in == b_in;
    }
}   // namespace data_struct