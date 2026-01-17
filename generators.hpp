#pragma once
#include <functional>
#include "pointers/uniq_ptr.hpp"
#include "pointers/shared_ptr.hpp"
#include "optional.hpp"

// Forward declarations
template <typename T> class lazy_sequence;
template <typename T> class array_sequence;
template <typename T> class sequence;

template <typename T>
class generator
{
public:
    virtual const T &get_other_next() = 0;
    virtual const T &get_next() = 0;
    virtual bool has_next() const = 0;
    virtual optional<T> try_get_next() = 0;
    virtual ~generator() = default;
};

template <typename T>
class unary_generator : public generator<T>
{
private:
    T generator_storage;
    std::function<T(T)> unary_function;

public:
    unary_generator(const lazy_sequence<T>& owner, std::function<T(T)> unfunc);
    ~unary_generator() = default;

    const T& get_other_next() override;
    const T& get_next() override;
    bool has_next() const override;
    optional<T> try_get_next() override;
};

template <typename T>
class binary_generator : public generator<T>
{
private:
    array_sequence<T> generator_storage;
    std::function<T(T,T)> binary_function;

public:
    binary_generator(const lazy_sequence<T>& onwer, std::function<T(T,T)> binfunc);
    ~binary_generator() = default;

    const T& get_other_next() override;
    const T& get_next() override;
    bool has_next() const override;
    optional<T> try_get_next() override;
};

template <typename T>
class nary_generator : public generator<T>
{
private:
    int arity;
    uniq_ptr<array_sequence<T>> generator_storage;
    std::function<T(sequence<T> *)> sequence_function;

public:
    nary_generator(const lazy_sequence<T>& owner, int ar, std::function<T(sequence<T> *)> seqfunc);
    nary_generator(const nary_generator<T>& other);
    ~nary_generator() = default;

    const T &get_other_next() override;
    const T &get_next() override;
    bool has_next() const override;
    optional<T> try_get_next() override;
};

template <typename T>
class concat_generator : public generator<T>
{
private:
    const lazy_sequence<T>* parent;
    const lazy_sequence<T>* other;

public:
    concat_generator(const lazy_sequence<T>& parent, const lazy_sequence<T>& other);
    ~concat_generator() = default;

    const T &get_other_next() override;
    const T &get_next() override;
    bool has_next() const override;
    optional<T> try_get_next() override;
};

template <typename T, typename T2>
class map_generator : public generator<T2>
{
private: 
    const lazy_sequence<T>* parent;
    std::function<T2(T)> map_function;

public:
    map_generator(const lazy_sequence<T>& parent, std::function<T2(T)> func);
    ~map_generator() = default;

    const T2 &get_other_next() override;
    const T2 &get_next() override;
    bool has_next() const override;
    optional<T2> try_get_next() override;
};

template <typename T>
class filter_generator : public generator<T>
{
private:
    const lazy_sequence<T>* parent;
    std::function<bool(T)> filter_function;

public:
    filter_generator(const lazy_sequence<T>& parent, std::function<bool(T)> filter_funcion);
    ~filter_generator() = default;

    const T& get_other_next() override;
    const T& get_next() override;
    bool has_next() const override;
    optional<T> try_get_next() override;
};

template <typename T>
class pull_generator : public generator<T>
{
private:
    const lazy_sequence<T>* parent;
    T element;
    ordinal index;
    ordinal index_initialize;

public:
    pull_generator(const lazy_sequence<T>& parent, const T& item, const ordinal& index, const ordinal& start_idx);
    ~pull_generator() = default;

    const T &get_other_next() override;
    const T &get_next() override;
    bool has_next() const override;
    optional<T> try_get_next() override;
};

#include "generators.tpp"