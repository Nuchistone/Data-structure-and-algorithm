# This is the note during the working process building project **Algorithm**

## About *Coding* (C++)

### General

1. Copy controls of `T*` has nothing to do with the ones of `T`. Example: `Node*` and `Node`, this is why Node doesn't need copy controllers.
2. **About std::move**.
    Only when designing `Array::insert()` do I know what `std::move` truly means. "std::move" **copies** the object (resource mannager) from the source and then **deletes** it, namely, it "steels" the object. However, **object does not mean the resource** that the object is taking charge of.

    `std::string`, for example, is an object, restoring and taking charge of a char array (using a pointer), recording the state of the char array, which serve as a **resource mannager**. Moving the string means copies the parts managing the char array (pointer and other things), changing the address of the mannager, but the array (resource) itself remains invariant. This is why we say `std::move` saves the cost of copying.

    Built-in types, like `int`, is different. An int object itself is the resource, with no pointers managing the resource. So moving saves nothing comparing to copy when dealing with built-in types.

    This also help me to understand how array mannage resource. Arrays store the **mannager**, instead of the **resource**. Furthermore, when we say "arrays store continuously in physical memory", we means that **the physical address of the mannagers is continuous, not the address of the resources**. If the object in the array is not the built-in type, the resources could completely be sparsely assigned in the memory--it doesn't matter.
<!-- markdownlint-disable-next-line MD033 -->
1. **new, placement new, operator new**.<a id="new-new-new"></a>
    We use `new` to allocate memory on heap, but what `new` trully does is two things: **allocation and construction**. Here comes a question: if we want to segregate these two steps, what should we do? This problem do exist, and a perfect example is Array. In Array, we need to segregate "allocation" and "construction" to implement reallocation.

    The solution is `::operator new` and `placement new`. The former one allocate an **unconstructed memory** while the latter one construct on a specific memory by **calling a constructor explicitly**. These two operations have their corresponding destruction and deallocation: to destruct the data constructed by placement new, we have to **call the destructor explicitly**, and to deallocate the memory allocated by `::operator new`, we have to use **`::operator delete`**. The syntax is as below:

    ```C++
    // ::operator new
    T* data_memory = static_cast<T*>(::operator new(sizeof(T)));

    // placement new, calling the constructor explicitly
    T* data = new (data_memory) T(init);
    // syntax: new (address) constructor

    do_something(data);

    // destruction corresponding to placement new,
    // calling the destructor explicitly
    data.~T();

    // ::operator delete
    ::operator delete(data_memory);
    ```

    By using these two special operation, we have now completely segregate allocation and construction, which is the core function of them. However, I introduced them primarily dealing with the exception safety problem: if exception are thrown when the constructor using `new[]` allocate and construct objects, the memory allocated are leaked -- they are on the heap so will not be deallocated during stack unwinding. Then, by letting these two operation to take control of the construction, the problem won't exist. We can now control the whole process of allocation, construction, destruction and deallocation by ourselves!

    But I think we could benefit from it if we ask deeper: Why using `placement new` instead of  `unique_ptr` to deal with exception? Because we don't need it here, and it won't work as well as operator + placement here. `unique_ptr` will call the default constructor at once, which means many memory blocks will be overwrited when we use them -- the performance is not satisfying.

### OOP tips

1. **The rule of 5**. In the five copy controller, difine all of them, or none of them;
2. The realization of **copy constructor** and **copy assignment operator**. In order to make the two functions more robust:
    (1) We need to avoid memory leaking when the assignment in the loop fails. There are several different strategies.

    - `unique_ptr`
        The examples are Array and BST.
        Array:

        ```C++
        // unique_ptr version for Array; not adopted
        template <typename T>
        Array<T>::Array(const Array<T>& init)
            :length_(init.length_)
            ,data_(nullptr)
        {
            std::unique_ptr<T[]> temp(new T[init.length_]);
            for (std::size_t i = 0; i < length_; ++i) {
                temp[i] = init.data_[i];
            }
            data_ = temp.release();
        }
        ```

        BST:

        ```C++
        // unique_ptr version for BST; not adopted
        template <typename T>
        typename BST<T>::BSTNode*
        BST<T>::clone_impl(BSTNode* current_subtree) const
        {
            if (current_subtree == nullptr)
            {
                return nullptr;
            }

            auto cloned_subtree
                = std::make_unique<BSTNode>(current_subtree->data_);
            cloned_subtree->left_ = clone_impl(current_subtree->left_);
            cloned_subtree->right_ = clone_impl(current_subtree->right_);

            return cloned_subtree.release();
        }

        template <typename T>
        BST<T>::BST(const BST& init)
            : root_(nullptr)
            , size_(init.size_)
        {
            root_ = clone_impl(init.root_);
        }
        ```

        The ups of this design is that, if any exception are thrown in the constructor, `unique_ptr` guarantees the "new"ed memory are correctly reclaimed during stack unwinding, otherwise these memories will leak.

        Although I said that `unique_ptr` prevents the memory blocks from leaking, the `BST` example actually does NOT did this. If exceptions are thrown when cloning the left/right child, `unique_ptr` (inplicitly) calls `delete BSTNode`. But the destruction of `BSTNode` CANNOT delete its children recursively, so the child subtree will be leaked.

        Then comes a natural question: why we can use `unique_ptr` in `Array` to gurantee strong exceptional safety but can't in `BST`? The reason has already been pointed out: the destruction of `BST` is not recursive. Here we have a crucial conclusion: if the destruction of the type that the `unique_ptr` owns is not recursive, then `unique_ptr` CANNOT garuantee strong exceptional safety; instead, use try-catch would be a good choice. \o/\o/ The use of `unique_ptr` in `insert_impl` is a good example.
    - `::operator new` + placement new
        This design is used in Array not only in the copy constructor but also anywhere that allocate happens. See [new, replacement new, operator new](#new-new-new) for details.

    (2) Use **swap-and-copy** in copy assignment operator:

    ```C++
    template <typename T>
    Array<T>& Array<T>::operator=(const Array& init)
    {
        if (this != &init)
        {
            Array<T> temp(init);
            swap(temp);
        }
        return *this;
    }
    ```

3. **Template code organizing**. Use **.h** and **.tpp** to make your code defining templates clear. Put the declare and some defines in the .h file while complicate, long difines in the .tpp file. Don't forget to **include the .tpp file** at the end of .h, followed by **explicit extantiation**.
4. **Name of members**.

    ```C++
    namespace data_structure {
        std::size_t length_;    // data
        void empty();   // function
        class Array;    // class
    }
    ```

5. **Use `noexcept`**.

Example: *List*
1.**Using delegating constructors.** After using delegation, we can call the methods definded in the class to reuse the same logic in the constructor body. See the constructor receiving an `initializer_list` object in `List`.

### Exception tips for C++

Example: *Array*

1. Use assert.

### Data structure design tips for C++

#### About std

When designing a data structure, make it **capable to std**; make sure that the member functions in the class follow **the same convention as the std do**.

#### iterator (Array, List)

Three types of interators are defined in the classes Array and List:

```C++
// template, typename T
using iterator = T*;
using const_iterator = const T*;

iterator begin() noexcept { return data_; }

const_iterator begin() const noexcept { return data_; }

const_iterator cbegin() const noexcept { return data_; }
```

The difference of the last two happens in the process of parameter matching if the object the iterator points to is *non-const*. In such case,  compiler will match `iterator begin()` instead of `const_iterator begin()` if one uses `begin()`. Only using `cbegin()` explicitly can one assign a const_iterator to the non-const object.

#### Return type of `pop` methods

`pop`
I designed the "pop" operations returning `void` instead of `T`. This is based on several considerations.

1. **Semantics**. Following the **SRP** (Single Responsibility Principle), "pop" should simply means to pop an element out. This explanation is by no means "pop out and *take*".
2. **Exceptional safety**. This is actually a reason why we follow SRP. Consider a "pop" like this:

    ```C++
    T pop()
    {
        T value = the_last_element;
        remove(the_last_element);
        return value;
    }
    ```

    For compilers without "Mandatory Copy Elision", the method could throw exceptions (`bad_alloc`, for example) when calling the copy constructor. In such situation, the value is removed, but we did not get the value at all.
3. **Performance**. Also one of the reasons of SRP. If we simply want to pop an element out but not using it, the copy operation in the `pop` method would introduce extra time complexity into this method.

### Test

### Comment and documentation

Use Doxygen.

Useful tags:

- @brief @details @warning
- @param @return  @exception
- @invariant @pre @post
- @see

@brief -> @details -> @tparam -> @param -> @return -> @exception -> @pre -> @post -> @invariant -> @note/@warning -> @todo -> @see

@pre: This tag tells the users what should be guaranteed BEFORE the call; could write constraints on the caller (object) and the params.
@post: This tag tells the users what to pay extra attention on AFTER the call; could write status changing, static member modifying.

## About *Data Structures*

### BST

#### `remove_impl`

In the `remove_impl` method, I deal with the case in which both of the children node exist differently from the standard recursion.

The standard recursion method I mentioned refers to:

```C++
// BST.tpp, remove_impl method

// if both the children exist
// find the node with minimun data
BSTNode* min_in_right = current_subtree->right;
while (min_in_right->left != nullptr)
{
    min_in_right = min_in_right->left_;
}

// asign the minimum value to the current node and update the 
current_subtree->data_ = min_in_right->data_;
current_subtree = remove_impl(
    current_subtree->right,
    value,
    is_removed
);

return current_subtree;
```

In such an algorithm, the recursion will go throught the path finding the `min_in_right` for several more times, depending on the layers that the min exists. So if the min node is deep inside the tree, this implementation would slow the algorithm down while putting the system in the risk of stack overflowing.

The solution I proposed is that instead of doing the recursion on the `current->right`, I directly act on the min node. To make sure that the updated subtree starting from the min is still connected to the whole tree, I used another pointer `min_parent`, to store the position of the parent node of min. When we find min node, the succeeding process is actually removing a node that has no right child, so we can simply duplicate the code in `remove_impl` dealing with such case, and the part now is

```C++
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
```

Both time and space are saved.
