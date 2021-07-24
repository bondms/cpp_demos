// Copyright 2021 Mark Bond

#include "lib/odbc.h"

#include <cassert>
#include <string>

using std::string_literals::operator""s;

namespace Odbc {

Exception::Exception(SQLRETURN sqlReturn, const char * context) :
    std::runtime_error(context + ": "s + std::to_string(sqlReturn)),
    sqlReturn_{ sqlReturn } {
}

SQLRETURN Exception::SqlReturn() const {
    return sqlReturn_;
}

void freeEnvHandle(SQLHANDLE h) noexcept {
    const auto result{ SQLFreeHandle(SQL_HANDLE_ENV, h) };
    assert(SQL_SUCCEEDED(result));
}

EnvHandle allocEnvHandle(SQLRETURN & sqlReturn) {
    EnvHandle result{};
    sqlReturn = SQLAllocHandle(
        SQL_HANDLE_ENV, SQL_NULL_HANDLE, &result.getHandleReference());
    return result;
}

EnvHandle allocEnvHandle() {
    SQLRETURN sqlReturn{};
    auto result{ allocEnvHandle(sqlReturn) };
    if ( !SQL_SUCCEEDED(sqlReturn) ) {
        throw Exception(sqlReturn, __FUNCTION__);
    }
    return result;
}

}  // namespace Odbc
