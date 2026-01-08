#pragma once

template <typename T>
class optional
{
private:
    bool has_value;
    union storage // выкинуть конструктор и деструктор и посмотреть что будет
    {
        T value;
        constexpr storage() noexcept {}
        ~storage() {}
    } storage;

public:
    optional();
    optional(const T& value);
    optional(T&& value);
    optional(const optional<T>& other);
    optional(optional<T>&& other);
    ~optional();

    optional& operator=(const T& value);
    optional& operator=(T&& value);
    optional& operator=(const optional& other);
    optional& operator=(optional&& other);
    optional& reset();

    const T& get_value() const;
    const T& operator*() const;

    bool is_sated();
    operator bool();
};

