load("@rules_cc//cc:defs.bzl", "cc_binary")

cc_binary(
    name = "main",
    srcs = [
        "main.cpp",
    ],
    deps = [
        "//lib:job_multiplexor",
    ],
    visibility = ["//visibility:public"],
)
