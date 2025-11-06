#pragma once 

template <typename T>
class lazy_sequence 
{
private: 
    class generator
    {
    public:
        virtual T& get_next() = 0;
        virtual T& get_prev() = 0;
        virtual T& has_next() = 0;

        virtual shared_ptr<generator> insert(const shared_ptr<lazy_sequence<T>>& other, int start, int end) = 0;
        virtual shared_ptr<generator> get_segment(int start, int end, const shared_ptr<lazy_sequence>>& parent);
        virtual bool has_prev() = 0;
    };

    class default_generator : public generator
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

    class skip_generator : public generator
    {
        shared_ptr<lazy_sequence<T>> owner;
        shared_ptr<lazy_sequence<T>> parent;
        int start_skip;
        int end_skip;
    public:
        skip_generator(shared_ptr<lazy_sequence<T>> owner, int start_skip, int end_skip, shared_ptr<lazy_sequence<T>> parent);
    };

    class insert_generator : public generator
    {
        shared_ptr<lazy_sequence<T>> owner;
        shared_ptr<lazy_sequence<T>> other;
        int start;
        int end;
    public:
        insert_generator(shared_ptr<lazy_sequence<T>> owner, shared_ptr<lazy_sequence<T>> other, int start, int end);

        get_next
    };

protected:
    uniq_ptr<sequence<T>> materialized;
    uniq_ptr<sequence<operations>> ops;
    generator* geneartor__;

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


    void set(int index, T item);
    void set_first(T item);
    void set_last(T item);
    void add(T item);


};
