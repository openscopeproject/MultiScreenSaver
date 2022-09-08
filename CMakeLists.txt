cmake_minimum_required(VERSION 3.2 FATAL_ERROR)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
cmake_policy(SET CMP0074 OLD)
cmake_policy(SET CMP0057 NEW)
cmake_policy(SET CMP0091 NEW)  # to enable CMAKE_MSVC_RUNTIME_LIBRARY

add_definitions(-DwxUSE_GRAPHICS_CONTEXT)
add_definitions(-D_CRT_SECURE_NO_WARNINGS)
add_definitions(-DUNICODE -D_UNICODE)
add_definitions(-D_USE_MATH_DEFINES)
# Hide Windows's min() and max() macros
add_definitions( -DNOMINMAX )
# source and execution charset are UTF-8
string( APPEND CMAKE_CXX_FLAGS " /utf-8" )
# C4290: throw() is interpreted as declspec(nothrow)
string( APPEND CMAKE_CXX_FLAGS " /wd4290" )
# C4800: non-bool is explicitly cast to bool, forcing value of 0 or 1
string( APPEND CMAKE_CXX_FLAGS " /wd4800" )
# /Zi: create PDB
# string( APPEND CMAKE_CXX_FLAGS " /Zi" )

# /GF: enable string pooling
string( APPEND CMAKE_CXX_FLAGS_RELEASE " /GF" )
string( APPEND CMAKE_CXX_FLAGS_RELWITHDEBINFO " /GF" )
# /Gy: Enable Function-Level Linking
string( APPEND CMAKE_CXX_FLAGS_RELEASE " /Gy" )
string( APPEND CMAKE_CXX_FLAGS_RELWITHDEBINFO " /Gy" )
# /EHsc: Enable exception unwind semantics
string( APPEND CMAKE_CXX_FLAGS " /EHsc" )

# /MT: static linking
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

project(photoscreensaver)

set(wxWidgets_CONFIGURATION mswu)
set(wxWidgets_USE_STATIC ON)
find_package(wxWidgets COMPONENTS core base adv REQUIRED)
include(${wxWidgets_USE_FILE})

set(APP_SRC
    main.cpp
    saverframe.cpp
)
add_subdirectory("gui")
set(CMAKE_EXECUTABLE_SUFFIX .scr)

add_executable(${PROJECT_NAME} WIN32 ${APP_SRC} ${APP_RESOURCES})
target_link_libraries(${PROJECT_NAME} GUI)
target_link_libraries(${PROJECT_NAME} ${wxWidgets_LIBRARIES})

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION ".")
