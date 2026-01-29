#include <gtest/gtest.h>
#include "Math/Rotator.h"
#include "Math/Vector.h"
#include <glm/glm.hpp>
#include <sstream>

TEST(RotatorTests, Construction) {
    // Default constructor
    SRotator r1;
    EXPECT_FLOAT_EQ(r1.Yaw, 0.0f);
    EXPECT_FLOAT_EQ(r1.Pitch, 0.0f);
    EXPECT_FLOAT_EQ(r1.Roll, 0.0f);

    // Constructor with values (Pitch, Yaw, Roll)
    SRotator r2(45.0f, 90.0f, 180.0f);
    EXPECT_FLOAT_EQ(r2.Pitch, 45.0f);
    EXPECT_FLOAT_EQ(r2.Yaw, 90.0f);
    EXPECT_FLOAT_EQ(r2.Roll, 180.0f);
    
    // Constructor from scalar
    SRotator r3(5.0f);
    EXPECT_FLOAT_EQ(r3.Pitch, 5.0f);
    EXPECT_FLOAT_EQ(r3.Yaw, 5.0f);
    EXPECT_FLOAT_EQ(r3.Roll, 5.0f);

    // Constructor from glm::vec3
    glm::vec3 v(2.0f, 1.0f, 3.0f);
    SRotator r4(v);
    EXPECT_FLOAT_EQ(r4.Pitch, 2.0f);
    EXPECT_FLOAT_EQ(r4.Yaw, 1.0f);
    EXPECT_FLOAT_EQ(r4.Roll, 3.0f);
    
    // Constructor from SVector
    SVector sv(10.0f, 20.0f, 30.0f); // X->Pitch, Y->Yaw, Z->Roll based on impl
    // impl: Pitch = V.X; Yaw = V.Y; Roll = V.Z;
    SRotator r5(sv);
    EXPECT_FLOAT_EQ(r5.Pitch, 10.0f);
    EXPECT_FLOAT_EQ(r5.Yaw, 20.0f);
    EXPECT_FLOAT_EQ(r5.Roll, 30.0f);

    // Constructor from array
    float values[] = { 10.0f, 20.0f, 30.0f }; // Note: Implementation reads pFloat[0] for all three? Let's check existing code.
    SRotator r6(values);    
    EXPECT_FLOAT_EQ(r6.Pitch, 10.0f);
    EXPECT_FLOAT_EQ(r6.Yaw, 20.0f);
    EXPECT_FLOAT_EQ(r6.Roll, 30.0f);
}

TEST(RotatorTests, MathOperations) {
    SRotator r1(20.0f, 10.0f, 30.0f);
    SRotator r2(2.0f, 1.0f, 3.0f);

    // Addition
    SRotator add = r1 + r2;
    EXPECT_FLOAT_EQ(add.Pitch, 22.0f);
    EXPECT_FLOAT_EQ(add.Yaw, 11.0f);
    EXPECT_FLOAT_EQ(add.Roll, 33.0f);

    r1 += r2;
    EXPECT_FLOAT_EQ(r1.Pitch, 22.0f);
    EXPECT_FLOAT_EQ(r1.Yaw, 11.0f);
    EXPECT_FLOAT_EQ(r1.Roll, 33.0f);

    // Subtraction
    SRotator sub = r1 - r2;
    EXPECT_FLOAT_EQ(sub.Pitch, 20.0f);
    EXPECT_FLOAT_EQ(sub.Yaw, 10.0f);
    EXPECT_FLOAT_EQ(sub.Roll, 30.0f);

    r1 -= r2;
    EXPECT_FLOAT_EQ(r1.Pitch, 20.0f);
    EXPECT_FLOAT_EQ(r1.Yaw, 10.0f);
    EXPECT_FLOAT_EQ(r1.Roll, 30.0f);

    // Negation
    SRotator neg = -r1;
    EXPECT_FLOAT_EQ(neg.Pitch, -20.0f);
    EXPECT_FLOAT_EQ(neg.Yaw, -10.0f);
    EXPECT_FLOAT_EQ(neg.Roll, -30.0f);

    // Scalar Multiplication
    SRotator mul = r2 * 2.0f;
    EXPECT_FLOAT_EQ(mul.Pitch, 4.0f);
    EXPECT_FLOAT_EQ(mul.Yaw, 2.0f);
    EXPECT_FLOAT_EQ(mul.Roll, 6.0f);

    r2 *= 2.0f;
    EXPECT_FLOAT_EQ(r2.Pitch, 4.0f);
    EXPECT_FLOAT_EQ(r2.Yaw, 2.0f);
    EXPECT_FLOAT_EQ(r2.Roll, 6.0f);

    // Scalar Division
    SRotator div = r2 / 2.0f;
    EXPECT_FLOAT_EQ(div.Pitch, 2.0f);
    EXPECT_FLOAT_EQ(div.Yaw, 1.0f);
    EXPECT_FLOAT_EQ(div.Roll, 3.0f);

    // Division (scalar / rotator)
    SRotator rDiv(4.0f, 2.0f, 8.0f); // Yaw=2, Pitch=4, Roll=8
    SRotator inv = 16.0f / rDiv;
    EXPECT_FLOAT_EQ(inv.Pitch, 4.0f); // 16/4
    EXPECT_FLOAT_EQ(inv.Yaw, 8.0f);   // 16/2
    EXPECT_FLOAT_EQ(inv.Roll, 2.0f);  // 16/8
}

TEST(RotatorTests, Conversions) {
    SRotator r(0.0f, 90.0f, 0.0f); // Yaw=90, Pitch=0, Roll=0

    // ToGLM
    glm::vec3 glmVec = r.ToGLM();
    EXPECT_FLOAT_EQ(glmVec.x, 0.0f);
    EXPECT_FLOAT_EQ(glmVec.y, 90.0f);
    EXPECT_FLOAT_EQ(glmVec.z, 0.0f);

    // ToVector (Forward vector)    
    SVector v = r.ToVector();
    EXPECT_NEAR(v.X, 0.0f, 0.0001f);
    EXPECT_NEAR(v.Y, 0.0f, 0.0001f);
    EXPECT_NEAR(v.Z, 1.0f, 0.0001f);

    // Cast to float*
    const float* ptr = (const float*)r;
    EXPECT_EQ(ptr, &r.Pitch);
    EXPECT_FLOAT_EQ(ptr[0], 0.0f);
    EXPECT_FLOAT_EQ(ptr[1], 90.0f);
    EXPECT_FLOAT_EQ(ptr[2], 0.0f);
}

TEST(RotatorTests, StringOperations) {
    SRotator r(2.5f, 1.5f, 3.5f);

    // ToString
    // Implementation uses sprintf_s(Str,"(%.02f, %.02f, %.02f)", Pitch, Yaw, Roll);
    std::string s = r.ToString();
    EXPECT_EQ(s, "(2.50, 1.50, 3.50)");

    // Stream operator <<
    // Implementation: "(" + Pitch + " " + Yaw + " " + Roll + ")"
    std::stringstream ss;
    ss << r;
    std::string streamed = ss.str();
    // Pitch=2.5, Yaw=1.5, Roll=3.5
    // " (2.500000 1.500000 3.500000)"
    // Note: std::to_string format depends on impl, usually 6 decimals.
    EXPECT_NE(streamed.find("2.5"), std::string::npos);
    EXPECT_NE(streamed.find("1.5"), std::string::npos);
    EXPECT_NE(streamed.find("3.5"), std::string::npos);

    // Stream operator >>    
    std::string input = "(10.0 20.0 30.0)";
    std::stringstream iss(input);
    SRotator rIn;
    iss >> rIn;
    
    EXPECT_FLOAT_EQ(rIn.Pitch, 10.0f);
    EXPECT_FLOAT_EQ(rIn.Yaw, 20.0f);
    EXPECT_FLOAT_EQ(rIn.Roll, 30.0f);
}
