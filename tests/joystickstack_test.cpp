#include <gtest/gtest.h>

#include <joystickstack.hpp>

TEST(JoystickStackTest, ShouldRegister_A) {
  JoystickStack stack;
  ASSERT_EQ(stack.a(true), 0);
}

TEST(JoystickStackTest, ShouldRegister_B) {
  JoystickStack stack;
  ASSERT_EQ(stack.b(true), 1023);
}

TEST(JoystickStackTest, ShouldRegisterLastPush_AB) {
  JoystickStack stack;
  stack.a(true);
  ASSERT_EQ(stack.b(true), 1023);
}

TEST(JoystickStackTest, ShouldRegisterLastPush_BA) {
  JoystickStack stack;
  stack.b(true);
  ASSERT_EQ(stack.a(true), 0);
}

TEST(JoystickStackTest, ShouldRestorePop_AB) {
  JoystickStack stack;
  stack.a(true);
  stack.b(true);
  ASSERT_EQ(stack.b(false), 0);
}

TEST(JoystickStackTest, ShouldRestorePop_BA) {
  JoystickStack stack;
  stack.b(true);
  stack.a(true);
  ASSERT_EQ(stack.a(false), 1023);
}

TEST(JoystickStackTest, ShouldKeep_AB) {
  JoystickStack stack;
  stack.a(true);
  stack.b(true);
  ASSERT_EQ(stack.a(false), 1023);
}

TEST(JoystickStackTest, ShouldRoll_BA) {
  JoystickStack stack;
  stack.b(true);
  stack.a(true);
  ASSERT_EQ(stack.b(false), 0);
}