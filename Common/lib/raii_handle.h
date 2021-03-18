#pragma once

#include <memory>

namespace utilities
{
    template<typename HandleType, void (*freeFunction)(HandleType) noexcept, HandleType invalidValue = nullptr>
    class RaiiHandle
    {
        HandleType handle_{ invalidValue };
    public:
        RaiiHandle() noexcept = default;
        explicit RaiiHandle(HandleType handle) noexcept :
            handle_{ handle }
        {
        }

        ~RaiiHandle()
        {
            free();
        }

        RaiiHandle(const RaiiHandle &) = delete;
        RaiiHandle & operator=(const RaiiHandle &) = delete;

        RaiiHandle(RaiiHandle && other) noexcept :
            handle_{ other.detach() }
        {
        }

        RaiiHandle & operator=(RaiiHandle && other) noexcept
        {
            if ( this != &other )
            {
                free();
                handle_ = other.detach();
            }
            return *this;
        }

        bool isValid() const noexcept
        {
            return invalidValue != handle_;
        }

        HandleType handle() const noexcept
        {
            return handle_;
        }

        void free() noexcept
        {
            if ( isValid() )
            {
                freeFunction(handle_);
                handle_ = invalidValue;
            }
        }

        HandleType detach() noexcept
        {
            const auto temp{ handle_ };
            handle_ = invalidValue;
            return temp;
        }

        HandleType & getHandleReference() noexcept
        {
            return handle_;
        }

        bool operator<(const RaiiHandle & other) const noexcept
        {
            return this < std::addressof(other);
        }
    };
} // namespace utilities
