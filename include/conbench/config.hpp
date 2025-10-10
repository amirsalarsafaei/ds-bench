#pragma once

#include <cstdint>

namespace conbench {

struct Config {
  static constexpr uint64_t MAX_THREADS = 256;

  static constexpr uint64_t DEBRA_CHECK_THRESHOLD = 50;
  static constexpr uint64_t DEBRA_INCR_THRESHOLD = 100;
  static constexpr uint64_t DEBRA_BLOCK_SIZE = 256;
};

} // namespace conbench
