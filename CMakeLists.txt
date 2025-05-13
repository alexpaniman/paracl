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
set_property(GLOBAL PROPERTY paracl_fuzz_property "")
if (NOT CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    message(WARNING "Fuzz tests are disabled, they don't work with any compilers but Clang")
endif()


macro(add_paracl_library target_name)
    cmake_parse_arguments(ARG "" "TOOL" "SOURCES;LIBRARIES;TESTS;FUZZ" ${ARGN})

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

    if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        if(DEFINED ARG_FUZZ AND NOT ARG_FUZZ STREQUAL "")
            foreach(fuzz IN LISTS ARG_FUZZ)
                get_filename_component(fuzz_name ${fuzz} NAME_WE)

                add_executable(${fuzz_name} "${PROJECT_SOURCE_DIR}/fuzz/${target_name}/${fuzz}")
                target_compile_options(${fuzz_name} PRIVATE -g3 -fsanitize=fuzzer,address,undefined,leak)
                target_link_libraries(${fuzz_name} PRIVATE -fsanitize=fuzzer,address,undefined,leak)
                target_link_libraries(${fuzz_name} PRIVATE ${target_name})

                get_property(paracl_fuzz GLOBAL PROPERTY paracl_fuzz_property)
                list(APPEND paracl_fuzz ${fuzz_name})

                set_property(GLOBAL PROPERTY paracl_fuzz_property "${paracl_fuzz}")
            endforeach()
        endif()
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
    --build-config "$<CONFIGURATION>"
  DEPENDS ${paracl_tests}
)

