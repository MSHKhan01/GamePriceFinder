cmake_minimum_required(VERSION 3.10)

# Define the project name
project(GamePriceFinder C)

# Set the compiler flags (e.g., enable all warnings and treat warnings as errors)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Werror")

# ------------------------------------------------
# 1. Dependency: Third-Party Library (libcsv)
# ------------------------------------------------
# For the purpose of this exercise, we assume libcsv can be found.
# The implementation uses manual parsing for demonstration.
find_package(LibCSV QUIET)
if (LibCSV_FOUND)
    message(STATUS "Found LibCSV: ${LibCSV_INCLUDE_DIRS}")
    set(CSV_LINK_LIBS ${LibCSV_LIBRARIES})
else()
    message(WARNING "LibCSV not found. Proceeding with manual parsing implementation.")
    set(CSV_LINK_LIBS "")
endif()

# ------------------------------------------------
# 2. Main Executable Target
# ------------------------------------------------
add_executable(game_finder 
    main.c 
    game_finder.c
)
target_link_libraries(game_finder PUBLIC ${CSV_LINK_LIBS})

# ------------------------------------------------
# 3. Unit Test Executable Target
# ------------------------------------------------
enable_testing()

# Create a placeholder source file for the test runner.
# This simulates including the Unity test framework's runner.
file(WRITE tests/unity_test_runner.c 
"
#include \"unity.h\"
#include \"../game_finder.h\"

extern void setUp(void);
extern void tearDown(void);
extern void test_find_max_price_standard_case(void);
extern void test_find_max_price_tie(void);
extern void test_find_max_price_invalid_data(void);

int main(void) {
    UnityBegin(\"test_game_finder.c\");
    RUN_TEST(test_find_max_price_standard_case);
    RUN_TEST(test_find_max_price_tie);
    RUN_TEST(test_find_max_price_invalid_data);
    return UnityEnd();
}
")

add_executable(game_finder_tests 
    tests/unity_test_runner.c
    tests/test_game_finder.c
    game_finder.c 
)

target_link_libraries(game_finder_tests PUBLIC ${CSV_LINK_LIBS})

# 4. Define the test for CTest
add_test(NAME MaxPriceFinderTests COMMAND game_finder_tests)