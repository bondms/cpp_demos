load("@rules_cc//cc:defs.bzl", "cc_test")

cc_test(
    name = "encode_test",
    srcs = [
        "encode_test.cpp",
    ],
    copts = [
        "-Iexternal/gmock/include",
    ],
    deps = [
        "//lib:encode",
        "@googletest//:gtest_main",
    ],
)
