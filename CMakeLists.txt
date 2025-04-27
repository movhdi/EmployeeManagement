# Minimum CMake version required
cmake_minimum_required(VERSION 3.10)

# Project name and version
project(EmployeePerformanceManager LANGUAGES CXX)


# --- Set the C++ standard ---
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# --- Include FetchContent module ---
include(FetchContent)

# --- Declare sqlite_modern_cpp dependency ---
# Specifies where to get the library from
FetchContent_Declare(
  sqlite_modern_cpp
  GIT_REPOSITORY https://github.com/SqliteModernCpp/sqlite_modern_cpp.git
  GIT_TAG        v3.2  # Use a specific tag or commit hash for stability
                         # Check the repo for the latest stable tag
)


# --- Make sqlite_modern_cpp available ---
# Downloads the source code into the build directory (e.g., build/_deps/sqlite_modern_cpp-src)
# and makes variables like sqlite_modern_cpp_SOURCE_DIR available.
FetchContent_MakeAvailable(sqlite_modern_cpp)

# --- Find SQLite3 ---
find_package(SQLite3 REQUIRED)
if(SQLite3_FOUND)
    message(STATUS "SQLite3 found: ${SQLite3_LIBRARIES}")
else()
    message(FATAL_ERROR "SQLite3 not found")
endif()

# Define the executable name
set(EXECUTABLE_NAME EmployeePerformanceManager)

add_executable(${EXECUTABLE_NAME} 
    src/main.cpp
    src/DatabaseManager.cpp
    src/test.cpp
)

# --- Include Directories ---
# Tell CMake where to find header files
target_include_directories(${EXECUTABLE_NAME} PRIVATE
    include # Our project's include directory
    ${SQLite3_INCLUDE_DIRS} # SQLite3 header directory found by find_package
)

# --- Include Directories ---
# Tell CMake where to find header files
target_include_directories(${EXECUTABLE_NAME} PRIVATE
    include # Our project's include directory
    # Add the header directory from the fetched content for sqlite_modern_cpp
    ${sqlite_modern_cpp_SOURCE_DIR}/hdr
    # Add the include directory for the base SQLite3 C library
    ${SQLite3_INCLUDE_DIRS}
)


# --- Link Libraries ---
# Link the SQLite3 C library to our executable
# sqlite_modern_cpp is header-only, so no separate linking needed for it,
# but our code using it needs the underlying C library.
target_link_libraries(${EXECUTABLE_NAME} PRIVATE
    SQLite::SQLite3 # CMake target for SQLite3 C library
)


install(TARGETS ${EXECUTABLE_NAME}
     DESTINATION bin
)

# Print configuration info (optional)
message(STATUS "Project Name: ${PROJECT_NAME}")
message(STATUS "CXX Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "SQLite3 Include Dirs: ${SQLite3_INCLUDE_DIRS}")
message(STATUS "SQLite3 Libraries: ${SQLite3_LIBRARIES}")
message(STATUS "Build Type: ${CMAKE_BUILD_TYPE}") # Debug, Release, etc.