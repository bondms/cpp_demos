load("@rules_cc//cc:defs.bzl", "cc_test")

cc_test(
  name = "shared_ptr_test",
  srcs = [
    "shared_ptr_test.cpp",
  ],
  copts = [
    "-Iexternal/gmock/include",
  ],
  deps = [
    "//SharedPtr/lib:shared_ptr",
    "@googletest//:gtest_main",
  ],
)
