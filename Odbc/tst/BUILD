load("@rules_cc//cc:defs.bzl", "cc_test")

cc_test(
  name = "odbc_test",
  srcs = [
    "odbc_test.cpp",
  ],
  deps = [
    "//Odbc/lib:odbc",
    "@googletest//:gtest_main",
  ],
  linkopts = [
    "-lodbc",
  ],
)
