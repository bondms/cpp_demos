load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "account",
    srcs = [
        "account.cpp",
    ],
    hdrs = [
        "account.h",
        "notifier_interface.h",
    ],
    visibility = ["//:__subpackages__"],
)

cc_library(
    name = "notifier",
    srcs = [
        "notifier.cpp",
        "notifier_interface.h",
    ],
    hdrs = [
        "notifier.h",
    ],
    visibility = ["//:__subpackages__"],
)
