#pragma once
#include <cstddef>
#include <string>

template <typename K, typename V> class SetInterface {
public:
  virtual ~SetInterface() = default;

  /**
   * Insert key-value pair
   * @param tid Thread ID (0 to num_threads-1)
   * @return true if inserted, false if key already exists
   */
  virtual bool insert(const int tid, const K &key, const V &value) = 0;

  /**
   * Remove key from set
   * @param tid Thread ID
   * @return true if removed, false if key not found
   */
  virtual bool remove(const int tid, const K &key) = 0;

  /**
   * Check if key exists
   * @param tid Thread ID
   * @return true if found, false otherwise
   */
  virtual bool contains(const int tid, const K &key) = 0;

  /**
   * Get value associated with key
   * @param tid Thread ID
   * @param value Output parameter
   * @return true if found
   */
  virtual bool find(const int tid, const K &key, V &value) {
    return contains(tid, key); // Default implementation
  }

  // ============ Thread Management ============
  /**
   * Initialize thread-local state (MUST be called before thread uses structure)
   */
  virtual void initThread(const int tid) = 0;

  /**
   * Cleanup thread-local state (MUST be called before thread exits)
   */
  virtual void deinitThread(const int tid) = 0;

  // ============ Utility Methods ============
  /**
   * Get approximate size (may be inconsistent during concurrent ops)
   */
  virtual size_t size() const = 0;

  /**
   * Remove all elements (should be called when no concurrent operations)
   */
  virtual void clear() = 0;

  /**
   * Get implementation name for benchmark reporting
   */
  virtual std::string getName() const = 0;

  /**
   * Get implementation description/parameters
   */
  virtual std::string getDetails() const { return getName(); }

  // ============ Optional: Range Operations ============
  /**
   * Range query [min_key, max_key]
   * @param tid Thread ID
   * @param min_key Lower bound (inclusive)
   * @param max_key Upper bound (inclusive)
   * @param max_results Maximum results to return
   * @return Number of keys found
   */
  virtual int rangeQuery(const int tid, const K &min_key, const K &max_key,
                         K *keys, V *values, int max_results) {
    return 0; // Default: not supported
  }
};
