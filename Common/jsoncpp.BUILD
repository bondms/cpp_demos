cc_library(
    name = "jsoncpp",
    srcs = glob(
        include = [
            "src/lib_json/*",
        ],
        exclude = [
            "src/lib_json/CMakeLists.txt",
        ],
    ),
    hdrs = glob([
        "include/json/*",
    ]),
    includes = [
        "include",
    ],
    visibility = ["//visibility:public"],
)
