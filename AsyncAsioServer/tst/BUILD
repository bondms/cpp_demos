load("@rules_cc//cc:defs.bzl", "cc_test")

cc_test(
  name = "countdown_timer_test",
  srcs = [
    "countdown_timer_test.cpp",
  ],
  deps = [
    "//AsyncAsioServer/lib:countdown_timer",
    "@googletest//:gtest_main",
  ],
)

cc_test(
  name = "tcp_server_test",
  srcs = [
    "tcp_server_test.cpp",
  ],
  deps = [
    "//AsyncAsioServer/lib:tcp_server",
    "@googletest//:gtest_main",
  ],
)
