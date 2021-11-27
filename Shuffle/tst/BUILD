load("@rules_cc//cc:defs.bzl", "cc_test")

cc_test(
    name = "shuffle_test",
    srcs = [
        "shuffle_test.cpp",
    ],
    copts = [
        "-Iexternal/gmock/include",
    ],
    deps = [
        "//Shuffle/lib:shuffle",
        "@googletest//:gtest_main",
    ],
)
