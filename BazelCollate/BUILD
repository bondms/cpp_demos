filegroup(
    name = "all_applications",
    srcs = [
        "@AsyncAsioServer//app:main",
        "@BinarySearch//app:main",
        "@ConsumerProducer//app:main",
        "@Gmock//app:main",
        "@Jsoncpp//app:main",
        "@Logger//app:main",
        "@Odbc//app:main",
        "@Realign//app:main",
        "@SharedPtr//app:main",
        "@Shuffle//app:main",
        "@TurkishLocale//app:main",
        "@VariableLengthIntegerProtocol//app:main",
    ],
)

test_suite(
    name = "all_tests",
    tests = [
        "@BinarySearch//tst:binary_search_test",
        "@Common//tst:event_test",
        "@Common//tst:raii_handle_test",
        "@Common//tst:scope_exit_test",
        "@ConsumerProducer//tst:consumer_test",
        "@Gmock//tst:account_test",
        "@Gmock//tst:notifier_test",
        "@Logger//tst:escaper_test",
        "@Logger//tst:flush_controller_test",
        "@Logger//tst:logger_test",
        "@Odbc//tst:odbc_test",
        "@Realign//tst:realign_test",
        "@SharedPtr//tst:shared_ptr_test",
        "@Shuffle//tst:shuffle_test",
        "@VariableLengthIntegerProtocol//tst:encode_test",
    ],
)
