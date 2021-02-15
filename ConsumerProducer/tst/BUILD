load("@rules_cc//cc:defs.bzl", "cc_test")

cc_test(
    name = "consumer_test",
    srcs = [
        "consumer_test.cpp",
    ],
    copts = ["-Iexternal/gmock/include"],
    deps = [
        "@googletest//:gtest_main",
        "//lib:consumer",
        "//lib:event",
    ],
)
