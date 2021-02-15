load("@rules_cc//cc:defs.bzl", "cc_test")

cc_test(
    name = "SharedPtrTest",
    srcs = [
        "SharedPtrTest.cpp",
    ],
    copts = ["-Iexternal/gmock/include"],
    deps = [
        "//lib:SharedPtr",
        "@googletest//:gtest_main",
    ],
)
