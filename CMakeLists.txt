cmake_minimum_required(VERSION 3.12 FATAL_ERROR)
project(optional_playground CXX)

Include(FetchContent)

FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG v3.4.0 # or a later release
)

FetchContent_MakeAvailable(Catch2)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_VERBOSE_MAKEFILE  ON)

set(COMPILE_FLAGS
        #        -Wno-unused-parameter
        -Wall
        -Wextra
        -pedantic
        -O0
        -fsanitize=address -fsanitize=undefined
)
set(LINK_FLAGS
        -fsanitize=address -fsanitize=undefined
)


add_executable(tests
        test/test_view.cpp
#        test/test_vector.cpp
#        test/test_optional.cpp
#        test/test_list.cpp
#        test/test_deque.cpp
#        test/test_set.cpp
#        test/test_unordered_set.cpp
#        test/test_map.cpp
#        test/test_string.cpp
#        test/test_unordered_map.cpp
)

#test/test_array.cpp


target_include_directories(tests PRIVATE src)
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)
target_compile_options(tests PRIVATE ${COMPILE_FLAGS})
target_link_options(tests PRIVATE ${LINK_FLAGS})

add_executable(example example.cpp)
target_compile_options(example PRIVATE ${COMPILE_FLAGS})
target_link_options(example PRIVATE ${LINK_FLAGS})



