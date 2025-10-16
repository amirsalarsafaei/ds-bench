#pragma once

#include "../../config.hpp"
#include <cassert>

namespace conbench::memory::bag {

template <typename RecordType> class Block {
private:
  Block* next{nullptr};
  RecordType* block[conbench::Config::DEBRA_BLOCK_SIZE];
  std::size_t size{0};

public:
  virtual ~Block() = default;
  Block(Block* const _next) : next(_next), size(0) {}
  bool isEmpty() const { return size == 0; }
  bool isFull() const { return size == conbench::Config::DEBRA_BLOCK_SIZE; }
  std::size_t computeSize() const { return size; }

  void push(RecordType* const record) {
    assert(size < conbench::Config::DEBRA_BLOCK_SIZE);
    block[size++] = record;
  }

  RecordType* pop() {
    assert(size > 0);
    return block[size--];
  }
};

} // namespace conbench::memory::bag
