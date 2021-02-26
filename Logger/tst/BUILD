load("@rules_cc//cc:defs.bzl", "cc_test")

cc_test(
    name = "escaper_test",
    srcs = [
        "escaper_test.cpp",
    ],
    copts = [
        "-Iexternal/gmock/include",
    ],
    deps = [
        "//lib:escaper",
        "@googletest//:gtest_main",
    ],
)

cc_test(
    name = "flush_controller_test",
    srcs = [
        "flush_controller_test.cpp",
    ],
    copts = [
        "-Iexternal/gmock/include",
    ],
    deps = [
        "//lib:flush_controller",
        "@googletest//:gtest_main",
    ],
)

cc_test(
    name = "logger_test",
    srcs = [
        "logger_test.cpp",
    ],
    copts = [
        "-Iexternal/gmock/include",
    ],
    deps = [
        "//lib:logger",
        "@Common//lib:gtest_helper",
        "@googletest//:gtest_main",
    ],
)
