cmake_minimum_required(VERSION 3.12 FATAL_ERROR)

if (NOT DEFINED PROJECT_NAME)
    set(NOT_SUBPROJECT ON)
else ()
    set(NOT_SUBPROJECT OFF)
endif ()


project(Aspartame
        VERSION 0.0.1
        LANGUAGES CXX
)

if (CMAKE_VERSION VERSION_GREATER_EQUAL "3.24.0")
    cmake_policy(SET CMP0135 NEW)
endif ()

option(ASPARTAME_ENABLE_TEST "Enable unit test project" OFF)
option(ASPARTAME_ENABLE_BENCHMARK "Enable benchmark project" OFF)
option(ASPARTAME_ENABLE_EXAMPLE "Enable example project" OFF)
option(ASPARTAME_ENABLE_SAN "Enable ASan and UBSan for Clang and GCC (not available for MSVC), this is for executable targets (e.g test) only" OFF)

add_subdirectory(src)

if (ASPARTAME_ENABLE_TEST OR ASPARTAME_ENABLE_BENCHMARK OR ASPARTAME_ENABLE_EXAMPLE)


    include(FetchContent)
    FetchContent_Declare(
            Catch2
            URL https://github.com/catchorg/Catch2/archive/refs/tags/v3.5.2.tar.gz
            URL_HASH SHA1=ce9613c9b25803a5b052fb75b71a6e14f1e95eb8
    )
    if (MSVC)
        set(ASPARTAME_COMPILE_FLAGS /bigobj)
    else ()
        set(ASPARTAME_COMPILE_FLAGS
                #        -Wno-unused-parameter
                -Wall
                -Wextra
                $<$<CXX_COMPILER_ID:Clang,GNU>:-pedantic>
                $<$<CXX_COMPILER_ID:PGI,NVHPC>:--pedantic>

                $<$<CXX_COMPILER_ID:Clang,GNU>:-march=native>
        )
        if (ASPARTAME_ENABLE_SAN)
            list(APPEND ASPARTAME_COMPILE_FLAGS -fsanitize=address -fsanitize=undefined)
            list(APPEND ASPARTAME_LINK_FLAGS -fsanitize=address -fsanitize=undefined)
        endif ()
    endif ()
    FetchContent_MakeAvailable(Catch2)


    if (ASPARTAME_ENABLE_TEST)
        add_executable(test
                test/test_view_id.cpp
                test/test_view_map.cpp
                test/test_view_filter.cpp
                test/test_view_bind.cpp
                test/test_view_slice.cpp
                test/test_view_collect.cpp
                test/test_view_concat.cpp
                test/test_view_ownership.cpp

                test/test_view_custom_ptr_iterator.cpp

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
        target_link_libraries(test PRIVATE Aspartame Catch2::Catch2WithMain)
        target_compile_options(test PRIVATE ${ASPARTAME_COMPILE_FLAGS})
        target_link_options(test PRIVATE ${ASPARTAME_LINK_FLAGS})
    endif ()

    if (ASPARTAME_ENABLE_BENCHMARK)
        add_executable(benchmark
                benchmark/numeric.cpp
        )
        target_link_libraries(benchmark PRIVATE Aspartame Catch2::Catch2WithMain)
        target_compile_features(benchmark PRIVATE cxx_std_20)
        target_compile_options(benchmark PRIVATE ${ASPARTAME_COMPILE_FLAGS})
        target_link_options(benchmark PRIVATE ${ASPARTAME_LINK_FLAGS})
    endif ()

    if (ASPARTAME_ENABLE_EXAMPLE)
        add_executable(example example.cpp)
        target_compile_options(example PRIVATE ${ASPARTAME_COMPILE_FLAGS})
        target_link_options(example PRIVATE ${ASPARTAME_LINK_FLAGS})
    endif ()
endif ()
