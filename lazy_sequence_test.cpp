#include <gtest/gtest.h>
#include "lazy_sequence.hpp"

TEST(test_lazy_sequence_iterator, default_constructor_iterator)
{
    lazy_sequence<int>::ls_iterator it;
    EXPECT_EQ(it.get_num_part(), 0);
    EXPECT_FALSE(it.has_omega());
}

TEST(test_lazy_sequence_iterator, constructor_with_index)
{
    lazy_sequence<int>::ls_iterator it(42);
    EXPECT_EQ(it.get_num_part(), 42);
    EXPECT_FALSE(it.has_omega());
}

TEST(test_lazy_sequence_iterator, copy_constructor_iterator)
{
    lazy_sequence<int>::ls_iterator it_one(10);
    lazy_sequence<int>::ls_iterator it_two(it_one);
    EXPECT_EQ(it_two.get_num_part(), 10);
}

TEST(test_lazy_sequence_iterator_operators, prefix_increment)
{
    lazy_sequence<int>::ls_iterator it(5);
    ++it;
    EXPECT_EQ(it.get_num_part(), 6);
}

TEST(test_lazy_sequence_iterator_operators, postfix_increment)
{
    lazy_sequence<int>::ls_iterator it(5);
    auto old_it = it++;
    EXPECT_EQ(old_it.get_num_part(), 5);
    EXPECT_EQ(it.get_num_part(), 6);
}

TEST(test_lazy_sequence_iterator_methods, get_index_methods)
{
    lazy_sequence<int>::ls_iterator it(7);
    EXPECT_EQ(it.get_num_part(), 7);
    EXPECT_EQ(it.get_omega_part(), 0);

    ordinary &idx_ref = it.get_index();
    EXPECT_EQ(idx_ref.get_number(), 7);
}

TEST(test_lazy_sequence_iterator_methods, set_idx_method)
{
    lazy_sequence<int>::ls_iterator it;
    it.set_idx(25);
    EXPECT_EQ(it.get_num_part(), 25);
}

// Тесты для самой lazy_sequence
TEST(test_lazy_sequence, default_constructor)
{
    lazy_sequence<int> seq;
    // По умолчанию пустая последовательность
    EXPECT_EQ(seq.begin().get_num_part(), 0);
    EXPECT_EQ(seq.end().get_num_part(), 0);
}

TEST(test_lazy_sequence, constructor_from_initializer_list)
{
    lazy_sequence<int> seq = {1, 2, 3, 4, 5};

    // Проверяем доступ к элементам через get
    EXPECT_EQ(seq.get(0), 1);
    EXPECT_EQ(seq.get(1), 2);
    EXPECT_EQ(seq.get(4), 5);

    // Проверяем first и last
    EXPECT_EQ(seq.get_first(), 1);
    EXPECT_EQ(seq.get_last(), 5);
}

TEST(test_lazy_sequence, constructor_from_array)
{
    int array[] = {10, 20, 30, 40};
    lazy_sequence<int> seq(array, 4);

    EXPECT_EQ(seq.get(0), 10);
    EXPECT_EQ(seq.get(3), 40);
}

TEST(test_lazy_sequence, constructor_from_sequence)
{
    array_sequence<int> arr_seq = {100, 200, 300};
    lazy_sequence<int> seq(&arr_seq);

    EXPECT_EQ(seq.get(0), 100);
    EXPECT_EQ(seq.get(2), 300);
}

TEST(test_lazy_sequence, copy_constructor)
{
    lazy_sequence<int> seq_one = {5, 10, 15, 20};
    lazy_sequence<int> seq_two(seq_one);

    EXPECT_EQ(seq_two.get(0), 5);
    EXPECT_EQ(seq_two.get(2), 15);
    EXPECT_EQ(seq_two.get(3), 20);
}

TEST(test_lazy_sequence, constructor_with_unary_generator)
{
    array_sequence<int> base_seq = {1, 2, 3};

    auto double_generator = [](int x)
    { return x * 2; };

    lazy_sequence<int> seq(&base_seq, double_generator);
    EXPECT_EQ(seq.get(0), 1);
    EXPECT_EQ(seq.get(1), 2);
    EXPECT_EQ(seq.get(2), 3);
}

TEST(test_lazy_sequence, constructor_with_binary_generator)
{
    array_sequence<int> base_seq = {0, 1};
    auto fib_generator = [](int a, int b)
    { return a + b; };

    lazy_sequence<int> seq(&base_seq, fib_generator);
    EXPECT_EQ(seq.get(0), 0);
    EXPECT_EQ(seq.get(1), 1);
}

TEST(test_lazy_sequence, begin_end_methods)
{
    lazy_sequence<int> seq = {1, 2, 3, 4, 5};

    auto begin_it = seq.begin();
    auto end_it = seq.end();

    EXPECT_EQ(begin_it.get_num_part(), 0);
    EXPECT_EQ(end_it.get_num_part(), 5);
}

TEST(test_lazy_sequence, get_methods)
{
    lazy_sequence<int> seq = {10, 20, 30, 40, 50};
    EXPECT_EQ(seq.get(0), 10);
    EXPECT_EQ(seq.get(2), 30);
    EXPECT_EQ(seq.get(4), 50);
    EXPECT_EQ(seq.get_first(), 10);
    EXPECT_EQ(seq.get_last(), 50);
}

TEST(test_lazy_sequence, get_with_ordinary_index)
{
    lazy_sequence<int> seq = {1, 2, 3, 4, 5};
    ordinary idx(3);

    EXPECT_EQ(seq.get(idx), 4);
}

TEST(test_lazy_sequence, get_subsequence_finite)
{
    lazy_sequence<int> seq = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    lazy_sequence<int> *subseq = seq.get_subsequence(2, 6);

    EXPECT_NE(subseq, nullptr);
    EXPECT_EQ(subseq->get(0), 3); 
    EXPECT_EQ(subseq->get(1), 4);
    EXPECT_EQ(subseq->get(4), 8);

    delete subseq;
}

TEST(test_lazy_sequence, concat_method)
{
    lazy_sequence<int> seq1 = {1, 2, 3};
    lazy_sequence<int> seq2 = {4, 5, 6};

    lazy_sequence<int> *concatenated = seq1.concat(&seq2);

    EXPECT_NE(concatenated, nullptr);
    EXPECT_EQ(concatenated->get(0), 1);
    EXPECT_EQ(concatenated->get(2), 3);
    EXPECT_EQ(concatenated->get(3), 4);
    EXPECT_EQ(concatenated->get(5), 6);

    delete concatenated;
}

TEST(test_lazy_sequence, reduce_method)
{
    lazy_sequence<int> seq = {1, 2, 3, 4, 5};

    auto sum = [](int a, int b)
    { return a + b; };

    int result = seq.reduce(sum);

    EXPECT_EQ(result, 15);
}
TEST(test_lazy_sequence, lazy_evaluation_demo)
{
    array_sequence<int> base_seq = {1};
    auto double_gen = [](int x)
    { return x * 2; };

    lazy_sequence<int> seq(&base_seq, double_gen);

    EXPECT_EQ(seq.get(0), 1);
    EXPECT_EQ(seq.get(1), 2);
    EXPECT_EQ(seq.get(2), 4);
    EXPECT_EQ(seq.get(3), 8);
}

TEST(test_lazy_sequence, get_subsequence_with_ordinary)
{
    lazy_sequence<int> seq = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    ordinary start(2);
    ordinary end(7);

    lazy_sequence<int> *subseq = seq.get_subsequence(start, end);

    EXPECT_NE(subseq, nullptr);
    EXPECT_EQ(subseq->get(0), 3);
    EXPECT_EQ(subseq->get(4), 8);

    delete subseq;
}

TEST(test_lazy_sequence, iterator_traversal)
{
    lazy_sequence<int> seq = {10, 20, 30, 40, 50};

    auto it = seq.begin();
    int expected = 10;
    for (int i = 0; i < 5; i++)
    {
        int value = seq.get(it.get_num_part());
        EXPECT_EQ(value, expected);
        expected += 10;
        ++it;
    }

    EXPECT_EQ(it.get_num_part(), 5);
}

TEST(test_lazy_sequence, DISABLED_where_method)
{
    lazy_sequence<int> seq = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    auto isEven = [](int x)
    { return x % 2 == 0; };

    lazy_sequence<int>* filtered = seq.where(isEven);

    EXPECT_EQ(filtered->get(0), 2);
    EXPECT_EQ(filtered->get(1), 4);
    EXPECT_EQ(filtered->get(4), 10);

    delete filtered;
}