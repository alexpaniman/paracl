cmake_minimum_required(VERSION 3.29)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


project(paracl)

# Colored diagnostics gets automatically disabled when cmake
# uses Ninja as a build generator, because Ninja creates
# a pipe internally and messes up isatty, which is not nice.

# This options is a mitigation for this inconvenience.
option(FORCE_COLORED_OUTPUT "Always produce ANSI-colored output (GNU/Clang only)." FALSE)
if (${FORCE_COLORED_OUTPUT})
    if     ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
        add_compile_options (-fdiagnostics-color=always)
    elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
        add_compile_options (-fcolor-diagnostics)
    endif ()
endif ()


add_subdirectory(vendor)

add_compile_options(-Wall -Wextra -Werror)


include(CTest)
enable_testing()


set_property(GLOBAL PROPERTY paracl_tests_property "")

macro(add_paracl_library target_name)
    cmake_parse_arguments(ARG "" "TOOL" "SOURCES;LIBRARIES;TESTS" ${ARGN})

    if(NOT DEFINED ARG_LIBRARIES AND ARG_LIBRARIES STREQUAL "")
        message(FATAL_ERROR "SOURCES have to be provided for target ${target_name}")
    endif()

    add_library(${target_name} STATIC ${ARG_SOURCES})
    target_include_directories(${target_name} PUBLIC "${PROJECT_SOURCE_DIR}/include/")
    target_include_directories(${target_name} PRIVATE "${PROJECT_SOURCE_DIR}/src/${target_name}")

    if(DEFINED ARG_LIBRARIES AND NOT ARG_LIBRARIES STREQUAL "")
        foreach(lib IN LISTS ARG_LIBRARIES)
            target_link_libraries(${target_name} PUBLIC ${lib})
        endforeach()
    endif()

    if(DEFINED ARG_TESTS AND NOT ARG_TESTS STREQUAL "")
        foreach(test IN LISTS ARG_TESTS)
            get_filename_component(test_name ${test} NAME_WE)
            set(test_name "${target_name}-${test_name}")

            add_executable(${test_name} "${PROJECT_SOURCE_DIR}/tests/${target_name}/${test}")
            target_link_libraries(${test_name} PRIVATE catch2)
            target_link_libraries(${test_name} PRIVATE ${target_name})

            add_test(NAME ${test_name} COMMAND ${test_name})

            get_property(paracl_tests GLOBAL PROPERTY paracl_tests_property)
            list(APPEND paracl_tests ${test_name})

            set_property(GLOBAL PROPERTY paracl_tests_property "${paracl_tests}")
        endforeach()
    endif()

    if(DEFINED ARG_TOOL AND NOT ARG_TOOL STREQUAL "")
        set(tool_name "cli-${target_name}")

        add_executable("${tool_name}" ${ARG_TOOL})
        target_link_libraries("${tool_name}" PRIVATE ${target_name})

        install(TARGETS "${tool_name}" DESTINATION bin)
    endif()
endmacro()


add_subdirectory(src)


list(APPEND CMAKE_CTEST_ARGUMENTS "--output-on-failure")

get_property(paracl_tests GLOBAL PROPERTY paracl_tests_property)
add_custom_target(check
  COMMAND ${CMAKE_CTEST_COMMAND}
    --force-new-ctest-process
    --output-on-failure
    --rerun-failed
    --build-config "$<CONFIGURATION>"
  DEPENDS ${paracl_tests}
)

