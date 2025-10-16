#pragma once

#include "../bag/block.hpp"

namespace conbench::memory::allocators {
template <typename T = void> class Interface {
public:
  const int NUM_PROCESSES;

  template <typename _Tp1> struct rebind {
    typedef Interface<_Tp1> other;
  };

  T* allocate(const int tid);
  void deallocate(const int tid, T* const p);
  void deallocateAndClear(const int tid, bag::Block<T>* const bag);

  void initThread(const int tid);
  void deinitThread(const int tid);

  Interface(const int numProcesses) : NUM_PROCESSES(numProcesses) {}
  ~Interface() {}
};
}; // namespace conbench::memory
