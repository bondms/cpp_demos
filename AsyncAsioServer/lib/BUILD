load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
  name = "console",
  srcs = [
    "console.cpp",
  ],
  hdrs = [
    "console.h",
  ],
  deps = [
    "tcp_server",
  ],
  visibility = ["//:__subpackages__"],
)

cc_library(
  name = "countdown_timer",
  srcs = [
  ],
  hdrs = [
    "countdown_timer.h",
  ],
  deps = [
    "@asio",
  ],
  visibility = ["//:__subpackages__"],
)

cc_library(
  name = "tcp_connection",
  srcs = [
    "tcp_connection.cpp",
  ],
  hdrs = [
    "tcp_connection.h",
  ],
  deps = [
    "countdown_timer",
  ],
  visibility = ["//:__subpackages__"],
)

cc_library(
  name = "tcp_server",
  srcs = [
    "tcp_server.cpp",
  ],
  hdrs = [
    "tcp_server.h",
  ],
  deps = [
    "tcp_connection",
  ],
  visibility = ["//:__subpackages__"],
)
