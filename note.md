# This is the note during the working process building project **Algorithm**

## About *Coding*

### General

1. Copy controls of ```T*``` has nothing to do with the ones of ```T```. Example: ```Node*``` and ```Node```, this is why Node doesn't need copy controllers.

### OOP tips for C++

Exmaple: *Array*

1. **The rule of 5**. In the five copy controller, difine all of them, or none of them;
2. The realization of **copy constructor** and **copy assignment operator**. In order to make the two functions more robust:
    (1) Use **unique_ptr** in copy constructor.     This avoid the memory leaking when the assignment in the loop fails:

    ```C++
    template <typename T>
    Array<T>::Array(const Array<T>& init): 
        length_(init.length_), data_(nullptr)
    {
        std::unique_ptr<T[]> temp(new T[init.length_]);
        for (std::size_t i = 0; i < length_; ++i) {
            temp[i] = init.data_[i];
        }
        data_ = temp.release();
    }
    ```

    (2) Use **swap** in copy assignment operator:

    ```C++
    template <typename T>
    void Array<T>::swap(Array& rhs) noexcept
    {
        std::swap(length_, rhs.length_);
        std::swap(data_, rhs.data_);
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

5. **Use ```noexcept```**.
6. **About std::move**. Only when designing ```Array::insert()``` do I know what ```std::move``` truly means. "std::move" **copies** the object (resource mannager) from the source and then **deletes** it, namely, it "steels" the object. However, **object does not mean the resource** that the object is taking charge of.

    ```std::string```, for example, is an object, restoring and taking charge of a char array (using a pointer), recording the state of the array, which serve as a **resource mannager**. Moving the string means copies the parts managing the char array (pointer and other things), changing the address of the mannager, but the array (resource) itself remains invariant. This is why we say ```std::move``` saves the cost of copying.

    Built-in types, like ```int```, is different. An int object itself is the resource, with no pointer managing the resource. So moving saves nothing comparing to copy when dealing with built-in types.

    This also help me to understand how array mannage resource. Arrays store the **mannager**, instead of the **resource**. Furthermore, when we say "arrays store continuously in physical memory", we means that **the physical address of the mannagers is continuous, not the address of the resources**. If the object in the array is not the built-in type, the resources could completely be sparsely assigned in the memory--it doesn't matter.

Example: *List*
1.**Using delegating constructors.** After using delegation, we can call the methods definded in the class to reuse the same logic in the constructor body. See the constructor receiving an ```initializer_list``` object in ```List```.

### Exception tips for C++

Example: *Array*

1. Use assert.

### Data structure design tips for C++

Example: *Array*

1. When designing a data structure, make it **capable to std**; make sure that the member functions in the class follow **the same convention as the std do**.
2. Three types of interators are defined in the class:

```C++
// template, typename T
using iterator = T*;
using const_iterator = const T*;

iterator begin() noexcept { return data_; }

const_iterator begin() const noexcept { return data_; }

const_iterator cbegin() const noexcept { return data_; }
```

The difference of the last two happens in the process of parameter matching if the object the iterator points to is *non-const*. In such case,  compiler will match ```iterator begin()``` instead of ```const_iterator begin()``` if one uses ```begin()```. Only using ```cbegin()``` explicitly can one assign a const_iterator to the non-const object.

### Test

## About *Data Structures*
