#!/bin/bash

set -x

set -e
set -o pipefail
set -u

HERE="$(readlink -f "$(dirname "$0")")"
[[ -d "${HERE}" ]] || exit 1

pushd "${HERE}/." || exit 1
bazel query //... | xargs bazel test --test_output=errors -- || exit 1
popd || exit 1
