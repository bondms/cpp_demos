load("@rules_cc//cc:defs.bzl", "cc_test")

cc_test(
  name = "countdown_timer_test",
  srcs = [
    "countdown_timer_test.cpp",
  ],
  copts = [
    "-Iexternal/gmock/include",
  ],
  deps = [
    "//AsyncAsioServer/lib:countdown_timer",
    "@googletest//:gtest_main",
  ],
)
