#pragma once

#include "../config.hpp"
#include "bag/block.hpp"
#include <atomic>
#include <cstdint>
#include <cstdlib>
#include <new>

namespace conbench::memory::debra {

template <typename RecordType>
struct alignas(std::hardware_destructive_interference_size) ThreadState {
  int64_t tid{-1};
  uint64_t check_next{0};
  uint64_t ops_since_check{0};

  bag::RetiredBlock<RecordType>* bags[3]{nullptr, nullptr, nullptr};
  int64_t index{0};
};

template <typename RecordType> class Manager {
public:
  static Manager<RecordType>& instance() {
    static Manager<RecordType> instance_;
    return instance_;
  }

  Manager(const Manager&) = delete;
  Manager& operator=(const Manager&) = delete;

  void retire(bag::RetiredBlock<RecordType>* node) {
    if (!node)
      return;
    auto& state = get_thread_state();
    node->next = state.bags[state.index];
    state.bags[state.index] = node;
  }

  void enter_q_state() {
    auto tid = get_thread_state().tid;
    announce_[tid].value.fetch_or(1L, std::memory_order_relaxed);
  }

  void leave_q_state() {
    auto& state = get_thread_state();
    int64_t read_epoch = global_epoch_.value.load(std::memory_order_acquire);

    if (!is_equal(read_epoch,
                  announce_[state.tid].value.load(std::memory_order_relaxed))) {
      state.ops_since_check = 0;
      state.check_next = 0;
      rotate_and_reclaim(state);
    }

    if (++state.ops_since_check >= check_threshold_) {
      state.ops_since_check = 0;
      uint64_t num_threads = num_threads_.value.load(std::memory_order_relaxed);
      if (num_threads > 0) {
        int64_t other_pid = state.check_next % num_threads;
        int64_t other_announcement =
            announce_[other_pid].value.load(std::memory_order_acquire);

        if (is_equal(read_epoch, other_announcement) ||
            get_quiescent_bit(other_announcement)) {
          uint64_t c = ++state.check_next;
          if (c >= num_threads && c >= incr_threshold_) {
            global_epoch_.value.compare_exchange_strong(
                read_epoch, read_epoch + 2, std::memory_order_acq_rel,
                std::memory_order_acquire);
          }
        }
      }
    }

    announce_[state.tid].value.store(read_epoch, std::memory_order_release);
  }

private:
  Manager() {
    for (uint64_t i = 0; i < Config::MAX_THREADS; ++i) {
      announce_[i].value.store(1L, std::memory_order_relaxed);
    }
  }
  ~Manager() {}

  static bool get_quiescent_bit(int64_t announcement) {
    return announcement & 1L;
  }
  static bool is_equal(int64_t epoch, int64_t announcement) {
    return epoch == (announcement & ~1L);
  }

  void rotate_and_reclaim(ThreadState<RecordType>& state) {
    int64_t reclaim_idx = (state.index + 1) % 3;
    free_bag(state.bags[reclaim_idx]);
    state.bags[reclaim_idx] = nullptr;
    state.index = reclaim_idx;
  }

  void free_bag(bag::RetiredBlock<RecordType>* node) {
    while (node) {
      bag::RetiredBlock<RecordType>* next = node->next;
      delete node;
      node = next;
    }
  }

  ThreadState<RecordType>& get_thread_state() {
    thread_local ThreadState<RecordType> state;
    if (state.tid == -1) {
      register_thread(state);
    }
    return state;
  }

  void register_thread(ThreadState<RecordType>& state) {
    uint64_t tid = num_threads_.value.fetch_add(1, std::memory_order_relaxed);

    if (tid >= Config::MAX_THREADS) {
      abort();
    }

    state.tid = static_cast<int64_t>(tid);
    state.index = 0;
    announce_[state.tid].value.store(
        global_epoch_.value.load(std::memory_order_relaxed) | 1L,
        std::memory_order_relaxed);
  }

  struct alignas(std::hardware_destructive_interference_size) PaddedAtomic {
    std::atomic<int64_t> value{1L};
  };

  struct alignas(std::hardware_destructive_interference_size) PaddedAtomicU64 {
    std::atomic<uint64_t> value{0};
  };

  PaddedAtomic global_epoch_;
  PaddedAtomic announce_[Config::MAX_THREADS];
  PaddedAtomicU64 num_threads_;

  static constexpr uint64_t check_threshold_ = Config::DEBRA_CHECK_THRESHOLD;
  static constexpr uint64_t incr_threshold_ = Config::DEBRA_INCR_THRESHOLD;
};

template <typename RecordType> class Guard {
public:
  Guard(Manager<RecordType>& manager) : manager_(manager) {
    manager_.enter_q_state();
  }
  ~Guard() { manager_.leave_q_state(); }

  Guard(const Guard&) = delete;
  Guard& operator=(const Guard&) = delete;

private:
  Manager<RecordType>& manager_;
};

} // namespace conbench::memory::debra
