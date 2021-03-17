load("@rules_cc//cc:defs.bzl", "cc_test")

cc_test(
    name = "null_test",
    srcs = [
        "null_test.cpp",
    ],
    copts = [
        "-Iexternal/gmock/include",
    ],
    deps = [
        "@googletest//:gtest_main",
    ],
)
