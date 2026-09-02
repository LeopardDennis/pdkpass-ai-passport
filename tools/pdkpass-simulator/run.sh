#!/usr/bin/env bash
set -euo pipefail

simulator_dir="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
repo_root="$(cd -- "${simulator_dir}/../.." && pwd)"
build_dir="${PDKPASS_SIMULATOR_BUILD_DIR:-${repo_root}/build/pdkpass-simulator}"

cmake -S "${simulator_dir}" -B "${build_dir}" -DCMAKE_BUILD_TYPE=Debug
cmake --build "${build_dir}" --parallel
exec "${build_dir}/pdkpass-simulator" "$@"

