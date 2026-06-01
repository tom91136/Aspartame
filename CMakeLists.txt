cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

project(Aspartame
        VERSION 0.0.1
        LANGUAGES CXX
)

if (CMAKE_VERSION VERSION_GREATER_EQUAL "3.24.0")
    cmake_policy(SET CMP0135 NEW)
endif ()

option(ASPARTAME_ENABLE_TEST "Enable unit test project" OFF)
option(ASPARTAME_ENABLE_BENCHMARK "Enable benchmark project" OFF)
option(ASPARTAME_ENABLE_SAN "Enable ASan and UBSan for Clang and GCC (not available for MSVC), this is for executable targets (e.g test) only" OFF)
option(ASPARTAME_TEST_EXT_LLVM "Build the test_ext_llvm demo which opts LLVM ADT types into Aspartame (requires LLVM dev headers; not built by default)" OFF)
option(ASPARTAME_TEST_EXT_CONTAINERS "Build third-party container adapter demos (phmap, tsl, boost, tl_expected - header-only deps)" OFF)
option(ASPARTAME_TEST_EXT_ABSEIL "Build the Abseil container adapter demo (requires abseil-cpp-devel; links absl)" OFF)
option(ASPARTAME_TEST_EXT_QT "Build the Qt QMap/QHash adapter demo (requires qt6-qtbase-devel; links Qt6::Core)" OFF)
option(ASPARTAME_TEST_COMPILE_FAIL "Register ctest cases asserting that ill-formed operator^= usages fail to compile (uses ctest WILL_FAIL)" OFF)

add_subdirectory(src)

if (ASPARTAME_ENABLE_TEST OR ASPARTAME_ENABLE_BENCHMARK)

    include(FetchContent)
    FetchContent_Declare(
            Catch2
            URL https://github.com/catchorg/Catch2/archive/refs/tags/v3.15.0.tar.gz
            URL_HASH SHA1=e50cdd9097f28a492d75b6a8b78d0dc13d57dbc7
    )
    if (MSVC)
        set(ASPARTAME_COMPILE_FLAGS /bigobj /W4 /EHsc /Zc:__cplusplus /permissive-)
    else ()
        include(CheckCXXCompilerFlag)
        set(ASPARTAME_CLANG_OPT_FLAGS "")
        if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
            # XXX clang-22+ flags Catch2's use of __COUNTER__ as a C2y extension under -pedantic
            check_cxx_compiler_flag(-Wno-c2y-extensions ASPARTAME_HAS_WNO_C2Y_EXTENSIONS)
            if (ASPARTAME_HAS_WNO_C2Y_EXTENSIONS)
                list(APPEND ASPARTAME_CLANG_OPT_FLAGS -Wno-c2y-extensions)
            endif ()
            # XXX clang's -Wpass-failed sometimes fires for inlined loops in templates; not actionable
            check_cxx_compiler_flag(-Wno-pass-failed ASPARTAME_HAS_WNO_PASS_FAILED)
            if (ASPARTAME_HAS_WNO_PASS_FAILED)
                list(APPEND ASPARTAME_CLANG_OPT_FLAGS -Wno-pass-failed)
            endif ()
        endif ()
        set(ASPARTAME_COMPILE_FLAGS
                -Wno-unused-parameter
                -Wall
                -Wextra
                $<$<CXX_COMPILER_ID:Clang,GNU,AppleClang>:-Werror>
                $<$<CXX_COMPILER_ID:Clang,GNU,AppleClang>:-pedantic>
                ${ASPARTAME_CLANG_OPT_FLAGS}
                # GCC 12+ false-positives on std::optional<std::variant<...>> with -O2; clang is clean.
                $<$<CXX_COMPILER_ID:GNU>:-Wno-maybe-uninitialized>
                $<$<CXX_COMPILER_ID:PGI,NVHPC>:--pedantic>
        )
        if (ASPARTAME_ENABLE_SAN)
            list(APPEND ASPARTAME_COMPILE_FLAGS -fsanitize=address -fsanitize=undefined)
            list(APPEND ASPARTAME_LINK_FLAGS -fsanitize=address -fsanitize=undefined)
        endif ()
    endif ()
    FetchContent_MakeAvailable(Catch2)
    if (TARGET Catch2)
        get_target_property(_catch2_inc Catch2 INTERFACE_INCLUDE_DIRECTORIES)
        set_target_properties(Catch2 PROPERTIES INTERFACE_SYSTEM_INCLUDE_DIRECTORIES "${_catch2_inc}")
    endif ()


    if (ASPARTAME_ENABLE_TEST)
        set(ASPARTAME_TEST_STDS 17 20)
        if (cxx_std_23 IN_LIST CMAKE_CXX_COMPILE_FEATURES)
            list(APPEND ASPARTAME_TEST_STDS 23)
        endif ()
        foreach (std IN LISTS ASPARTAME_TEST_STDS)
            add_executable(test_${std}
                    test/test_spike_basic.cpp
                    test/test_variant.cpp
                    test/test_optional.cpp
                    test/test_vector.cpp
                    test/test_deque.cpp
                    test/test_list.cpp
                    test/test_set.cpp
                    test/test_unordered_set.cpp
                    test/test_map.cpp
                    test/test_unordered_map.cpp
                    test/test_string.cpp
                    test/test_array.cpp
                    test/test_view_id.cpp
                    test/test_view_map.cpp
                    test/test_view_filter.cpp
                    test/test_view_flat_map.cpp
                    test/test_view_slice.cpp
                    test/test_view_collect.cpp
                    test/test_view_concat.cpp
                    test/test_view_ownership.cpp
                    test/test_view_factory.cpp
                    test/test_view_custom_ptr_iterator.cpp
                    test/test_inplace.cpp
                    test/test_closure_lifetime.cpp
                    test/test_expected.cpp
                    test/test_compile_constexpr.cpp
            )
            target_compile_options(test_${std} PRIVATE ${ASPARTAME_COMPILE_FLAGS})
            # XXX Pre-PCH compilers (nvc++) don't see the ASPARTAME_SHOW_STREAM define from
            # test_base_includes.hpp before aspartame/all.hpp is parsed; set it here too.
            target_compile_definitions(test_${std} PRIVATE ASPARTAME_SHOW_STREAM)
            target_compile_features(test_${std} PRIVATE cxx_std_${std})
            target_link_libraries(test_${std} PRIVATE Aspartame Catch2::Catch2WithMain)
            target_link_options(test_${std} PRIVATE ${ASPARTAME_LINK_FLAGS})
            target_precompile_headers(test_${std} PRIVATE
                    "${CMAKE_CURRENT_SOURCE_DIR}/test/test_base_includes.hpp"
                    <catch2/catch_test_macros.hpp>
            )
        endforeach ()

        if (ASPARTAME_TEST_EXT_LLVM)
            # LLVMConfig pulls FindTerminfo which uses check_c_source_compiles.
            enable_language(C)
            find_package(LLVM REQUIRED CONFIG)
            message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION} at ${LLVM_DIR}")
            file(GLOB _ext_llvm_srcs CONFIGURE_DEPENDS
                    "${CMAKE_CURRENT_SOURCE_DIR}/test/test_ext_llvm.cpp"
                    "${CMAKE_CURRENT_SOURCE_DIR}/test/test_ext_llvm_*.cpp")
            foreach (std 17 20)
                add_executable(test_ext_llvm_${std} ${_ext_llvm_srcs})
                target_compile_options(test_ext_llvm_${std} PRIVATE ${ASPARTAME_COMPILE_FLAGS})
                target_compile_features(test_ext_llvm_${std} PRIVATE cxx_std_${std})
                target_include_directories(test_ext_llvm_${std} SYSTEM PRIVATE ${LLVM_INCLUDE_DIRS})
                target_compile_definitions(test_ext_llvm_${std} PRIVATE ${LLVM_DEFINITIONS_LIST} ASPARTAME_SHOW_STREAM)
                target_link_libraries(test_ext_llvm_${std} PRIVATE Aspartame Catch2::Catch2WithMain LLVMSupport)
                target_link_options(test_ext_llvm_${std} PRIVATE ${ASPARTAME_LINK_FLAGS})
            endforeach ()
        endif ()

        if (ASPARTAME_TEST_EXT_CONTAINERS)
            foreach (lib phmap tsl boost tl_expected)
                file(GLOB _ext_${lib}_srcs CONFIGURE_DEPENDS
                        "${CMAKE_CURRENT_SOURCE_DIR}/test/test_ext_${lib}.cpp"
                        "${CMAKE_CURRENT_SOURCE_DIR}/test/test_ext_${lib}_*.cpp")
                foreach (std 17 20)
                    add_executable(test_ext_${lib}_${std} ${_ext_${lib}_srcs})
                    target_compile_options(test_ext_${lib}_${std} PRIVATE ${ASPARTAME_COMPILE_FLAGS})
                    target_compile_definitions(test_ext_${lib}_${std} PRIVATE ASPARTAME_SHOW_STREAM)
                    target_compile_features(test_ext_${lib}_${std} PRIVATE cxx_std_${std})
                    target_link_libraries(test_ext_${lib}_${std} PRIVATE Aspartame Catch2::Catch2WithMain)
                    target_link_options(test_ext_${lib}_${std} PRIVATE ${ASPARTAME_LINK_FLAGS})
                endforeach ()
            endforeach ()
        endif ()

        if (ASPARTAME_TEST_EXT_ABSEIL)
            find_package(absl REQUIRED)
            file(GLOB _ext_abseil_srcs CONFIGURE_DEPENDS
                    "${CMAKE_CURRENT_SOURCE_DIR}/test/test_ext_abseil.cpp"
                    "${CMAKE_CURRENT_SOURCE_DIR}/test/test_ext_abseil_*.cpp")
            foreach (std 17 20)
                add_executable(test_ext_abseil_${std} ${_ext_abseil_srcs})
                target_compile_options(test_ext_abseil_${std} PRIVATE ${ASPARTAME_COMPILE_FLAGS})
                target_compile_definitions(test_ext_abseil_${std} PRIVATE ASPARTAME_SHOW_STREAM)
                target_compile_features(test_ext_abseil_${std} PRIVATE cxx_std_${std})
                target_link_libraries(test_ext_abseil_${std} PRIVATE Aspartame Catch2::Catch2WithMain
                        absl::flat_hash_map absl::flat_hash_set absl::btree absl::inlined_vector)
                target_link_options(test_ext_abseil_${std} PRIVATE ${ASPARTAME_LINK_FLAGS})
            endforeach ()
        endif ()

        if (ASPARTAME_TEST_EXT_QT)
            find_package(Qt6 REQUIRED COMPONENTS Core)
            file(GLOB _ext_qt_srcs CONFIGURE_DEPENDS
                    "${CMAKE_CURRENT_SOURCE_DIR}/test/test_ext_qt.cpp"
                    "${CMAKE_CURRENT_SOURCE_DIR}/test/test_ext_qt_*.cpp")
            foreach (std 17 20)
                add_executable(test_ext_qt_${std} ${_ext_qt_srcs})
                target_compile_options(test_ext_qt_${std} PRIVATE ${ASPARTAME_COMPILE_FLAGS})
                target_compile_definitions(test_ext_qt_${std} PRIVATE ASPARTAME_SHOW_STREAM)
                target_compile_features(test_ext_qt_${std} PRIVATE cxx_std_${std})
                target_link_libraries(test_ext_qt_${std} PRIVATE Aspartame Catch2::Catch2WithMain Qt6::Core)
                target_link_options(test_ext_qt_${std} PRIVATE ${ASPARTAME_LINK_FLAGS})
            endforeach ()
        endif ()

        if (ASPARTAME_TEST_COMPILE_FAIL)
            enable_testing()

            # Registers one ctest case per compile-fail target. `expect_fail` flips
            # the pass condition: the test passes iff the build command fails.
            function (aspartame_compile_case name ty op expect_fail)
                add_executable(${name} EXCLUDE_FROM_ALL test/test_compile_fail.cpp)
                target_compile_definitions(${name} PRIVATE ASPARTAME_TYPE=${ty} ASPARTAME_OP=${op})
                target_compile_features(${name} PRIVATE cxx_std_17)
                target_link_libraries(${name} PRIVATE Aspartame)
                add_test(NAME ${name}
                        COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target ${name} --config $<CONFIG>)
                set_tests_properties(${name} PROPERTIES WILL_FAIL ${expect_fail})
            endfunction ()

            # Matrix: {vector,deque,list,set,unordered_set}
            #       x {map-retype, fold, count, exists, mk_string, map-narrow}
            foreach (ty RANGE 0 4)
                foreach (op RANGE 0 5)
                    aspartame_compile_case(compile_fail_t${ty}_o${op} ${ty} ${op} TRUE)
                endforeach ()
            endforeach ()

            # Standalone must-fail cases.
            aspartame_compile_case(compile_fail_map_keys 0 10 TRUE)
            aspartame_compile_case(compile_fail_unregistered 0 11 TRUE)
            aspartame_compile_case(compile_fail_get_or_emplace_pipe 0 12 TRUE)
            aspartame_compile_case(compile_fail_map_values_retype 0 13 TRUE)

            # Positive control: must compile.
            aspartame_compile_case(compile_ok_inplace 0 99 FALSE)
        endif ()

    endif ()

    if (ASPARTAME_ENABLE_BENCHMARK)
        include(CheckCXXSourceCompiles)
        if (MSVC)
            set(CMAKE_REQUIRED_FLAGS "/std:c++20")
        else ()
            set(CMAKE_REQUIRED_FLAGS "-std=c++20")
        endif ()
        check_cxx_source_compiles("
            #include <ranges>
            int main() {
                for (auto x : std::ranges::iota_view{0, 10}
                              | std::views::transform([](int v) { return v * 2; })
                              | std::views::filter([](int v) { return v > 5; })
                              | std::views::take(3)) { (void)x; }
                return 0;
            }
        " ASPARTAME_HAS_CXX20_RANGES)
        unset(CMAKE_REQUIRED_FLAGS)

        if (ASPARTAME_HAS_CXX20_RANGES)
            foreach (bench numeric reductions pipelines inplace)
                add_executable(${bench} benchmark/${bench}.cpp)
                target_link_libraries(${bench} PRIVATE Aspartame Catch2::Catch2WithMain)
                target_compile_features(${bench} PRIVATE cxx_std_20)
                target_compile_options(${bench} PRIVATE ${ASPARTAME_COMPILE_FLAGS})
                target_link_options(${bench} PRIVATE ${ASPARTAME_LINK_FLAGS})
            endforeach ()
        else ()
            message(STATUS "Skipping benchmark targets: compiler/stdlib lacks C++20 std::ranges support")
        endif ()
    endif ()

endif ()
