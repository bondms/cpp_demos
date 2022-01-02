load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
  name = "shared_ptr",
  srcs = [
    "shared_ptr.cpp",
  ],
  hdrs = [
    "shared_ptr.h",
  ],
  deps = [
    "//Common/lib:event",
  ],
  visibility = ["//:__subpackages__"],
)
