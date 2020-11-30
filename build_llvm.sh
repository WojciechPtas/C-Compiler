#!/usr/bin/env bash
set -eu

mkdir -p llvm
cd llvm

CUR=`pwd`

if [ ! -e  "${CUR}/llvm" ]; then
        wget https://github.com/llvm/llvm-project/releases/download/llvmorg-11.0.0/llvm-project-11.0.0.tar.xz
        tar xf llvm-project-11.0.0.tar.xz
        rm llvm-project-11.0.0.tar.xz
        mv llvm-project-11.0.0 llvm

        cd ${CUR}
fi

mkdir -p build
mkdir -p install
cd build
cmake ../llvm/llvm \
        -DLLVM_ENABLE_PROJECTS='clang' \
        -DCMAKE_BUILD_TYPE:STRING=Debug \
        -DCMAKE_INSTALL_PREFIX:PATH="${CUR}/install" \
        -DLLVM_ENABLE_ASSERTIONS:BOOL=ON \
        -DLLVM_ENABLE_EH:BOOL=ON \
        -DLLVM_ENABLE_RTTI:BOOL=ON \
        -DLLVM_INCLUDE_TESTS:BOOL=OFF \
        -DLLVM_USE_SPLIT_DWARF:BOOL=ON \
        -DLLVM_TARGETS_TO_BUILD="X86"


# In the above command,
# "-DLLVM_ENABLE_PROJECTS='clang'" enables building clang, LLVM's C/C++
#       frontend. Remove this line to save memory and disk space.
# "-DCMAKE_BUILD_TYPE:STRING=Debug" enables debug information for this build.
#       This is useful for debugging and for better backtraces, at the cost of
#       a larger memory consumption of the build process as well as disk space
#       consumption of the result.
#       Replace this with "-DCMAKE_BUILD_TYPE:STRING=Release" to sacrifice
#       usability for reduced memory requirements.
# "-DLLVM_ENABLE_ASSERTIONS:BOOL=ON" enables assertions that ensure validity
#       constraints in the LLVM code, you should never disable them for
#       development!
# "-DLLVM_ENABLE_EH:BOOL=ON" enables exception handling. LLVM does not use
#       exceptions internally, but if you want to use them in your code, you
#       need this.
# "-DLLVM_ENABLE_RTTI:BOOL=ON" enables run-time type information, that is
#       required for C++ constructs that refer to the dynamic type of objects,
#       e.g. dynamic_cast<>. LLVM by itself does not require this since they
#       implement their own leight weight type information for certain classes
#       for efficiency reasons.
# "-DLLVM_USE_SPLIT_DWARF:BOOL=ON" improves the compile-time and on-disk memory
#       consumption of debug builds by storing debug information separately
#       from the binaries.
# "-DLLVM_TARGETS_TO_BUILD="X86"" restricts the LLVM build to only create
#       backend code for the X86 backend. As backends tend to be huge, this
#       improves build times and memory as well as disk space consumption.

make install

# You can replace the above command with the following one to use more threads
# for building LLVM (this might increase the memory consumption significantly).
# make -j2 install
