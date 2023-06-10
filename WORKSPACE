load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

http_archive(
    name = "googletest",
    url = "https://github.com/google/googletest/archive/release-1.12.1.zip",
    strip_prefix = "googletest-release-1.12.1",
    sha256 = "24564e3b712d3eb30ac9a85d92f7d720f60cc0173730ac166f27dda7fed76cb2",
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
    url = "https://github.com/chriskohlhoff/asio/archive/refs/tags/asio-1-28-0.zip",
    build_file = "@//:asio.BUILD",
    strip_prefix = "asio-asio-1-28-0",
    sha256 = "5c2af07ef73b42a2d48e34c0ecbf41cd40dc823bc681bf5833c129384999c963",
)
