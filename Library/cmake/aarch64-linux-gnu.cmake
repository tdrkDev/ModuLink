set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR aarch64)

set(CMAKE_C_COMPILER aarch64-linux-gnu-gcc)
set(CMAKE_CXX_COMPILER aarch64-linux-gnu-g++)
set(CMAKE_CXX_COMPILER_TARGET aarch64-linux-gnu)
set(CMAKE_C_COMPILER_TARGET aarch64-linux-gnu)
set(CMAKE_ASM_COMPILE_TARGET aarch64-linux-gnu)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-address-of-packed-member")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -lpthread")
