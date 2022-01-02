load("@rules_cc//cc:defs.bzl", "cc_test")

cc_test(
  name = "account_test",
  srcs = [
    "account_test.cpp",
  ],
  copts = [
    "-Iexternal/gmock/include",
  ],
  deps = [
    "//Gmock/lib:account",
    "@googletest//:gtest_main",
  ],
)

cc_test(
  name = "notifier_test",
  srcs = [
    "notifier_test.cpp",
  ],
  copts = [
    "-Iexternal/gmock/include",
  ],
  deps = [
    "//Gmock/lib:notifier",
    "@googletest//:gtest_main",
  ],
)
