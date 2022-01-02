load("@rules_cc//cc:defs.bzl", "cc_test")

cc_test(
  name = "completor_test",
  srcs = [
    "completor_test.cpp",
  ],
  copts = [
    "-Iexternal/gmock/include",
  ],
  deps = [
    "//MultiplexingJobs/lib:multiplexing_jobs",
    "//Common/lib:event",
    "@googletest//:gtest_main",
  ],
)

cc_test(
  name = "initiator_test",
  srcs = [
    "initiator_test.cpp",
  ],
  copts = [
    "-Iexternal/gmock/include",
  ],
  deps = [
    "//MultiplexingJobs/lib:multiplexing_jobs",
    "@googletest//:gtest_main",
  ],
)

cc_test(
  name = "multiplexor_test",
  srcs = [
    "multiplexor_test.cpp",
  ],
  copts = [
    "-Iexternal/gmock/include",
  ],
  deps = [
    "//MultiplexingJobs/lib:multiplexing_jobs",
    "//Common/lib:event",
    "@googletest//:gtest_main",
  ],
)

cc_test(
  name = "pool_test",
  srcs = [
    "pool_test.cpp",
  ],
  copts = [
    "-Iexternal/gmock/include",
  ],
  deps = [
    "//MultiplexingJobs/lib:multiplexing_jobs",
    "@googletest//:gtest_main",
  ],
)
