load("@rules_cc//cc:defs.bzl", "cc_test")

cc_test(
    name = "binary_search_test",
    srcs = [
        "binary_search_test.cpp",
    ],
    copts = ["-Iexternal/gmock/include"],
    deps = [
        "@googletest//:gtest_main",
        "//lib:binary_search",
    ],
)
