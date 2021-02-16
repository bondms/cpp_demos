load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "event",
    srcs = [
        "event.cpp",
    ],
    hdrs = [
        "event.h",
    ],
    visibility = ["//:__subpackages__"],
)

cc_library(
    name = "shared_ptr",
    srcs = [
        "shared_ptr.cpp",
    ],
    hdrs = [
        "shared_ptr.h",
    ],
    deps = [
        ":event",
    ],
    visibility = ["//:__subpackages__"],
)
