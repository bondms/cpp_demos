load("@rules_cc//cc:defs.bzl", "cc_test")

cc_test(
    name = "account_test",
    srcs = [
        "account_test.cpp",
    ],
    # copts = ["-Iexternal/gtest/include"],
    deps = [
        # "@gtest//:main",
        "//Gmock_bazel/lib:account",
    ],
)

cc_test(
    name = "notifier_test",
    srcs = [
        "notifier_test.cpp",
    ],
    # copts = ["-Iexternal/gtest/include"],
    deps = [
        # "@gtest//:main",
        "//Gmock_bazel/lib:notifier",
    ],
)
