// Copyright 2021 Mark Bond

#pragma once

#include <gmock/gmock.h>

#include <functional>

namespace GtestHelper {

template <typename ExceptionType>
void expect_throw_with_callback(std::function<void()> code,
                                std::function<bool(ExceptionType e)> callback) {
  try {
    code();
    ADD_FAILURE() << "Exception not thrown";
  } catch (const ExceptionType &e) {
    EXPECT_TRUE(callback(e));
  } catch (...) {
    ADD_FAILURE() << "Unexpected exception type thrown";
  }
}

} // namespace GtestHelper
