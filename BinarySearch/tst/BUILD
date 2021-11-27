load("@rules_cc//cc:defs.bzl", "cc_test")

cc_test(
    name = "binary_search_test",
    srcs = [
        "binary_search_test.cpp",
    ],
    copts = [
        "-Iexternal/gmock/include",
    ],
    deps = [
        "//BinarySearch/lib:binary_search",
        "@googletest//:gtest_main",
    ],
)
