#!/bin/bash

set -x

set -e
set -o pipefail
set -u

HERE="$(readlink -f "$(dirname "$0")")"
[[ -d "${HERE}" ]] || exit $?

pushd "${HERE}/." || exit $?
bazel query //... | xargs bazel test -- || exit $?
popd || exit $?
