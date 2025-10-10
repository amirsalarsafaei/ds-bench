{ pkgs ? import <nixpkgs> {} }:

pkgs.mkShell {
  name = "conbench-dev";

  buildInputs = with pkgs; [
    # Build tools
    cmake
    pkg-config

    # Compiler toolchain - focusing on g++
    gcc13
    gdb

    # Development tools
    valgrind

    # Optional: build performance
    ccache

    # Version control
    git
  ];

  # Environment variables
  shellHook = ''
    export CC=gcc
    export CXX=g++
    
    export CMAKE_C_COMPILER_LAUNCHER=ccache
    export CMAKE_CXX_COMPILER_LAUNCHER=ccache

    export CMAKE_BUILD_PARALLEL_LEVEL=$(nproc)
    
    # Force CMake to use Unix Makefiles generator
    export CMAKE_GENERATOR="Unix Makefiles"
  '';

  hardeningDisable = [ "fortify" ];
}
