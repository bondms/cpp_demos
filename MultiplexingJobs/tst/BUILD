load("@rules_cc//cc:defs.bzl", "cc_test")

cc_test(
    name = "multiplexor_test",
    srcs = [
        "multiplexor_test.cpp",
    ],
    copts = [
        "-Iexternal/gmock/include",
    ],
    deps = [
        "//lib:job_multiplexor",
        "@googletest//:gtest_main",
    ],
)
