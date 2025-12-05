#pragma once

#include <functional>
#include "../pointers/weak_ptr.hpp"
#include "../pointers/shared_ptr.hpp"
#include "../lab3_2ndsem/headers/sequence.hpp"
#include "../lab3_2ndsem/headers/array_sequence.hpp"

template <typename T> //uml диаграмма данных
class lazy_sequence 
{
private:
    class ls_iterator
    {
    private:
        int cur_idx;
    public: 
        ls_iterator();
        ls_iterator(int index);
        ls_iterator(const ls_iterator& other);
        ~ls_iterator() = default;

        ls_iterator& operator++();
        ls_iterator operator++(int);

        int get_index();

        void skip(int start, int end);
        void increment_idx();
    };

protected:
    shared_ptr<sequence<T>> buffer;
    generator* generator_;
    shared_ptr<ls_iterator> iterator;

private:
    lazy_sequence(int start, int end, shared_ptr<lazy_sequence<T>> parent);

    generator* get_generator();

public:
    lazy_sequence();
    lazy_sequence(T* items, int size);
    lazy_sequence(sequence<T>* seq);
    lazy_sequence(sequence<T> *seq, std::function<T(T)> other_generator);
    lazy_sequence(sequence<T>* seq, std::function<T(T, T)> other_generator);
    lazy_sequence(sequence<T>* seq, int arity, std::function<T(sequence<T>*)> other_generator);
    lazy_sequence(const lazy_sequence<T>& other);
    ~lazy_sequence();

    T& get_first();
    T& get_last();
    T& get(int index);
    T& reduce(std::function<T(T, T)> func);

    lazy_sequence<T>* get_subsequence(int startidx, int endidx);
    lazy_sequence<T>* concat(lazy_sequence<T>* other);
    lazy_sequence<T>* concat(T* items, int size);
    lazy_sequence<T>* concat(sequence<T>* seq, std::function<T(T, T)> other_generator);
    lazy_sequence<T>* concat(sequence<T>* seq, std::function<T(T)> other_generator);
    lazy_sequence<T>* concat(sequence<T>* seq, int arity, std::function<T(sequence<T>*)> other_generator);
    lazy_sequence<T>* map(std::function<T(T)> func);
    lazy_sequence<T>* where(std::function<bool(T)> func);
    lazy_sequence<T>* zip(sequence<T>* seq);
    lazy_sequence<T>* set(T& item, int index); // remake with ordinal
    lazy_sequence<T>* set_first();
    lazy_sequence<T>* set_last();

};

#include "lazy_sequence.tpp"