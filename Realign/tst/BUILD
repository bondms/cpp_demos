load("@rules_cc//cc:defs.bzl", "cc_test")

cc_test(
    name = "realign_test",
    srcs = [
        "realign_test.cpp",
    ],
    copts = [
        "-Iexternal/gmock/include",
    ],
    deps = [
        "//Realign/lib:realign",
        "@googletest//:gtest_main",
    ],
)
