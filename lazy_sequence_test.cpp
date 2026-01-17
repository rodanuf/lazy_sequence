#include <gtest/gtest.h>
#include <iostream>
#include "lazy_sequence.hpp"

TEST(lazy_sequence_test, default_constructor)
{
    const lazy_sequence<int> seq;

    EXPECT_EQ(seq.get_generator(), nullptr);
}

TEST(lazy_sequence_test, constructor_from_initializer_list)
{
    std::cout << "Before\n";
    lazy_sequence<int> seq = {1, 2, 3, 4, 5};
    std::cout << "After\n";

    EXPECT_EQ(seq.get(0), 1);
    EXPECT_EQ(seq.get(2), 3);
    EXPECT_EQ(seq.get(4), 5);
}

TEST(lazy_sequence_test, method_get_first)
{
    lazy_sequence<int> seq = {10, 20, 30, 40};

    EXPECT_EQ(seq.get_first(), 10);
}

TEST(lazy_sequence_test, method_get_last_memoized)
{
    lazy_sequence<int> seq = {10, 20, 30, 40};

    EXPECT_EQ(seq.get_last_memoized(), 40);
}

TEST(lazy_sequence_test, method_get_int_index)
{
    lazy_sequence<int> seq = {100, 200, 300, 400, 500};

    EXPECT_EQ(seq.get(0), 100);
    EXPECT_EQ(seq.get(2), 300);
    EXPECT_EQ(seq.get(4), 500);

    EXPECT_THROW(seq.get(-1), std::invalid_argument);
    EXPECT_THROW(seq.get(10), std::logic_error);
}

TEST(lazy_sequence_test, method_get_ordinal_index)
{
    lazy_sequence<int> seq = {0, 1, 2, 3, 4, 5};

    ordinal idx_two(2);
    ordinal idx_four(4);

    EXPECT_EQ(seq.get(idx_two), 2);
    EXPECT_EQ(seq.get(idx_four), 4);
}

TEST(lazy_sequence_test, method_reduce)
{
    lazy_sequence<int> seq = {1, 2, 3, 4};

    auto sum = [](int a, int b)
    { return a + b; };

    const int result = seq.reduce(sum, 0);
    EXPECT_EQ(result, 10);
}

TEST(lazy_sequence_test, method_reduce_multiplication)
{
    lazy_sequence<int> seq = {1, 2, 3, 4};

    auto multiply = [](int a, int b)
    { return a * b; };

    const int &result = seq.reduce(multiply, 1);
    EXPECT_EQ(result, 24);
}

TEST(lazy_sequence_test, method_map)
{
    lazy_sequence<int> seq = {1, 2, 3, 4, 5};

    auto times_two = [](int x)
    { return x * 2; };

    lazy_sequence<int> *mapped = seq.map<int>(times_two);

    EXPECT_NE(mapped, nullptr);
    EXPECT_EQ(mapped->get(0), 2);
    EXPECT_EQ(mapped->get(1), 4);
    EXPECT_EQ(mapped->get(2), 6);
    EXPECT_EQ(mapped->get(3), 8);
    EXPECT_EQ(mapped->get(4), 10);

    delete mapped;
}

TEST(lazy_sequence_test, method_map_square)
{
    lazy_sequence<int> seq = {1, 2, 3};

    auto square = [](int x)
    { return x * x; };

    lazy_sequence<int> *mapped = seq.map<int>(square);

    EXPECT_NE(mapped, nullptr);
    EXPECT_EQ(mapped->get(0), 1);
    EXPECT_EQ(mapped->get(1), 4);
    EXPECT_EQ(mapped->get(2), 9);

    delete mapped;
}

TEST(lazy_sequence_test, method_where_even)
{
    lazy_sequence<int> seq = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    auto is_even = [](int x)
    { return x % 2 == 0; };

    lazy_sequence<int> *filtered = seq.where(is_even);

    EXPECT_NE(filtered, nullptr);
    EXPECT_EQ(filtered->get(0), 2);
    EXPECT_EQ(filtered->get(1), 4);
    EXPECT_EQ(filtered->get(2), 6);
    EXPECT_EQ(filtered->get(3), 8);
    EXPECT_EQ(filtered->get(4), 10);

    delete filtered;
}

TEST(lazy_sequence_test, method_where_greater_than)
{
    lazy_sequence<int> seq = {5, 10, 15, 20, 25, 30};

    auto greater_than_fifteen = [](int x)
    { return x > 15; };

    lazy_sequence<int> *filtered = seq.where(greater_than_fifteen);

    EXPECT_NE(filtered, nullptr);
    EXPECT_EQ(filtered->get(0), 20);
    EXPECT_EQ(filtered->get(1), 25);
    EXPECT_EQ(filtered->get(2), 30);

    delete filtered;
}

TEST(lazy_sequence_test, method_get_subsequence_int)
{
    lazy_sequence<int> seq = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

    lazy_sequence<int> *subseq = seq.get_subsequence(3, 8);

    EXPECT_NE(subseq, nullptr);
    EXPECT_EQ(subseq->get(0), 3);
    EXPECT_EQ(subseq->get(1), 4);
    EXPECT_EQ(subseq->get(2), 5);
    EXPECT_EQ(subseq->get(3), 6);
    EXPECT_EQ(subseq->get(4), 7);

    delete subseq;
}

TEST(lazy_sequence_test, method_get_subsequence_ordinal)
{
    lazy_sequence<int> seq = {10, 20, 30, 40, 50, 60};

    ordinal start(1);
    ordinal end(5);
    lazy_sequence<int> *subseq = seq.get_subsequence(start, end);

    EXPECT_NE(subseq, nullptr);
    EXPECT_EQ(subseq->get(0), 20);
    EXPECT_EQ(subseq->get(1), 30);
    EXPECT_EQ(subseq->get(2), 40);
    EXPECT_EQ(subseq->get(3), 50);

    delete subseq;
}

TEST(lazy_sequence_test, method_set_int_index)
{
    lazy_sequence<int> seq = {1, 2, 3, 4, 5};

    lazy_sequence<int> *modified = seq.set(99, 2);

    EXPECT_EQ(modified->get(0), 1);
    EXPECT_EQ(modified->get(1), 2);
    EXPECT_EQ(modified->get(2), 99);
    EXPECT_EQ(modified->get(3), 4);
    EXPECT_EQ(modified->get(4), 5);
}

TEST(lazy_sequence_test, method_set_first)
{
    lazy_sequence<int> seq = {10, 20, 30, 40, 50};

    lazy_sequence<int> *modified = seq.set_first(999);
    EXPECT_EQ(modified->get(0), 999);
}

TEST(lazy_sequence_test, method_set_last)
{
    lazy_sequence<int> seq = {10, 20, 30, 40, 50};

    lazy_sequence<int> *modified = seq.set_last_memoized(777);
    EXPECT_EQ(modified->get(4), 777);
}

TEST(lazy_sequence_test, method_concat)
{
    lazy_sequence<int> seq_one = {1, 2, 3};
    lazy_sequence<int> seq_two = {4, 5, 6};

    lazy_sequence<int> *concatenated = seq_one.concat(seq_two);

    EXPECT_NE(concatenated, nullptr);

    delete concatenated;
}

TEST(lazy_sequence_test, complex_operations_chain)
{
    lazy_sequence<int> seq = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    auto is_odd = [](int x)
    { return x % 2 != 0; };
    auto times_ten = [](int x)
    { return x * 10; };

    lazy_sequence<int> *step_one = seq.where(is_odd);
    lazy_sequence<int> *step_two = step_one->map<int>(times_ten);
    lazy_sequence<int> *result = step_two->get_subsequence(0, 3);

    EXPECT_NE(result, nullptr);
    EXPECT_EQ(result->get(0), 10);
    EXPECT_EQ(result->get(1), 30);
    EXPECT_EQ(result->get(2), 50);

    delete result;
    delete step_two;
    delete step_one;
}

TEST(lazy_sequence_test, method_map_different_types)
{
    lazy_sequence<int> int_seq = {1, 2, 3, 4};

    auto int_to_string = [](int x)
    { return "Number: " + std::to_string(x); };

    lazy_sequence<std::string> *str_seq = int_seq.map<std::string>(int_to_string);

    EXPECT_NE(str_seq, nullptr);
    EXPECT_EQ(str_seq->get(0), "Number: 1");
    EXPECT_EQ(str_seq->get(1), "Number: 2");
    EXPECT_EQ(str_seq->get(2), "Number: 3");
    EXPECT_EQ(str_seq->get(3), "Number: 4");

    delete str_seq;
}

TEST(lazy_sequence_test, method_where_complex_condition)
{
    lazy_sequence<int> seq = {15, 25, 35, 45, 55, 65};

    auto complex_condition = [](int x)
    { return (x % 5 == 0) && (x > 30); };

    lazy_sequence<int> *filtered = seq.where(complex_condition);

    EXPECT_NE(filtered, nullptr);
    EXPECT_EQ(filtered->get(0), 35);
    EXPECT_EQ(filtered->get(1), 45);
    EXPECT_EQ(filtered->get(2), 55);
    EXPECT_EQ(filtered->get(3), 65);

    delete filtered;
}

TEST(lazy_sequence_test, empty_sequence)
{
    lazy_sequence<int> empty_seq;

    EXPECT_THROW(empty_seq.get_first(), std::out_of_range);
    EXPECT_THROW(empty_seq.get_last_memoized(), std::out_of_range);

    EXPECT_THROW(empty_seq.get_subsequence(0, 1), std::invalid_argument);
}

TEST(lazy_sequence_test, single_element_sequence)
{
    lazy_sequence<int> single = {42};

    EXPECT_EQ(single.get_first(), 42);
    EXPECT_EQ(single.get_last_memoized(), 42);
    EXPECT_EQ(single.get(0), 42);

    lazy_sequence<int> *subseq = single.get_subsequence(0, 1);
    EXPECT_NE(subseq, nullptr);
    EXPECT_EQ(subseq->get(0), 42);
    delete subseq;
}

TEST(lazy_sequence_test, invalid_arguments)
{
    lazy_sequence<int> seq = {1, 2, 3};

    EXPECT_THROW(seq.concat(nullptr), std::invalid_argument);

    EXPECT_THROW(seq.get_subsequence(5, 2), std::invalid_argument);
    EXPECT_THROW(seq.get_subsequence(-1, 2), std::invalid_argument);
}

TEST(lazy_sequence_test, method_map_with_capture)
{
    lazy_sequence<int> seq = {1, 2, 3, 4, 5};

    int multiplier = 3;
    auto times_multiplier = [multiplier](int x)
    { return x * multiplier; };

    lazy_sequence<int> *mapped = seq.map<int>(times_multiplier);

    EXPECT_NE(mapped, nullptr);
    EXPECT_EQ(mapped->get(0), 3);  // 1*3
    EXPECT_EQ(mapped->get(1), 6);  // 2*3
    EXPECT_EQ(mapped->get(2), 9);  // 3*3
    EXPECT_EQ(mapped->get(3), 12); // 4*3
    EXPECT_EQ(mapped->get(4), 15); // 5*3

    delete mapped;
}

TEST(lazy_sequence_test, method_where_with_reference_capture)
{
    lazy_sequence<int> seq = {10, 20, 30, 40, 50};

    int threshold = 25;
    auto greater_than_threshold = [&threshold](int x)
    { return x > threshold; };

    lazy_sequence<int> *filtered = seq.where(greater_than_threshold);

    EXPECT_NE(filtered, nullptr);
    EXPECT_EQ(filtered->get(0), 30);
    EXPECT_EQ(filtered->get(1), 40);
    EXPECT_EQ(filtered->get(2), 50);

    delete filtered;
}
