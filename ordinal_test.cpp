#include <gtest/gtest.h>
#include "ordinal.hpp"

TEST(ordinal_test, default_constructor) 
{
    ordinal o;
    EXPECT_EQ(o.get_numerical_part(), 0);
    EXPECT_TRUE(o.is_finite());
}

TEST(ordinal_test, constructor_from_int)
{
    ordinal o(123);
    EXPECT_EQ(o.get_numerical_part(), 123);
    EXPECT_TRUE(o.is_finite());
}

TEST(ordinal_test, constructor_from_initializer_list)
{
    ordinal o({term(5, 3), term(2, 1)});
    EXPECT_EQ(o.get_numerical_part(), 0);
    EXPECT_FALSE(o.is_finite());
}

TEST(ordinal_test, constructor_from_cantor_form)
{
    ordinal::cantor_form form;
    form.append_element(term(3, 2));
    form.append_element(term(1, 0));
    ordinal o(form);
}

TEST(ordinal_test, constructor_from_term)
{
    ordinal o(term(12, 0));
    EXPECT_EQ(o.get_numerical_part(), 12);
    EXPECT_TRUE(o.is_finite());
    
    ordinal o2(term(5, 3));
    EXPECT_EQ(o2.get_numerical_part(), 0);
    EXPECT_FALSE(o2.is_finite());
}

TEST(ordinal_test, copy_constructor)
{
    ordinal o1(term(123, 1));
    ordinal o2(o1);
    EXPECT_EQ(o2, o1);
    EXPECT_EQ(o2.get_numerical_part(), o1.get_numerical_part());
}

TEST(ordinal_test, operator_addition)
{
    ordinal o1(term(1, 1));  
    ordinal o2(term(2, 0));  
    ordinal o3 = o1 + o2;
    
    EXPECT_EQ(o3.get_numerical_part(), 2);
    EXPECT_FALSE(o3.is_finite());
    
    ordinal o4 = o2 + o1;
    EXPECT_EQ(o4, o1);
}

TEST(ordinal_test, operator_addition_int)
{
    ordinal o1(term(5, 2));
    ordinal o2 = o1 + 3;
    
    EXPECT_EQ(o2.get_numerical_part(), 3);
    
    ordinal o3(10);
    ordinal o4 = o3 + 5;
    EXPECT_EQ(o4.get_numerical_part(), 15);
}

TEST(ordinal_test, operator_subtraction)
{
    ordinal o1(term(10, 0));  
    ordinal o2(term(3, 0));   
    ordinal o3 = o1 - o2;
    
    EXPECT_EQ(o3.get_numerical_part(), 7);
    
    ordinal omega(term(1, 1));
    ordinal o4 = omega - 3;
    EXPECT_EQ(o4, omega);
}

TEST(ordinal_test, operator_subtraction_int)
{
    ordinal o1(15);
    ordinal o2 = o1 - 5;
    
    EXPECT_EQ(o2.get_numerical_part(), 10);
    
    ordinal o3(term(1, 2));
    ordinal o4 = o3 - 10;
    EXPECT_EQ(o4, o3); 
}

TEST(ordinal_test, operator_multiplication)
{
    ordinal omega(term(1, 1));
    ordinal two(2);
    ordinal result = omega * two;
    
    EXPECT_EQ(result.get_numerical_part(), 0);
    EXPECT_FALSE(result.is_finite());
    
}

TEST(ordinal_test, operator_multiplication_int)
{
    ordinal o1(term(3, 2));
    ordinal o2 = o1 * 4;
    
    EXPECT_EQ(static_cast<const ordinal&>(o2).get_leading_term(), term(12,2));
}

TEST(ordinal_test, increment_operators)
{
    ordinal o1(5);
    
    ++o1;
    EXPECT_EQ(o1.get_numerical_part(), 6);
    
    ordinal o2 = o1++;
    EXPECT_EQ(o2.get_numerical_part(), 6);
    EXPECT_EQ(o1.get_numerical_part(), 7);
}

TEST(ordinal_test, operator_assignment)
{
    ordinal o1(10);
    ordinal o2(20);
    
    o2 = o1;
    EXPECT_EQ(o2.get_numerical_part(), 10);
    
    o2 = 30;
    EXPECT_EQ(o2.get_numerical_part(), 30);
}

TEST(ordinal_test, compound_addition_assignment)
{
    ordinal o1(term(1, 1));  // ω
    ordinal o2(term(3, 0));  // 3
    
    o1 += o2;
    EXPECT_EQ(o1.get_numerical_part(), 3);
    
    ordinal o3(10);
    o3 += 5;
    EXPECT_EQ(o3.get_numerical_part(), 15);
}

TEST(ordinal_test, compound_subtraction_assignment)
{
    ordinal o1(20);
    ordinal o2(5);
    
    o1 -= o2;
    EXPECT_EQ(o1.get_numerical_part(), 15);
    
    o1 -= 3;
    EXPECT_EQ(o1.get_numerical_part(), 12);
}

TEST(ordinal_test, operator_equality)
{
    ordinal o1(10);
    ordinal o2(10);
    ordinal o3(20);
    
    EXPECT_TRUE(o1 == o2);
    EXPECT_FALSE(o1 == o3);
    
    EXPECT_TRUE(o1 == 10);
    EXPECT_FALSE(o1 == 20);
}

TEST(ordinal_test, operator_inequality)
{
    ordinal o1(term(1, 2));
    ordinal o2(term(2, 2));
    ordinal o3(term(1, 3));
    
    EXPECT_TRUE(o1 != o2);
    EXPECT_TRUE(o1 != o3);
    
    EXPECT_TRUE(o1 != 0);
}

TEST(ordinal_test, operator_less_than)
{
    ordinal o1(5);
    ordinal o2(10);
    ordinal omega(term(1, 1));
    ordinal omega_squared(term(1, 2));
    
    EXPECT_TRUE(o1 < o2);
    EXPECT_TRUE(o1 < omega);
    EXPECT_TRUE(omega < omega_squared);
    
    EXPECT_TRUE(o1 < 10);
    EXPECT_TRUE(omega > 5);
}

TEST(ordinal_test, operator_greater_than)
{
    ordinal o1(15);
    ordinal o2(10);
    ordinal omega(term(1, 1));
    
    EXPECT_TRUE(o1 > o2);
    EXPECT_TRUE(omega > o1);
    
    EXPECT_TRUE(o1 > 10);
    EXPECT_TRUE(omega > 1000);
}

TEST(ordinal_test, operator_less_than_or_equal)
{
    ordinal o1(5);
    ordinal o2(5);
    ordinal o3(10);
    
    EXPECT_TRUE(o1 <= o2);
    EXPECT_TRUE(o1 <= o3);
    EXPECT_FALSE(o3 <= o1);
    
    EXPECT_TRUE(o1 <= 5);
    EXPECT_TRUE(o1 <= 10);
}

TEST(ordinal_test, operator_greater_than_or_equal)
{
    ordinal o1(10);
    ordinal o2(10);
    ordinal o3(5);
    
    EXPECT_TRUE(o1 >= o2);
    EXPECT_TRUE(o1 >= o3);
    EXPECT_FALSE(o3 >= o1);
    
    EXPECT_TRUE(o1 >= 10);
    EXPECT_TRUE(o1 >= 5);
}

TEST(ordinal_test, method_get_form)
{
    ordinal o({term(5, 3), term(2, 1)});
    const ordinal::cantor_form& form = static_cast<const ordinal&>(o).get_form();
    
    EXPECT_EQ(form.get_length(), 2);
    EXPECT_TRUE(form[0] == term(5, 3));
    EXPECT_TRUE(form[1] == term(2, 1));
}

TEST(ordinal_test, operator_subscript)
{
    ordinal o({term(3, 2), term(1, 0)});
    
    const ordinal& const_o = o;
    EXPECT_TRUE(const_o[0] == term(3, 2));
    EXPECT_TRUE(const_o[1] == term(1, 0));
    
    EXPECT_TRUE(o[0] == term(3, 2));

    EXPECT_THROW(o[10], std::out_of_range);
}

TEST(ordinal_test, method_get_leading_term)
{
    ordinal o({term(2, 3), term(5, 1), term(3, 0)});
    
    const term& leading = const_cast<const ordinal&>(o).get_leading_term();
    EXPECT_TRUE(leading == term(2, 3));
}

TEST(ordinal_test, method_get_term)
{
    const ordinal o({term(1, 2), term(3, 1), term(2, 0)});
    
    EXPECT_EQ(o.get_term(0), term(1, 2));
    EXPECT_EQ(o.get_term(1), term(3, 1));
    EXPECT_EQ(o.get_term(2), term(2, 0));
    
    EXPECT_THROW(o.get_term(5), std::out_of_range);
}

TEST(ordinal_test, method_add_term)
{
    ordinal o;
    
    o.add_term(term(5, 3));
    EXPECT_EQ(o[0], term(5, 3));
    
    o.add_term(term(2, 1));
}

TEST(ordinal_test, method_normalize)
{
    ordinal o;
    o.add_term(term(3, 2));
    o.add_term(term(1, 0));  
    o.add_term(term(2, 2));  
    
    o.normalize();
    
    EXPECT_TRUE(o[0] == term(5, 2)); 
}

TEST(ordinal_test, method_find_index)
{
    ordinal o({term(3, 2), term(1, 1), term(2, 0)});
    
    EXPECT_EQ(o.find_index(term(3, 2)), 0);
    EXPECT_EQ(o.find_index(term(1, 1)), 1);
    EXPECT_EQ(o.find_index(term(2, 0)), 2);
    
}

TEST(ordinal_test, method_is_finite)
{
    ordinal finite(42);
    EXPECT_TRUE(finite.is_finite());
    
    ordinal infinite(term(1, 1));
    EXPECT_FALSE(infinite.is_finite());
    
    ordinal mixed({term(1, 2), term(3, 0)});
    EXPECT_FALSE(mixed.is_finite());
}

TEST(ordinal_test, method_get_numerical_part)
{
    ordinal o1(100);
    EXPECT_EQ(o1.get_numerical_part(), 100);
    
    ordinal o2({term(1, 1), term(5, 0)});
    EXPECT_EQ(o2.get_numerical_part(), 5);
    
    ordinal o3;
    EXPECT_EQ(o3.get_numerical_part(), 0);
}

TEST(ordinal_test, edge_cases)
{
    ordinal empty;
    EXPECT_TRUE(empty.is_finite());
    EXPECT_EQ(empty.get_numerical_part(), 0);
    
    ordinal num_only(999);
    EXPECT_TRUE(num_only.is_finite());
    
    ordinal trans_only(term(1, 100));
    EXPECT_FALSE(trans_only.is_finite());
    EXPECT_EQ(trans_only.get_numerical_part(), 0);
}

TEST(ordinal_test, large_numbers)
{
    ordinal large(1000000);
    EXPECT_EQ(large.get_numerical_part(), 1000000);
    
    ordinal result = large + 500000;
    EXPECT_EQ(result.get_numerical_part(), 1500000);
}

TEST(ordinal_test, canonical_form_properties)
{
    ordinal o({term(5, 3), term(2, 1), term(3, 0)});
    
    EXPECT_GT(o[0].get_exponent(), o[1].get_exponent());
    EXPECT_GT(o[1].get_exponent(), o[2].get_exponent());
    
    EXPECT_GT(o[0].get_coefficient(), 0);
    EXPECT_GT(o[1].get_coefficient(), 0);
    EXPECT_GT(o[2].get_coefficient(), 0);
}

TEST(ordinal_test, complex_operations_chain)
{
    ordinal o1(10);
    ordinal o2(5);
    ordinal omega(term(1, 1));
    
    ordinal result = ((o1 + o2) * 2) + omega;
    
    EXPECT_FALSE(result.is_finite());
    EXPECT_EQ(result.get_numerical_part(), 0);
}

TEST(ordinal_test, self_assignment)
{
    ordinal o(42);
    
    o = o;
    EXPECT_EQ(o.get_numerical_part(), 42);
    
    o += o;
    EXPECT_EQ(o.get_numerical_part(), 84);
}

TEST(ordinal_test, addition_associativity)
{
    ordinal a(1);
    ordinal b(2);
    ordinal c(3);
    
    ordinal left = (a + b) + c;
    ordinal right = a + (b + c);
    
    EXPECT_EQ(left, right);
    EXPECT_EQ(left.get_numerical_part(), 6);
}
