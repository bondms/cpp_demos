load("@rules_cc//cc:defs.bzl", "cc_test")

cc_test(
    name = "job_multiplexor_test",
    srcs = [
        "job_multiplexor_test.cpp",
    ],
    copts = [
        "-Iexternal/gmock/include",
    ],
    deps = [
        "//lib:job_multiplexor",
        "@googletest//:gtest_main",
    ],
)
