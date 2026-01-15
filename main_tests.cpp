#include <gtest/gtest.h>
#include "ordinal.hpp"
#include "lazy_sequence.hpp"

#include "ordinal_test.cpp"

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}