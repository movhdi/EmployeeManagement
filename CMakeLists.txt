cmake_minimum_required(VERSION 3.14)

project(EmployeePerformanceManager VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# Try to find Qt6 first
find_package(Qt6 QUIET COMPONENTS Widgets LinguistTools Sql)
if(Qt6_FOUND)
    set(QT_VERSION_MAJOR 6)
    find_package(Qt6 REQUIRED COMPONENTS Widgets LinguistTools Sql)
else()
    set(QT_VERSION_MAJOR 5)
    find_package(Qt5 REQUIRED COMPONENTS Widgets LinguistTools Sql)
endif()

# Add include directory
include_directories(include)

# You may prefer the modern style:
# target_include_directories(EmployeePerformanceManager PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

set(PROJECT_SOURCES
    src/main.cpp
    src/DatabaseManager.cpp
    src/NetworkManager.cpp
    src/test.cpp
    src/Models.cpp

    include/Models.hpp
    include/DatabaseManager.hpp
    include/NetworkManager.hpp
    include/test.hpp
)

# If you have .ui files, list them here and make sure qt_wrap_ui or AUTOUIC handles them
# If not, remove AUTOUIC above

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(EmployeePerformanceManager
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
else()
    if(ANDROID)
        add_library(EmployeePerformanceManager SHARED ${PROJECT_SOURCES})
    else()
        add_executable(EmployeePerformanceManager ${PROJECT_SOURCES})
    endif()
endif()

target_link_libraries(EmployeePerformanceManager PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Sql)

# macOS/iOS bundle ID (safe to skip if you're not targeting Apple platforms)
# Only needed for Qt < 6.1, define explicitly to avoid CMake error
if(${QT_VERSION_MAJOR} LESS 6 OR Qt${QT_VERSION_MAJOR}_VERSION VERSION_LESS 6.1.0)
    set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.EmployeePerformanceManager)
endif()

# Only apply BUNDLE_ID_OPTION if it's defined
set_target_properties(EmployeePerformanceManager PROPERTIES
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    ${BUNDLE_ID_OPTION}
)

include(GNUInstallDirs)
install(TARGETS EmployeePerformanceManager
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(${QT_VERSION_MAJOR} EQUAL 6)
    qt_finalize_executable(EmployeePerformanceManager)
endif()
