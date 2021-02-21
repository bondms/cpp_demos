load("@rules_cc//cc:defs.bzl", "cc_test")

cc_test(
    name = "consumer_test",
    srcs = [
        "consumer_test.cpp",
    ],
    copts = [
        "-Iexternal/gmock/include"
    ],
    deps = [
        "//lib:consumer",
        "@Common//lib:event",
        "@googletest//:gtest_main",
    ],
)
