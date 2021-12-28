#!/bin/bash

set -x

set -e
set -o pipefail
set -u

HERE="$(readlink -f "$(dirname "$0")")"
[[ -d "${HERE}" ]] || exit $?

find "${HERE}/." \
    \( -name "*.cpp" -o -name "*.cc" -o -name "*.h" \) \
    -print0 | xargs --null --max-args=1 clang-format -i -- || exit $?
