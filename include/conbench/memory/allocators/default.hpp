#pragma once

#include "interface.hpp"

namespace conbench::memory::allocators {
template <typename T = void> class Default : Interface<T> {

  T* allocate(const int tid) { return new T; }

  void deallocate(const int tid, T* const t) { delete t; }

  void deallocateAndClear(const int tid, bag::Block<T>* const bag) {
    while (!bag->isEmpty()) {
      bag::Block<T>* i = bag->pop();
      deallocate(tid, i);
    }
  }

  void initThread(const int tid);
  void deinitThread(const int tid);
};
}; // namespace conbench::memory::allocators
