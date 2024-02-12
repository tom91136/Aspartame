cmake_minimum_required(VERSION 3.12 FATAL_ERROR)
project(Aspartame CXX)

if (CMAKE_VERSION VERSION_GREATER_EQUAL "3.24.0")
    cmake_policy(SET CMP0135 NEW)
endif ()

include(FetchContent)

FetchContent_Declare(
        Catch2
        URL https://github.com/catchorg/Catch2/archive/refs/tags/v3.5.2.tar.gz
        URL_HASH SHA1=ce9613c9b25803a5b052fb75b71a6e14f1e95eb8
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)

option(ENABLE_SAN "Enable ASan and UBSan for Clang/GCC (not available for MSVC)" OFF)

if (MSVC)
    set(COMPILE_FLAGS /bigobj)
else ()
    set(COMPILE_FLAGS
            #        -Wno-unused-parameter
            -Wall
            -Wextra
            $<$<CXX_COMPILER_ID:Clang,GNU>:-pedantic>
            $<$<CXX_COMPILER_ID:PGI,NVHPC>:--pedantic>

            $<$<CXX_COMPILER_ID:Clang,GNU>:-march=native>
    )
    set(LINK_FLAGS)
    if (ENABLE_SAN)
        list(APPEND COMPILE_FLAGS -fsanitize=address -fsanitize=undefined)
        list(APPEND LINK_FLAGS -fsanitize=address -fsanitize=undefined)
    endif ()
endif ()

FetchContent_MakeAvailable(Catch2)

add_executable(tests
        test/test_view_id.cpp
        test/test_view_map.cpp
        test/test_view_filter.cpp
        test/test_view_bind.cpp
        test/test_view_slice.cpp
        test/test_view_collect.cpp
        test/test_view_concat.cpp

        test/test_view_ownership.cpp


        test/test_vector.cpp
        test/test_optional.cpp
        test/test_list.cpp
        test/test_deque.cpp
        test/test_set.cpp
        test/test_unordered_set.cpp
        test/test_map.cpp
        test/test_string.cpp
        test/test_unordered_map.cpp

        #test/test_array.cpp

)
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)

add_executable(benchmarks
        benchmark/numeric.cpp
)
target_link_libraries(benchmarks PRIVATE Catch2::Catch2WithMain)
target_compile_features(benchmarks PRIVATE cxx_std_20)

add_executable(example example.cpp)


foreach (TARGET "tests" "benchmarks" "example")
    target_include_directories(${TARGET} PRIVATE src)
    target_compile_options(${TARGET} PRIVATE ${COMPILE_FLAGS})
    target_link_options(${TARGET} PRIVATE ${LINK_FLAGS})
endforeach ()
