#pragma once

#include "../../memory/bag/block.hpp"
#include <concepts>

namespace conbench::concepts::memory::allocators {

template <typename Alloc, typename T>
concept AllocatorInterface = requires(Alloc alloc, const Alloc const_alloc, int tid, T* p, conbench::memory::bag::Block<T>* bag) {
    // Must be constructible with number of processes
    requires std::constructible_from<Alloc, int>;
    
    // Must have NUM_PROCESSES const member
    { const_alloc.NUM_PROCESSES } -> std::convertible_to<const int>;
    
    // Must have rebind template struct
    typename Alloc::template rebind<T>;
    typename Alloc::template rebind<T>::other;
    
    // Must have allocate method returning T*
    { alloc.allocate(tid) } -> std::same_as<T*>;
    
    // Must have deallocate method
    { alloc.deallocate(tid, p) } -> std::same_as<void>;
    
    // Must have deallocateAndClear method
    { alloc.deallocateAndClear(tid, bag) } -> std::same_as<void>;
    
    // Must have thread initialization methods
    { alloc.initThread(tid) } -> std::same_as<void>;
    { alloc.deinitThread(tid) } -> std::same_as<void>;
};

} // namespace conbench::memory::allocators
