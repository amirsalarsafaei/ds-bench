#pragma once

#include "../../config.hpp"

namespace conbench::memory::bag {

template <typename RecordType> struct RetiredBlock {
  virtual ~RetiredBlock() = default;
  RetiredBlock* next{nullptr};
  RecordType* block[conbench::Config::DEBRA_BLOCK_SIZE];
};

} // namespace conbench::memory::bag
