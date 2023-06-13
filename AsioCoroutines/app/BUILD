load("@rules_cc//cc:defs.bzl", "cc_binary")

cc_binary(
  name = "main",
  srcs = [
    "main.cpp",
  ],
  deps = [
    "@asio",
    "//AsioCoroutines/lib:counted_generator",
    "//AsioCoroutines/lib:launcher",
    "//AsioCoroutines/lib:oddifier",
    "//AsioCoroutines/lib:random_generator",
    "//AsioCoroutines/lib:throttled_line_printer",
  ],
)
