load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
  name = "countdown",
  srcs = [
    "countdown.cpp",
  ],
  hdrs = [
    "countdown.h",
  ],
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
    "@asio",
  ],
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
