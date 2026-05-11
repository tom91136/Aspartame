cmake_minimum_required(VERSION 3.14)
project(AspartameExample CXX)

include(FetchContent)

# Default to the in-tree source so this builds in CI and from a fresh checkout.
# Override ASPARTAME_GIT_REPOSITORY / ASPARTAME_GIT_TAG to fetch from a remote.
if (DEFINED ASPARTAME_GIT_REPOSITORY)
    FetchContent_Declare(
            Aspartame
            GIT_REPOSITORY ${ASPARTAME_GIT_REPOSITORY}
            GIT_TAG ${ASPARTAME_GIT_TAG}
    )
else ()
    FetchContent_Declare(
            Aspartame
            SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/..
    )
endif ()
FetchContent_MakeAvailable(Aspartame)

add_executable(example example.cpp)
target_link_libraries(example PRIVATE Aspartame)
target_compile_features(example PRIVATE cxx_std_17)
