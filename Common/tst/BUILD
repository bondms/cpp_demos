load("@rules_cc//cc:defs.bzl", "cc_test")

cc_test(
  name = "event_test",
  srcs = [
    "event_test.cpp",
  ],
  copts = [
    "-Iexternal/gmock/include",
  ],
  deps = [
    "//Common/lib:event",
    "@googletest//:gtest_main",
  ],
)

cc_test(
  name = "raii_handle_test",
  srcs = [
    "raii_handle_test.cpp",
  ],
  copts = [
    "-Iexternal/gmock/include",
  ],
  deps = [
    "//Common/lib:raii_handle",
    "@googletest//:gtest_main",
  ],
)

cc_test(
  name = "scope_exit_test",
  srcs = [
    "scope_exit_test.cpp",
  ],
  copts = [
    "-Iexternal/gmock/include",
  ],
  deps = [
    "//Common/lib:gtest_helper",
    "//Common/lib:scope_exit",
    "@googletest//:gtest_main",
  ],
)
