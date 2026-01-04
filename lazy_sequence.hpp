#pragma once

#include <functional>
#include <initializer_list>
#include "../pointers/weak_ptr.hpp"
#include "../pointers/shared_ptr.hpp"
#include "../lab3_2ndsem/headers/sequence.hpp"
#include "../lab3_2ndsem/headers/array_sequence.hpp"
#include "../lab3_2ndsem/monad_header/monad_adapter.hpp"
#include "generators.hpp"
#include "ordinal.hpp" 
#include "cardinal.hpp"

// ui для алгебры как и списков, так и ординалов

template <typename T> 
class lazy_sequence 
{
private:
    shared_ptr<array_sequence<T>> memoized;
    generator<T>* generator_; //переименовать, убрать подчеркивания
    cardinal length; //

public:

    counter begin();
    counter end();

    lazy_sequence();
    lazy_sequence(T* items, int size);
    lazy_sequence(const std::initializer_list<T>& list); 
    lazy_sequence(sequence<T>* seq, generator<T>* gen = nullptr);
    ~lazy_sequence();

    // по константной ссылке
    T& get_first();
    T& get_last_memoized();
    T& get(const ordinal& index);
    T& get(int index);
    T& reduce(std::function<T(T, T)> func);

    lazy_sequence<T>* get_subsequence(int startidx, int endidx);
    lazy_sequence<T>* get_subsequence(ordinal& start_idx, ordinal& end_idx);
    lazy_sequence<T>* concat(lazy_sequence<T>* other);
    template <typename T2>
    lazy_sequence<T>* map(std::function<T2(T)> func);
    lazy_sequence<T>* where(std::function<bool(T)> func);
    lazy_sequence<T>* zip(sequence<T>* seq);

    lazy_sequence<T>& set(T& item, ordinal index);
    lazy_sequence<T>& set(T& item, int index);
    lazy_sequence<T>& set_first();
    lazy_sequence<T>& set_last();

private:
    lazy_sequence(int start, int end, shared_ptr<lazy_sequence<T>> parent);

    generator* get_generator();
    shared_ptr<array_sequence<T>> get_memoized();
    cardinal get_length();

    lazy_sequence<T>& set_generator(generator<T> *generator);
    lazy_sequence<T>& set_generator(generator<T> *generator, int arity);
    lazy_sequence<T>& set_memoized(shared_ptr<array_sequence<T>> memoized);
    lazy_sequence<T>& set_length(cardinal length);
};

#include "lazy_sequence.tpp"