#!/bin/bash

set -eux
set -o pipefail

sudo apt install unixodbc-dev || exit $?
