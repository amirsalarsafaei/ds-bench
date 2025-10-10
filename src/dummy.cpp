#include "conbench/config.hpp"
#include "conbench/memory/debra.hpp"

// This file exists solely to generate compile_commands.json entries
// for clangd to understand our header-only library

int main() {
    // Dummy usage to ensure everything compiles
    constexpr auto max_threads = conbench::Config::MAX_THREADS;
    (void)max_threads;
    return 0;
}
