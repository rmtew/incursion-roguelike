#!/bin/bash

# Exit immediately if a command exits with a non-zero status.
set -e

################################################################################
# CONFIGURATION
################################################################################

# Compiler and flags
CXX="g++" # C++ compiler
CXXFLAGS=("-std=c++17" "-O2" -w) # Using C++17, with optimizations and warnings "-Wall" "-Wextra"
CC="gcc" # C compiler
CFLAGS=("-std=c11" "-O2" "-Wall" "-Wextra") # C compilation flags (e.g., C11 standard)

# Project directories
ROOT_DIR=$(pwd)
BUILD_DIR="${ROOT_DIR}/build"
DEPS_BUILD_DIR="${BUILD_DIR}/deps_install"

# libtcod configuration
LIBTCOD_SRC_DIR="${ROOT_DIR}/libtcod"

# modaccent configuration
MODACCENT_SRC_DIR="${ROOT_DIR}/modaccent"

# Incursion source files (adjust this to match your project)
INCURSION_SRC_FILES=$(find src -name "*.cpp")
INCURSION_EXE_NAME="incursion"

################################################################################
# HELPER FUNCTIONS
################################################################################

# A function to check if a command exists
command_exists() {
    command -v "$1" >/dev/null 2>&1
}

################################################################################
# BUILD STEPS
################################################################################

build_dependencies() {
    echo "--- Building Dependencies ---"
    mkdir -p "${DEPS_BUILD_DIR}/lib" # For compiled libraries
    mkdir -p "${DEPS_BUILD_DIR}/include" # For library headers

    # --- SDL2 ---
    echo "[Dep] Checking for SDL2..."
    # We check if the development package is installed.
    if pkg-config --exists sdl2; then
        echo "SDL2 development library found via pkg-config."
    else
        echo "WARNING: SDL2 development library not found."
        echo "Please install it using your system's package manager."
        echo "Example for Debian/Ubuntu: sudo apt-get install libsdl2-dev"
        exit 1
    fi

    # --- libtcod ---
    echo "[Dep] Checking for libtcod..."
    if [ ! -f "${DEPS_BUILD_DIR}/lib/libtcod.a" ]; then
        echo "libtcod not found in build directory. Building from source..."

        if [ ! -d "${LIBTCOD_SRC_DIR}" ]; then
            echo "ERROR: libtcod source directory not found at ${LIBTCOD_SRC_DIR}"
            echo "Please ensure the libtcod source is present in your project."
            exit 1
        fi

        echo "Building libtcod via direct compilation from local source..."
        cd "${LIBTCOD_SRC_DIR}"

        # Find all C and C++ source files needed for the library.
        LIBTCOD_SOURCES=$(find src -name "*.c" -o -name "*.cpp")
        OBJECTS_DIR="build/objects"
        mkdir -p "${OBJECTS_DIR}"

        # Compile all source files into object files (.o).
        # -fPIC is required for code that will be linked into a library.
        # We need to include the libtcod 'src' dir and the system SDL2 headers.
        echo "Compiling libtcod source files (mixed C/C++)..."
        
        # Base flags common to both C and C++ compilation for libtcod
        LIBTCOD_BASE_FLAGS=("-O2" "-g" "-Wall" "-fPIC" "-DTCOD_SDL2" "-DNO_OPENGL" $(pkg-config --cflags sdl2) "-Iinclude")

        OBJECT_FILES=()
        for SRC_FILE in ${LIBTCOD_SOURCES}; do
            # Determine compiler and flags based on file extension
            COMPILER=""
            COMPILE_FLAGS=()
            if [[ "${SRC_FILE}" == *.c ]]; then
                COMPILER="${CC}"
                COMPILE_FLAGS=("${CFLAGS[@]}" "${LIBTCOD_BASE_FLAGS[@]}")
            elif [[ "${SRC_FILE}" == *.cpp ]]; then
                COMPILER="${CXX}"
                COMPILE_FLAGS=("${CXXFLAGS[@]}" "${LIBTCOD_BASE_FLAGS[@]}" "-fpermissive") # -fpermissive is often C++ specific
            else
                echo "WARNING: Unknown source file type for libtcod: ${SRC_FILE}. Skipping."
                continue
            fi

            # Create a unique object file path by replacing slashes with underscores to avoid name collisions.
            OBJ_FILE="${OBJECTS_DIR}/$(echo "${SRC_FILE}" | tr '/' '_').o"
            echo "  - Compiling ${SRC_FILE} with ${COMPILER}"
            "${COMPILER}" "${COMPILE_FLAGS[@]}" -c "${SRC_FILE}" -o "${OBJ_FILE}"
            OBJECT_FILES+=("${OBJ_FILE}")
        done

        # Archive all the object files into a single static library (libtcod.a).
        echo "Archiving object files into libtcod.a..."
        ar rcs "${DEPS_BUILD_DIR}/lib/libtcod.a" "${OBJECT_FILES[@]}"

        # Copy the public headers to our dependency installation directory.
        echo "Installing libtcod headers..."
        cp -r include/* "${DEPS_BUILD_DIR}/include/"
        
        cd "${ROOT_DIR}" # Return to the project root
        echo "libtcod build complete."
    else
        echo "libtcod is already built."
    fi

    if [ ! -f "${BUILD_DIR}/modaccent" ]; then
        echo "Building modaccent..."
        cd "${MODACCENT_SRC_DIR}"
        "${CC}" "${CFLAGS[@]}" -Iinclude -o "${BUILD_DIR}/modaccent" $(find src -name "*.c")
        cd "${ROOT_DIR}" # Return to the project root
    else
        echo "modaccent is already built."
    fi
}

build_incursion() {
    # echo "--- Running flex ---"
    # rm src/Tokens.cpp
    # flex -B -i -osrc/Tokens.cpp lang/Tokens.lex
    # echo "--- Running modaccent ---"
    # rm src/yygram.cpp inc/yygram.h
    # ${BUILD_DIR}/modaccent lang/Grammar.acc src inc
    echo "--- Building Incursion ---"
    mkdir -p "${BUILD_DIR}/incursion"

    # Add include paths for our dependencies
    # SDL2 flags are retrieved using pkg-config for simplicity
    # We add our locally-built libtcod paths manually
    INCLUDE_PATHS="-Iinc -Ilib -I${DEPS_BUILD_DIR}/include $(pkg-config --cflags sdl2)"

    # Add library paths and libraries to link
    LINKER_PATHS="-L${DEPS_BUILD_DIR}/lib"
    LIBRARIES="$(pkg-config --libs sdl2) -ltcod"

    echo "Compiling and linking Incursion..."
    echo "Source files: ${INCURSION_SRC_FILES}"

    echo "Compiling with CXXFLAGS: ${CXXFLAGS[*]}"
    ${CXX} "${CXXFLAGS[@]}" -DLIBTCOD_TERM ${INCLUDE_PATHS} ${LINKER_PATHS} \
        -o "${BUILD_DIR}/incursion/${INCURSION_EXE_NAME}" \
        ${INCURSION_SRC_FILES} \
        ${LIBRARIES}

    echo "--- Build successful! ---"
    echo "Executable is at: ${BUILD_DIR}/incursion/${INCURSION_EXE_NAME}"
}

################################################################################
# SCRIPT MAIN LOGIC
################################################################################

# Check for required tools
command_exists ${CXX} || { echo "${CXX} is not installed. Please install g++."; exit 1; }
command_exists pkg-config || { echo "pkg-config is not installed. Please install it."; exit 1; }
command_exists ar || { echo "ar (from binutils) is not installed. Please install build-essential."; exit 1; }

build_dependencies
build_incursion

echo "All tasks complete."