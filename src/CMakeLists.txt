add_library(Aspartame INTERFACE)
target_include_directories(Aspartame INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_features(Aspartame INTERFACE cxx_std_17)

add_library(Aspartame::Aspartame ALIAS Aspartame)
