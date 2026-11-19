#include "List.h"

/**
 * @file Queue.h
 * @brief Queue interface implementations.
 * @ingroup container_adaptors
 * @author Nuchistone
 */

namespace data_struct
{
    /**
     * @brief Queue (FIFO list), a container adaptator.
     * @details Encapsulate a container into a Queue.
     * @tparam T The data type.
     * @tparam Container The container type, default List.
     *         Container must provide these operations:
     *         `size()`, `empty()`;
     *         const and non-const override of `front()`;
     *         lvalue-reference and rvalue-reference override of `push_back()`;
     *         `pop_front()`
     */
    template <typename T, typename Container = List<T>>
    class Queue
    {
    public:
        /**
         * @name Queue Size
         * @{
         */

        std::size_t size()
        {
            return c_.size();
        }
        bool empty()
        {
            return c_.empty();
        }

        /** @} */   // name Queue Size

        /**
         * @name Queue Getter
         * @{
         */

        T& front()
        {
            return c_.front();
        }
        const T& front() const
        {
            return c_.front();
        }

        /** @} */   // name Queue Getter

        /**
         * @name Queue Mutator
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
            c_.pop_front();
        }

        /** @} */   // name Queue Getter

    private:
        Container c_;   /**< container to adapt queue */
    };
}   // namespace data_struct