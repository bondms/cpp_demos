load("@rules_cc//cc:defs.bzl", "cc_test")

cc_test(
    name = "event_test",
    srcs = [
        "event_test.cpp",
    ],
    copts = [
        "-Iexternal/gmock/include"
    ],
    deps = [
        "//lib:event",
        "@googletest//:gtest_main",
    ],
)
