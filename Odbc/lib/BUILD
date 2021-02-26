load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "odbc",
    srcs = [
        "odbc.cpp",
    ],
    hdrs = [
        "odbc.h",
    ],
    deps = [
        "@Common//lib:raii_handle",
    ],
    visibility = ["//:__subpackages__"],
)
