#pragma once

#include <functional>
#include <initializer_list>
#include "pointers/weak_ptr.hpp"
#include "pointers/shared_ptr.hpp"
#include "lab3_2ndsem/headers/sequence.hpp"
#include "lab3_2ndsem/headers/array_sequence.hpp"
#include "lab3_2ndsem/monad_header/monad_adapter.hpp"
#include "counter.hpp"
#include "generators.hpp"
#include "ordinal.hpp" 
#include "cardinal.hpp"


template <typename T> 
class lazy_sequence 
{
private:
    mutable shared_ptr<array_sequence<T>> memoized;
    generator<T>* gen; 
    cardinal length; 

public:

    counter begin() const;
    counter end() const;

    lazy_sequence();
    lazy_sequence(T* items, int size);
    lazy_sequence(const std::initializer_list<T>& list, generator<T>* gen = nullptr); 
    lazy_sequence(sequence<T>* seq, generator<T>* gen = nullptr);
    lazy_sequence(const lazy_sequence<T>& other) = delete;
    ~lazy_sequence();

    const T& get_first() const;
    const T& get_next() const;
    const T& get_last_memoized() const;
    const T& get(const ordinal& index) const;
    const T& get(int index) const;
    
    const T reduce(std::function<T(T, T)> func, const T& initial) const;

    lazy_sequence<T>* get_subsequence(int startidx, int endidx);
    lazy_sequence<T>* get_subsequence(const ordinal& start_idx, const ordinal& end_idx);
    lazy_sequence<T>* concat(const lazy_sequence<T>& other);
    template <typename T2>
    lazy_sequence<T2>* map(std::function<T2(T)> func);
    lazy_sequence<T>* where(std::function<bool(T)> func);
    lazy_sequence<T>* set(const T& item, const ordinal& index);
    lazy_sequence<T>* set(const T& item, int index);
    lazy_sequence<T>* set_first(const T &item);
    lazy_sequence<T>* set_last_memoized(const T &item);

    const generator<T>* get_generator() const;

    lazy_sequence<T>& set_generator(generator<T> *gen);
    lazy_sequence<T>& set_memoized(shared_ptr<array_sequence<T>> memoized);
    lazy_sequence<T>& set_length(cardinal length);

protected:

    generator<T>* get_generator();
    shared_ptr<array_sequence<T>> get_memoized();
    shared_ptr<array_sequence<T>> get_memoized() const;
    cardinal get_length();
    const cardinal get_length() const;

};

#include "lazy_sequence.tpp"