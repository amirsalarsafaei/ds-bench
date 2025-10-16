#include "conbench/config.hpp"


int main() {
    constexpr auto max_threads = conbench::Config::MAX_THREADS;
    (void)max_threads;
    return 0;
}
