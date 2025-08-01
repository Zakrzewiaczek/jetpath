#include <gtest/gtest.h>
// Implementacje funkcji dla testów (bez main)
int add(int a, int b) {
    return a + b;
}

int multiply(int a, int b) {
    return a * b;
}

bool isEven(int number) {
    return number % 2 == 0;
}


// Testy funkcji multiply
TEST(MathTest, MultiplyTest) {
    EXPECT_EQ(multiply(2, 3), 6);
    EXPECT_EQ(multiply(-2, 3), -6);
    EXPECT_EQ(multiply(0, 5), 0);
    EXPECT_EQ(multiply(7, 7), 49);
}

// Testy funkcji isEven
TEST(MathTest, IsEvenTest) {
    EXPECT_TRUE(isEven(2));
    EXPECT_TRUE(isEven(0));
    EXPECT_TRUE(isEven(-4));
    EXPECT_FALSE(isEven(1));
    EXPECT_FALSE(isEven(3));
    EXPECT_FALSE(isEven(-1));
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
