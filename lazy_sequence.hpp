#pragma once

#include "../pointers/weak_ptr.hpp"
#include "../pointers/shared_ptr.hpp"
#include "../lab3_2ndsem/headers/sequence.hpp"
#include "../lab3_2ndsem/headers/array_sequence.hpp"

template <typename T>
class lazy_sequence 
{
private:
    class default_generator
    {
        weak_ptr<sequence<T>> owner;
        int arity;
        std::function<T(T)> unary_generator;
        std::function<T(T, T)> binary_generator;
        std::function<T(sequence<T>*)> generator;
    public:
        default_generator(sequence<T>* seq, int arity = 1, std::fucntion<T,(T)>> other_ungenerator);
        default_generator(sequence<T>* seq, int arity = 2, std::fucntion<T, (T, T)>> other_bingenerator);
        default_generator(sequence<T>* seq, int arity, std::fucntion<T(sequence<T>*)> other_generator);

        virtual T& get_next() override;
        virtual bool has_next() override;
    };

    class skip_generator : public default_generator
    {
        shared_ptr<lazy_sequence<T>> owner;
        shared_ptr<lazy_sequence<T>> parent;
        int start_skip;
        int end_skip;
    public:
        skip_generator(shared_ptr<lazy_sequence<T>> owner, int start_skip, int end_skip, shared_ptr<lazy_sequence<T>> parent);

        lazy_sequence<T>& skip
        virtual T& get_next() override;
        virtual bool has_next() override;
    };

    class insert_generator : public default_generator
    {
        shared_ptr<lazy_sequence<T>> owner;
        shared_ptr<lazy_sequence<T>> other;
        int start;
        int end;
    public:
        insert_generator(shared_ptr<lazy_sequence<T>> owner, shared_ptr<lazy_sequence<T>> other, int start, int end);
        insert_generator(shared_ptr<lazy_sequence<T>> owner, int index);

        lazy_sequence<T>& insert();
        lazy_sequence<T>& set(T& element, int idx);
        virtual T& get_next() override;
        virtual bool has_next() override;
    };



protected:
    uniq_ptr<sequence<T>> buffer;
    default_generator* geneartor__;

private:
    lazy_sequence(int start, int end, shared_ptr<lazy_sequence<T>> parent);

    default_generator* get_generator();

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
