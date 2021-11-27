// Copyright 2021 Mark Bond

#pragma once

#include <sql.h>

#include <stdexcept>

#include "Common/lib/raii_handle.h"

namespace Odbc {

class Exception :
    public std::runtime_error {
 private:
    SQLRETURN sqlReturn_{};

 public:
    Exception(SQLRETURN sqlReturn, const char * context);
    SQLRETURN SqlReturn() const;
};

void freeEnvHandle(SQLHANDLE h) noexcept;
using EnvHandle =
    utilities::RaiiHandle<
        SQLHANDLE, freeEnvHandle, reinterpret_cast<SQLHANDLE>(SQL_NULL_HANDLE)
    >;

// Wraps SQLAllocHandle for SQL_HANDLE_ENV.
// On success, returns a valid handle and sets sqlReturn.
// On failure, sets sqlReturn and returns a default constructed handle.
EnvHandle allocEnvHandle(SQLRETURN & sqlReturn);

// Wraps SQLAllocHandle for SQL_HANDLE_ENV.
// On success returns a valid handle.
// On failure, throws std::exception.
EnvHandle allocEnvHandle();

}  // namespace Odbc
