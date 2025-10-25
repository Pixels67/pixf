#ifndef RESULT_HPP
#define RESULT_HPP

#include <iostream>
#include <utility>
#include <variant>

#include "../Common.hpp"

namespace Pixf::Core::Error {
    template<typename T, typename E = std::string>
    class Result {
    public:
        Result(const T &value) : m_Data(value) {}
        Result(const E &error) : m_Data(error) {}
        Result(T &&value) : m_Data(std::move(value)) {}
        Result(E &&error) : m_Data(std::move(error)) {}

        Result(const Result &) = default;
        Result(Result &&) = default;
        Result &operator=(const Result &) = default;
        Result &operator=(Result &&) = default;
        ~Result() = default;

        const T &Unwrap() const & {
            ASSERT(IsSuccess(), "Unwrap on non-successful Result<T, E>");
            return std::get<T>(m_Data);
        }

        T &&Unwrap() && {
            ASSERT(IsSuccess(), "Unwrap on non-successful Result<T, E>");
            return std::move(std::get<T>(m_Data));
        }

        const E &UnwrapError() const & {
            ASSERT(IsError(), "UnwrapError on successful Result<T, E>");
            return std::get<E>(m_Data);
        }

        E &&UnwrapError() && {
            ASSERT(IsError(), "UnwrapError on successful Result<T, E>");
            return std::move(std::get<E>(m_Data));
        }

        T UnwrapOr(T defaultValue) const & { return IsSuccess() ? std::get<T>(m_Data) : std::move(defaultValue); }

        T UnwrapOr(T defaultValue) && { return IsSuccess() ? std::move(std::get<T>(m_Data)) : std::move(defaultValue); }

        bool IsSuccess() const { return std::holds_alternative<T>(m_Data); }
        bool IsError() const { return std::holds_alternative<E>(m_Data); }

    private:
        std::variant<T, E> m_Data;
    };
} // namespace Pixf::Core::Error

#endif // RESULT_HPP
