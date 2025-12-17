#pragma once

#include <functional>
#include <initializer_list>
#include "../pointers/weak_ptr.hpp"
#include "../pointers/shared_ptr.hpp"
#include "../lab3_2ndsem/headers/sequence.hpp"
#include "../lab3_2ndsem/headers/array_sequence.hpp"
#include "../lab3_2ndsem/monad_header/monad_adapter.hpp"
#include "generators.hpp"
#include "ordinary.hpp"
#include "cardinality.hpp"

template <typename T> //uml диаграмма данных
class lazy_sequence 
{
public:
    class ls_iterator
    {
    private:
        ordinary cur_idx;

    public: 
        ls_iterator();
        ls_iterator(int index);
        ls_iterator(const ordinary& other);
        ls_iterator(const ls_iterator& other);
        ~ls_iterator() = default;

        ls_iterator& operator++();
        ls_iterator operator++(int);

        ordinary& get_index();
        const ordinary& get_index() const;

        bool has_omega();

        int get_num_part();
        int get_omega_part();

        void skip(int start, int end);
        void skip(ordinary start, ordinary end);
        void increment_idx();
        void set_idx(int num);
    };

protected:
    shared_ptr<array_sequence<T>> buffer;
    generator<T>* generator_;
    cardinality length;

private:
    lazy_sequence(int start, int end, shared_ptr<lazy_sequence<T>> parent);

    generator* get_generator();

public:

    ls_iterator begin();
    ls_iterator end();

    lazy_sequence();
    lazy_sequence(T* items, int size);
    lazy_sequence(const std::initializer_list<T>& list);
    lazy_sequence(sequence<T>* seq);
    lazy_sequence(sequence<T>* seq, std::function<T(T)> other_generator);
    lazy_sequence(sequence<T>* seq, std::function<T(T, T)> other_generator);
    lazy_sequence(sequence<T>* seq, int arity, std::function<T(sequence<T>*)> other_generator);
    lazy_sequence(const lazy_sequence<T>& other);
    lazy_sequence(const lazy_sequence<T>& seq_one, const lazy_sequence<T>& seq_two);
    ~lazy_sequence();

    T& get_first();
    T& get_last();
    T& get(int index);
    T& get(const ordinary& index);
    T& reduce(std::function<T(T, T)> func);

    lazy_sequence<T>* get_subsequence(int startidx, int endidx);
    lazy_sequence<T>* get_subsequence(ordinary& start_idx, ordinary& end_idx);
    lazy_sequence<T>* concat(lazy_sequence<T>* other);
    lazy_sequence<T>* map(std::function<T(T)> func);
    lazy_sequence<T>* where(std::function<bool(T)> func);
    lazy_sequence<T>* zip(sequence<T>* seq);
    lazy_sequence<T>* set(T& item, int index); // remake with ordinal
    lazy_sequence<T>* set_first();
    lazy_sequence<T>* set_last();

};

#include "lazy_sequence.tpp"