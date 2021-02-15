load("@rules_cc//cc:defs.bzl", "cc_test")

cc_test(
    name = "EventTest",
    srcs = [
        "EventTest.cpp",
    ],
    copts = ["-Iexternal/gmock/include"],
    deps = [
        "@googletest//:gtest_main",
        "//lib:event",
    ],
)
