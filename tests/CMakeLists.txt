# Tests CMakeLists.txt
cmake_minimum_required(VERSION 3.14)

# Enable testing
enable_testing()

# Find required packages for testing
find_package(Threads REQUIRED)

# Test utility function
function(add_conbench_test test_name source_file)
    add_executable(${test_name} ${source_file})
    target_link_libraries(${test_name} 
        PRIVATE 
        conbench 
        Threads::Threads
    )
    
    # Apply compiler warnings if available
    if(COMMAND set_project_warnings)
        set_project_warnings(${test_name})
    endif()
    
    # Set output directory
    set_target_properties(${test_name} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/tests
    )
    
    # Add to CTest
    add_test(NAME ${test_name} COMMAND ${test_name})
endfunction()

# Add test implementations here when ready
# Example:
# add_conbench_test(memory_test memory_test.cpp)
# add_conbench_test(debra_test debra_test.cpp)

