load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
  name = "escaper",
  srcs = [
    "escaper.cpp",
  ],
  hdrs = [
    "escaper.h",
  ],
  visibility = ["//:__subpackages__"],
)

cc_library(
  name = "flush_controller",
  srcs = [
  ],
  hdrs = [
    "flush_controller.h",
  ],
  visibility = ["//:__subpackages__"],
)

cc_library(
  name = "logger",
  srcs = [
    "logger.cpp",
  ],
  hdrs = [
    "logger.h",
  ],
  deps = [
    ":escaper",
    ":flush_controller",
    "//Common/lib:os",
    "//Common/lib:scope_exit",
  ],
  visibility = ["//:__subpackages__"],
)
