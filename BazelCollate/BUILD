filegroup(
    name = "all_applications",
    srcs = [
        "@binary_search//app:main",
        "@consumer_producer//app:main",
        "@gmock//app:main",
        "@logger//app:main",
        "@shared_ptr//app:main",
    ],
)

test_suite(
    name = "all_tests",
    tests = [
        "@binary_search//tst:binary_search_test",
        "@common//tst:event_test",
        "@common//tst:raii_handle_test",
        "@common//tst:scope_exit_test",
        "@consumer_producer//tst:consumer_test",
        "@gmock//tst:account_test",
        "@gmock//tst:notifier_test",
        "@logger//tst:escaper_test",
        "@logger//tst:flush_controller_test",
        "@logger//tst:logger_test",
        "@shared_ptr//tst:shared_ptr_test",
    ],
)
