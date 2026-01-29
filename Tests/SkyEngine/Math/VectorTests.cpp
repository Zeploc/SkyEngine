#include <gtest/gtest.h>
#include "Math/Vector.h"
#include <sstream>

TEST(VectorTests, Construction) {
    SVector v1;
    EXPECT_FLOAT_EQ(v1.X, 0.0f);
    EXPECT_FLOAT_EQ(v1.Y, 0.0f);
    EXPECT_FLOAT_EQ(v1.Z, 0.0f);

    SVector v2(1.0f, 2.0f, 3.0f);
    EXPECT_FLOAT_EQ(v2.X, 1.0f);
    EXPECT_FLOAT_EQ(v2.Y, 2.0f);
    EXPECT_FLOAT_EQ(v2.Z, 3.0f);

    SVector v3(5.0f);
    EXPECT_FLOAT_EQ(v3.X, 5.0f);
    EXPECT_FLOAT_EQ(v3.Y, 5.0f);
    EXPECT_FLOAT_EQ(v3.Z, 5.0f);
}

TEST(VectorTests, MathOperations) {
    SVector v1(1.0f, 2.0f, 3.0f);
    SVector v2(4.0f, 5.0f, 6.0f);

    SVector add = v1 + v2;
    EXPECT_FLOAT_EQ(add.X, 5.0f);
    EXPECT_FLOAT_EQ(add.Y, 7.0f);
    EXPECT_FLOAT_EQ(add.Z, 9.0f);

    SVector sub = v2 - v1;
    EXPECT_FLOAT_EQ(sub.X, 3.0f);
    EXPECT_FLOAT_EQ(sub.Y, 3.0f);
    EXPECT_FLOAT_EQ(sub.Z, 3.0f);

    float dot = v1.Dot(v2);
    // 1*4 + 2*5 + 3*6 = 4 + 10 + 18 = 32
    EXPECT_FLOAT_EQ(dot, 32.0f);
}

TEST(VectorTests, Normalization) {
    SVector v(10.0f, 0.0f, 0.0f);
    EXPECT_FLOAT_EQ(v.Size(), 10.0f);

    v.Normalize();
    EXPECT_FLOAT_EQ(v.X, 1.0f);
    EXPECT_FLOAT_EQ(v.Y, 0.0f);
    EXPECT_FLOAT_EQ(v.Z, 0.0f);
    EXPECT_FLOAT_EQ(v.Size(), 1.0f);
}

TEST(VectorTests, Rotate) {
    // Rotate vector (1, 0, 0) by 90 degrees around Z axis -> (0, 1, 0)
    SVector v(1.0f, 0.0f, 0.0f);
    SVector axis(0.0f, 0.0f, 1.0f);
    
    // Rotate takes degrees
    v.Rotate(90.0f, axis);

    // Allow for small floating point errors
    EXPECT_NEAR(v.X, 0.0f, 0.0001f);
    EXPECT_NEAR(v.Y, 1.0f, 0.0001f);
    EXPECT_NEAR(v.Z, 0.0f, 0.0001f);
}

TEST(VectorTests, StringParsing) {
    SVector v(1.5f, 2.5f, 3.5f);
    
    // Test ToString
    // Implementation uses std::to_string which might behave differently depending on locale/implementation
    // but usually produces "1.500000 2.500000 3.500000" or similar.
    // However, the operator<< adds brackets: "(X Y Z)"
    
    std::stringstream ss;
    ss << v;
    std::string output = ss.str();
    
    // Basic verification that numbers are present
    EXPECT_NE(output.find("1.5"), std::string::npos);
    EXPECT_NE(output.find("2.5"), std::string::npos);
    EXPECT_NE(output.find("3.5"), std::string::npos);
    EXPECT_NE(output.find("("), std::string::npos);
    EXPECT_NE(output.find(")"), std::string::npos);

    // Test operator>>
    // Format expected: "(X Y Z)" e.g. "(1.0 2.0 3.0)"
    std::string input = "(4.0 5.0 6.0)";
    std::stringstream input_ss(input);
    SVector v_in;
    input_ss >> v_in;
    
    EXPECT_FLOAT_EQ(v_in.X, 4.0f);
    EXPECT_FLOAT_EQ(v_in.Y, 5.0f);
    EXPECT_FLOAT_EQ(v_in.Z, 6.0f);
}
