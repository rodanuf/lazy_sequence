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
    class generator
    {
    public:
        virtual T& get_next() = 0;
        virtual bool has_next() = 0;
    };

    class unary_generator : public generator
    {
        T generator_storage;
        std::function<T(T)> unary_function;
    public:
        unary_generator(lazy_sequence<T>* owner, std::function<T(T)> unfunc);

        T& get_next() override;
        bool has_next() override;
    };

    class binary_generator : public generator
    {
        sequence<T> generator_storage;
        std::function<T(T,T)> binary_function;
    public:
        binary_generator(lazy_sequence<T>* onwer, std::function<T(T,T)> binfunc);

        T& get_next() override;
        bool has_next() override;
    }

    class nary_generator : public generator
    {
        int arity;
        sequence<T> generator_storage; 
        std::function<T(sequence<T>*)> sequence_function;
    public: 
        nary_generator(lazy_sequence<T>* owner, int ar, std::function<T(sequence<T>*)> seqfunc);

        T& get_next() override;
        bool has_next() override;
    }


    class skip_generator : public generator //подчеркивания убрать
    {
        shared_ptr<lazy_sequence<T>> parent;
        int start_idx;
        int end_idx;
    public:
        skip_generator(lazy_sequence<T>* owner, lazy_sequence<T>* parent, int start_skip, int end_skip);

        void skip();
        void skip(int new_start, int new_end);

        virtual T& get_next() override;
        virtual bool has_next() override;
    };

    class insert_generator : public generator
    {
        shared_ptr<lazy_sequence<T>> parent;
        shared_ptr<lazy_sequence<T>> other;
        int start_idx;
        int end_idx;
    public:
        insert_generator(shared_ptr<lazy_sequence<T>> parent, shared_ptr<lazy_sequence<T>> other, int start, int end);
        insert_generator(shared_ptr<lazy_sequence<T>> parent, int index);

        virtual T& get_next() override;
        virtual bool has_next() override;
    };



protected:
    uniq_ptr<sequence<T>> buffer;
    generator* geneartor_;

private:
    lazy_sequence(int start, int end, shared_ptr<lazy_sequence<T>> parent);

    generator* get_generator();

public:
    lazy_sequence();
    lazy_sequence(T* items, int size);
    lazy_sequence(sequence<T>* seq);
    lazy_sequence(sequence<T>* seq, std::function<T(T, T)> other_generator);
    lazy_sequence(sequence<T>* seq, std::function<T(T)> other_generator);
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