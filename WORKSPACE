load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

http_archive(
    name = "googletest",
    url = "https://github.com/google/googletest/archive/release-1.10.0.zip",
    strip_prefix = "googletest-release-1.10.0",
    sha256 = "94c634d499558a76fa649edb13721dce6e98fb1e7018dfaeba3cd7a083945e91",
)

http_archive(
    name = "jsoncpp",
    url = "https://github.com/open-source-parsers/jsoncpp/archive/refs/tags/1.9.4.zip",
    build_file = "@//:jsoncpp.BUILD",
    strip_prefix = "jsoncpp-1.9.4",
    sha256 = "6da6cdc026fe042599d9fce7b06ff2c128e8dd6b8b751fca91eb022bce310880",
)

http_archive(
    name = "asio",
    url = "https://github.com/chriskohlhoff/asio/archive/refs/tags/asio-1-18-2.zip",
    build_file = "@//:asio.BUILD",
    strip_prefix = "asio-asio-1-18-2",
    sha256 = "9071370beb50f4e974042a2a8604e761397cc34a2021a49b5712571b5e1536d7",
)