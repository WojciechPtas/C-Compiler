#!/usr/bin/env bash
set -eu

mkdir -p llvm
cd llvm

CUR=`pwd`

if [ ! -e  "${CUR}/llvm" ]; then
        wget http://releases.llvm.org/7.0.0/llvm-7.0.0.src.tar.xz
        tar xf llvm-7.0.0.src.tar.xz
        rm llvm-7.0.0.src.tar.xz
        mv llvm-7.0.0.src llvm

        # uncomment this to also build clang
        # cd llvm/tools
        # wget http://releases.llvm.org/7.0.0/cfe-7.0.0.src.tar.xz
        # tar xf cfe-7.0.0.src.tar.xz
        # rm cfe-7.0.0.src.tar.xz
        # mv cfe-7.0.0.src clang

        cd ${CUR}
fi

mkdir -p build
cd build
cmake ../llvm \
        -DCMAKE_BUILD_TYPE:STRING=Debug \
        -DCMAKE_INSTALL_PREFIX:PATH="${CUR}/install" \
        -DLLVM_ENABLE_ASSERTIONS:BOOL=ON \
        -DLLVM_ENABLE_EH:BOOL=ON \
        -DLLVM_ENABLE_RTTI:BOOL=ON \
        -DLLVM_INCLUDE_TESTS:BOOL=OFF \
        -DLLVM_TARGETS_TO_BUILD="X86"

# In the above command,
# "-DCMAKE_BUILD_TYPE:STRING=Debug" enables debug information for this build,
#       this simplifies debugging and provides better backtraces, but increases
#       memory consumption of the build process as well as disk space
#       consumption of the result significantly.
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
# "-DLLVM_TARGETS_TO_BUILD="X86"" restricts the LLVM build to only create
#       backend code for the X86 backend. As backends tend to be huge, this
#       improves build times and memory as well as disk space consumption.

make install

# You can replace the above command with the following one to use more threads
# for building LLVM (this increases the memory consumption significantly).
# make -j2 install

