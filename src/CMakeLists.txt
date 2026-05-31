include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

add_library(Aspartame INTERFACE)
target_include_directories(Aspartame INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
target_compile_features(Aspartame INTERFACE cxx_std_17)

add_library(Aspartame::Aspartame ALIAS Aspartame)

install(TARGETS Aspartame EXPORT AspartameTargets)

install(DIRECTORY aspartame
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
        FILES_MATCHING PATTERN "*.hpp")

install(EXPORT AspartameTargets
        FILE AspartameTargets.cmake
        NAMESPACE Aspartame::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Aspartame)

write_basic_package_version_file(
        "${CMAKE_CURRENT_BINARY_DIR}/AspartameConfigVersion.cmake"
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY SameMajorVersion)

configure_package_config_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/AspartameConfig.cmake.in"
        "${CMAKE_CURRENT_BINARY_DIR}/AspartameConfig.cmake"
        INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Aspartame)

install(FILES
        "${CMAKE_CURRENT_BINARY_DIR}/AspartameConfig.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/AspartameConfigVersion.cmake"
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Aspartame)
