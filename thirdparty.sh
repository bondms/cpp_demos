#!/bin/bash

set -eux
set -o pipefail

HERE="$(readlink -e "$(dirname "$0")")"
[[ -d "${HERE}" ]] || exit $?

mkdir --verbose --parents -- "${HERE}/thirdparty/src" || exit $?


### gmock ###

[[ -d "${HERE}/thirdparty/src/googletest" ]] ||
    git clone --depth 1 --branch release-1.8.1 --verbose -- https://github.com/google/googletest.git "${HERE}/thirdparty/src/googletest" ||
    exit $?

for PROJECT in googletest googlemock
do
    mkdir --verbose --parents -- "${HERE}/thirdparty/build/${PROJECT}" || exit $?
    pushd -- "${HERE}/thirdparty/build/${PROJECT}" || exit $?
    cmake -- "${HERE}/thirdparty/src/googletest/${PROJECT}" || exit $?
    make || exit $?
    popd || exit $?
done


### jsoncpp ###

[[ -d "${HERE}/thirdparty/src/jsoncpp" ]] ||
    git clone --depth 1 --branch 0.10.7 --verbose -- https://github.com/open-source-parsers/jsoncpp.git "${HERE}/thirdparty/src/jsoncpp" ||
    exit $?

[[ -d "${HERE}/thirdparty/src/jsoncpp/dist" ]] || (
    pushd -- "${HERE}/thirdparty/src/jsoncpp" || exit $?
    python -- "${HERE}/thirdparty/src/jsoncpp/amalgamate.py" || exit $?
    popd || exit $?
)
