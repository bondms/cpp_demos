load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "event",
    srcs = [
        "Event.cpp",
    ],
    hdrs = [
        "Event.h",
    ],
    visibility = ["//:__subpackages__"],
)

cc_library(
    name = "SharedPtr",
    srcs = [
        "SharedPtr.cpp",
    ],
    hdrs = [
        "SharedPtr.h",
    ],
    deps = [
        ":event",
    ],
    visibility = ["//:__subpackages__"],
)
