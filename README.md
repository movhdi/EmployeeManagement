# 1. Prerequisites:
- **c++ compiler:** A modern c++ compiler supporting C++17 (Like GCC, Clang or MSVC)
- **cmake:** Install CMake (version 3.11 or higher) from [cmake.org](cmake.org) or via your system's package manager (e.g., <pre>```sudo apt update && sudo apt install cmake build-essential```</pre> on Debian/Ubuntu, <pre>```brew install cmake```</pre> on macOS).
- SQLite3 Development Libraries: You need the SQLite3 library and its development headers.
    - **Debian/Ubuntu:** ```sudo apt update && sudo apt install libsqlite3-dev```

    - **Fedora/CentOS/RHEL:** ```sudo dnf install sqlite-devel```

    - **macOS (using Homebrew):** ```brew install sqlite``` (this usually includes headers)

    - **Windows:** Download the precompiled binaries (DLL) and the development files (headers and .lib file) from the [SQLite Download Page](https://sqlite.org/download.html). You might need to configure CMake to find these files using ```CMAKE_PREFIX_PATH``` or by setting environment variables.
# 2. Get sqlite_modern_cpp:
- This header-only library has been included uing cmake fetch-content mechanism to get rid of the hassle of downloading the lates version and including in the ```./include``` folder.
# 4. Build steps
- Open a terminal or command prompt.
- Navigate to the root directory of your project.
- Create a build directory and navigate into it:
<pre>
mkdir build
cd build/
</pre>
- Run CMake to configure the project. CMake will find SQLite3 and generate the build files (e.g., Makefiles on Linux/macOS, Visual Studio solution on Windows).
<pre>
cmake ..
</pre>
- compile the project 
- **Linux/macOS (with Makefiles):**
<pre>
make 
</pre>
- Using CMake's build tool abstraction (works generally):
<pre>
cmake -build . 
</pre>

# 5. **Run:**
- The executable (e.g., performance_manager ) will be located in the build directory.

- Run it from the build directory:
<pre>
./EmployeePerformanceManager 
</pre>


# 6. Naming convention
| Element | Style | Example |
|--------|-------|---------|
| Class/Struct Names | `PascalCase` | `Employee`, `PerformanceReview` |
| Member Variables | `camelCase` or `m_camelCase` | `name`, `personnelCode` or `m_name` |
| Function Names | `camelCase` | `addEmployee()`, `getReviewById()` |
| Local Variables | `camelCase` | `reviewScore`, `managerId` |
| Enum Types | `PascalCase` | `EmployeeRole` |
| Enum Values | `PascalCase` or `SCREAMING_SNAKE_CASE` | `Manager`, `HR`, `ROLE_HR` |
| SQL Table Names | `snake_case` | `employees`, `performance_reviews` |
| SQL Column Names | `snake_case` | `employee_id`, `hire_date`, `reports_to` |
| Constants | `SCREAMING_SNAKE_CASE` | `MAX_REVIEW_SCORE` |