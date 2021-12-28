#!/bin/bash

set -x

set -e
set -o pipefail
set -u

HERE="$(readlink -f "$(dirname "$0")")"
[[ -d "${HERE}" ]] || exit $?

pushd "${HERE}/." || exit $?
bazel query //... | xargs bazel test --test_output=errors -- || exit $?
popd || exit $?
