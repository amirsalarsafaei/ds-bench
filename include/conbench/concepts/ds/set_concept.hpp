#pragma once
#include <cstddef>
#include <string>
#include <concepts>

namespace conbench::concepts::ds {

template <typename S, typename K, typename V>
concept ConcurrentSet =
    requires(S set, const S const_set, const int tid, const K& key,
             const V& value, V& out_value, K* keys, V* values) {
      { set.insert(tid, key, value) } -> std::same_as<bool>;
      { set.remove(tid, key) } -> std::same_as<bool>;
      { set.contains(tid, key) } -> std::same_as<bool>;
      { set.find(tid, key, out_value) } -> std::same_as<bool>;

      { set.initThread(tid) } -> std::same_as<void>;
      { set.deinitThread(tid) } -> std::same_as<void>;

      { const_set.size() } -> std::convertible_to<std::size_t>;
      { set.clear() } -> std::same_as<void>;
      { const_set.getName() } -> std::convertible_to<std::string>;
      { const_set.getDetails() } -> std::convertible_to<std::string>;

      {
        set.rangeQuery(tid, key, key, keys, values, int{})
      } -> std::convertible_to<int>;
    };

} // namespace conbench::concepts
