#pragma once
#include "lazy_sequence.hpp"

template <typename T>
class generator
{
public:
    virtual T &get_next() = 0;
    virtual bool has_next() = 0;
    virtual generator *copy() = 0;
};

template <typename T>
class unary_generator : public generator
{
private:
    T &generator_storage;
    std::function<T(T)> unary_function;

public:
    unary_generator(lazy_sequence<T> *owner, std::function<T(T)> unfunc);
    ~unary_generator() = default;

    T &get_next() override;
    bool has_next() override;
};

template <typename T>
class binary_generator : public generator
{
private:
    sequence<T> generator_storage;
    std::function<T(T,T)> binary_function;

public:
    binary_generator(lazy_sequence<T>* onwer, std::function<T(T,T)> binfunc);
    ~binary_generator() = default;

     T& get_next() override;
    bool has_next() override;
};

template <typename T>
class nary_generator : public generator
{
private:
    int arity;
    sequence<T> generator_storage;
    std::function<T(sequence<T> *)> sequence_function;

public:
    nary_generator(lazy_sequence<T> *owner, int ar, std::function<T(sequence<T> *)> seqfunc);
    ~nary_generator() = default;

    T &get_next() override;
    bool has_next() override;
};

template <typename T>
class skip_generator : public generator // подчеркивания убрать
{
private:
    shared_ptr<lazy_sequence<T>> parent;
    int start_idx;
    int end_idx;

public:
    skip_generator(lazy_sequence<T> *owner, lazy_sequence<T> *parent, int start_skip, int end_skip);
    ~skip_generator() = default;

    void skip();
    void skip(int new_start, int new_end);

    virtual T &get_next() override;
    virtual bool has_next() override;
};

template <typename T>
class insert_generator : public generator
{
private:
    shared_ptr<lazy_sequence<T>> parent;
    shared_ptr<lazy_sequence<T>> other;
    T &element;

public:
    insert_generator(shared_ptr<lazy_sequence<T>> parent, shared_ptr<lazy_sequence<T>> other);
    insert_generator(shared_ptr<lazy_sequence<T>> parent, T &elem);
    ~insert_generator() = default;

    T &get_element();
    T &get_other_next();

    virtual T &get_next() override;
    virtual bool has_next() override;
};

#include "generators.tpp"