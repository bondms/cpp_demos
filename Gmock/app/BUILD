load("@rules_cc//cc:defs.bzl", "cc_binary")

cc_binary(
    name = "main",
    srcs = [
        "main.cpp",
    ],
    deps = [
        "//lib:account",
        "//lib:notifier",
    ],
    visibility = ["//visibility:public"],
)
