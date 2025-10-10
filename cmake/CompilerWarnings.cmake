# Compiler-specific warning flags
function(set_project_warnings target)
    set(MSVC_WARNINGS
        /W4          # Baseline reasonable warnings
        /permissive- # Standards conformance mode
        /w14640      # Enable warning on thread unsafe static member init
        /w14826      # Conversion from 'type1' to 'type2' is sign-extended
        /w14905      # Wide string literal cast to LPSTR
        /w14906      # String literal cast to LPWSTR
    )

    set(GCC_CLANG_WARNINGS
        -Wall
        -Wextra
        -Wpedantic
        -Wshadow
        -Wnon-virtual-dtor
        -Wold-style-cast
        -Wcast-align
        -Wunused
        -Woverloaded-virtual
        -Wconversion
        -Wsign-conversion
        -Wnull-dereference
        -Wdouble-promotion
        -Wformat=2
        -Wimplicit-fallthrough
    )

    if(MSVC)
        set(PROJECT_WARNINGS ${MSVC_WARNINGS})
    else()
        set(PROJECT_WARNINGS ${GCC_CLANG_WARNINGS})
    endif()

    target_compile_options(${target} PRIVATE ${PROJECT_WARNINGS})

    # Sanitizers (optional but useful for concurrent programming)
    if(ENABLE_SANITIZERS)
        if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
            target_compile_options(${target} PUBLIC 
                -fsanitize=address,undefined,thread
                -fno-omit-frame-pointer
            )
            target_link_options(${target} PUBLIC 
                -fsanitize=address,undefined,thread
            )
        endif()
    endif()
endfunction()
