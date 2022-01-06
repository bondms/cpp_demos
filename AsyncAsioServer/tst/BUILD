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

cc_test(
  name = "tcp_connection_test",
  srcs = [
    "tcp_connection_test.cpp",
  ],
  copts = [
    "-Iexternal/gmock/include",
  ],
  deps = [
    "//AsyncAsioServer/lib:tcp_connection",
    "@googletest//:gtest_main",
  ],
)
