function(add_project)
    set(prefix ADD_PROJECT)
    set(options LIBRARY SHARED STATIC EXECUTABLE LOG)
    set(oneValueArgs TARGET DIRECTORY)
    set(multyValueArgs INCLUDE LINK DEFINE SOURCES)
    cmake_parse_arguments(${prefix} "${options}" "${oneValueArgs}" "${multyValueArgs}" ${ARGN})

    if (${${prefix}_LOG})
        message("${prefix}_KEYWORDS_MISSING_VALUES = ${${prefix}_KEYWORDS_MISSING_VALUES}")
        message("${prefix}_LIBRARY = ${${prefix}_LIBRARY}")
        message("${prefix}_SHARED = ${${prefix}_SHARED}")
        message("${prefix}_STATIC = ${${prefix}_STATIC}")
        message("${prefix}_EXECUTABLE = ${${prefix}_EXECUTABLE}")
        message("${prefix}_LOG = ${${prefix}_LOG}")
        message("${prefix}_TARGET = ${${prefix}_TARGET}")
        message("${prefix}_DIRECTORY = ${${prefix}_DIRECTORY}")
        message("${prefix}_INCLUDE = ${${prefix}_INCLUDE}")
        message("${prefix}_LINK = ${${prefix}_LINK}")
        message("${prefix}_DEFINE = ${${prefix}_DEFINE}")
        message("${prefix}_SOURCES = ${${prefix}_SOURCES}")
    endif()

    if (${${prefix}_EXECUTABLE} AND ${${prefix}_LIBRARY})
        message(FATAL_ERROR "Trying to make executable library, select EXECUTABLE or LIBRARY!")
    endif()

    if(NOT ${${prefix}_DIRECTORY} STREQUAL "")
        file(GLOB_RECURSE ${${prefix}_TARGET}_SOURCES
            ${${prefix}_DIRECTORY}/*.h
            ${${prefix}_DIRECTORY}/*.c
            ${${prefix}_DIRECTORY}/*.hpp
            ${${prefix}_DIRECTORY}/*.cpp
        )
        if (${${prefix}_LOG})
            message("${${prefix}_TARGET}_SOURCES = ${${${prefix}_TARGET}_SOURCES}")
        endif()
    endif()

    if (${${prefix}_EXECUTABLE})
        add_executable(${${prefix}_TARGET} ${${${prefix}_TARGET}_SOURCES} ${${prefix}_SOURCES})
    endif()

    if (${${prefix}_LIBRARY})
        if(NOT ${${prefix}_STATIC} AND NOT ${${prefix}_SHARED})
            message(FATAL_ERROR "Trying make unknown library, select STATIC or SHARED")
        endif()
        if(${${prefix}_STATIC} AND ${${prefix}_SHARED})
            message(FATAL_ERROR "Trying make static shared library, select STATIC or SHARED")
        endif()

        if (${${prefix}_STATIC})
            set(_lib_type STATIC)
        elseif(${${prefix}_SHARED})
            set(_lib_type SHARED)
        endif()

        add_library(${${prefix}_TARGET} ${_lib_type} ${${${prefix}_TARGET}_SOURCES} ${${prefix}_SOURCES})
    endif()

    target_include_directories(${${prefix}_TARGET} PUBLIC ${${prefix}_DIRECTORY})

    foreach(includer ${${prefix}_INCLUDE})
        target_include_directories(${${prefix}_TARGET} PUBLIC ${includer})
    endforeach()

    foreach(linker ${${prefix}_LINK})
        target_link_libraries(${${prefix}_TARGET} PUBLIC ${linker})
    endforeach()

    foreach(definer ${${prefix}_DEFINE})
        target_compile_definitions(${${prefix}_TARGET} PUBLIC ${definer})
    endforeach()
    
    unset(prefix)
    unset(options)
    unset(oneValueArgs)
    unset(multyValueArgs)
endfunction()