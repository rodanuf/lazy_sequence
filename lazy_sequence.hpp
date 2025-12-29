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

template <typename T> 
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

        ls_iterator operator++(int);
        ls_iterator& operator++();

        ls_iterator& increment_idx();
        ls_iterator& set_idx(int omega, int num);
        ls_iterator& set_idx(const ordinary& idx);
        ls_iterator& set_idx(int num);

        ordinary& get_index();
        const ordinary& get_index() const;

        bool has_omega();

        int get_num_part();
        int get_omega_part();

        void skip(int start, int end);
        void skip(ordinary start, ordinary end);
    };

private:
    shared_ptr<array_sequence<T>> memoized;
    generator<T>* generator_;
    cardinality length;

public:

    ls_iterator begin();
    ls_iterator end();

    lazy_sequence();
    lazy_sequence(T* items, int size);
    lazy_sequence(const std::initializer_list<T>& list);
    lazy_sequence(const std::initializer_list<T>& list, std::function<T(T)> other_generator);
    lazy_sequence(const std::initializer_list<T>& list, std::function<T(T, T)> other_generator);
    lazy_sequence(const std::initializer_list<T>& list, int arity, std::function<T(sequence<T>*)> other_generator); 
    lazy_sequence(sequence<T>* seq);
    lazy_sequence(sequence<T>* seq, std::function<T(T)> other_generator);
    lazy_sequence(sequence<T>* seq, std::function<T(T, T)> other_generator);
    lazy_sequence(sequence<T>* seq, int arity, std::function<T(sequence<T>*)> other_generator);
    lazy_sequence(const lazy_sequence<T>& other);
    lazy_sequence(const lazy_sequence<T>& seq_one, const lazy_sequence<T>& seq_two);
    ~lazy_sequence();

    T& get_first();
    T& get_last();
    T& get(const ordinary& index);
    T &get(int index);
    T& reduce(std::function<T(T, T)> func);

    lazy_sequence<T>* get_subsequence(int startidx, int endidx);
    lazy_sequence<T>* get_subsequence(ordinary& start_idx, ordinary& end_idx);
    lazy_sequence<T>* concat(lazy_sequence<T>* other);
    template <typename T2>
    lazy_sequence<T>* map(std::function<T2(T)> func);
    lazy_sequence<T>* where(std::function<bool(T)> func);
    lazy_sequence<T>* zip(sequence<T>* seq);

    lazy_sequence<T>& set(T& item, ordinary index);
    lazy_sequence<T>& set(T& item, int index);
    lazy_sequence<T>& set_first();
    lazy_sequence<T>& set_last();

private:
    lazy_sequence(int start, int end, shared_ptr<lazy_sequence<T>> parent);

    generator* get_generator();
    shared_ptr<array_sequence<T>> get_memoized();
    cardinality get_length();

    lazy_sequence<T>& set_generator(generator<T> *generator);
    lazy_sequence<T>& set_generator(generator<T> *generator, int arity);
    lazy_sequence<T>& set_memoized(shared_ptr<array_sequence<T>> memoized);
    lazy_sequence<T>& set_length(cardinality length);
};

#include "lazy_sequence.tpp"