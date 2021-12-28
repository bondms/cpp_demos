load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

http_archive(
    name = "googletest",
    url = "https://github.com/google/googletest/archive/release-1.11.0.zip",
    strip_prefix = "googletest-release-1.11.0",
    sha256 = "353571c2440176ded91c2de6d6cd88ddd41401d14692ec1f99e35d013feda55a",
)

http_archive(
    name = "jsoncpp",
    url = "https://github.com/open-source-parsers/jsoncpp/archive/refs/tags/1.9.5.zip",
    build_file = "@//:jsoncpp.BUILD",
    strip_prefix = "jsoncpp-1.9.5",
    sha256 = "a074e1b38083484e8e07789fd683599d19da8bb960959c83751cd0284bdf2043",
)

http_archive(
    name = "asio",
    url = "https://github.com/chriskohlhoff/asio/archive/refs/tags/asio-1-21-0.zip",
    build_file = "@//:asio.BUILD",
    strip_prefix = "asio-asio-1-21-0",
    sha256 = "2edd7af3b5aa5746a0e552dfcd68b47d765a81695c4479c70027a378851a1bdc",
)
