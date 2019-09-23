#pragma once

#include <cassert>

#ifdef __NVCC__
#ifndef PERFECT_HAS_CUDA
#define PERFECT_HAS_CUDA
#endif
#endif

#ifdef PERFECT_HAS_CUDA
#include <nvml.h>
#endif

namespace perfect {

enum class Result {
  NO_PERMISSION,
  NOT_SUPPORTED,
  NO_TASK, // task not found in CpuSet
  NVML_NO_PERMISSION,
  NVML_NOT_SUPPORTED,
  NVML_UNINITIALIZED,
  SUCCESS,
  UNKNOWN
};

#ifdef PERFECT_HAS_CUDA
Result from_nvml(nvmlReturn_t nvml) {
  switch (nvml) {
  case NVML_SUCCESS:
    return Result::SUCCESS;
  case NVML_ERROR_UNINITIALIZED:
    return Result::NVML_UNINITIALIZED;
  case NVML_ERROR_NOT_SUPPORTED:
    return Result::NVML_NOT_SUPPORTED;
  case NVML_ERROR_NO_PERMISSION:
    return Result::NVML_NO_PERMISSION;
  case NVML_ERROR_INVALID_ARGUMENT:
  case NVML_ERROR_GPU_IS_LOST:
  case NVML_ERROR_UNKNOWN:
  default:
    assert(0 && "unhandled nvmlReturn_t");
  }
  return Result::UNKNOWN;
}
#endif

const char *get_string(const Result &result) {
  switch (result) {
  case Result::SUCCESS:
    return "success";
  case Result::NO_PERMISSION:
    return "no permission";
  case Result::NO_TASK:
    return "no task";
  case Result::UNKNOWN:
    return "unknown error";
  case Result::NVML_NOT_SUPPORTED:
    return "nvidia-ml returned not supported";
  case Result::NVML_NO_PERMISSION:
    return "nvidia-ml returned no permission";
  case Result::NOT_SUPPORTED:
    return "unsupported operation";
  default:
    assert(0 && "unexpected perfect::Result");
  }

  assert(0 && "unreachable");
  return "";
}

inline void check(Result result, const char *file, const int line) {
  if (result != Result::SUCCESS) {
    fprintf(stderr, "%s@%d: perfect Error: %s\n", file, line,
            get_string(result));
    exit(-1);
  }
}

} // namespace perfect

#define PERFECT(stmt) check(stmt, __FILE__, __LINE__);
