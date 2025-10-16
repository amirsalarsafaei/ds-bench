# Benchmarks CMakeLists.txt
cmake_minimum_required(VERSION 3.14)

# Find required packages for benchmarks
find_package(Threads REQUIRED)

# Benchmark executable template
function(add_benchmark benchmark_name source_file)
    add_executable(${benchmark_name} ${source_file})
    target_link_libraries(${benchmark_name} 
        PRIVATE 
        conbench 
        Threads::Threads
        mimalloc-static
    )
    
    # Apply compiler warnings if available
    if(COMMAND set_project_warnings)
        set_project_warnings(${benchmark_name})
    endif()
    
    # Set output directory
    set_target_properties(${benchmark_name} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/benchmarks
    )
endfunction()

# Add benchmark implementations here when ready
# Example:
# add_benchmark(set_benchmark set_benchmark.cpp)

