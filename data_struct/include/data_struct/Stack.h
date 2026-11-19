#include "Array.h"

/**
 * @file Stack.h
 * @brief Stack interfaces implementations.
 * @ingroup container_adaptors
 * @author Nuchistone
 */

namespace data_struct
{
    /**
     * @brief Stack (FILO list), a container adaptator.
     * @details Encapsulate a container into a Stack
     * @tparam T The data type.
     * @tparam Container The container type, default Array.
     *         Container must provide these operations:
     *         `size()`, `empty()`;
     *         const and non-const override of `back()`;
     *         lvalue-reference and rvalue-reference override of `push_back()`;
     *         `pop_back()`
     */
    template <typename T, typename Container = Array<T>>
    class Stack
    {
    public:
        /**
         * @name Stack Size
         * @{
         */

        std::size_t size()
        {
            return c_.size();
        }
        bool empty()
        {
            return c.empty();
        }

        /** @} */   // name Stack Size

        /**
         * @name Stack Getter
         * @{
         */

        T& top()
        {
            return c_.back();
        }
        const T& top() const
        {
            return c_.back();
        }

        /** @} */   // name Stack Getter

        /**
         * @name Stack Mutator
         * @{
         */

        void push(const T& value)
        {
            c_.push_back(value);
        }
        void push(T&& value)
        {
            c_.push_back(std::move(value));
        }
        void pop()
        {
            c_.pop_back();
        }

        /** @} */   // name Stack Mutator
        
    private:
        Container c_;   /** container to adapt stack */
    };
}   //namespace data_struct