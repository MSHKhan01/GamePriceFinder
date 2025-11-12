#include "unity.h"
#include "../game_finder.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TEST_FILE_PATH "temp_max_price_test.csv"

// --- Helper Functions for Test Environment ---

void setUp(void) {
    // Called before each test
}

void tearDown(void) {
    // Called after each test
    remove(TEST_FILE_PATH);
}

// Function to create the temporary CSV file for testing
void create_test_file(const char *data) {
    FILE *fp = fopen(TEST_FILE_PATH, "w");
    if (!fp) {
        TEST_FAIL_MESSAGE("Failed to create temporary test file!");
        return;
    }
    fprintf(fp, "%s", data);
    fclose(fp);
}

// --- Define the new long header and dummy fields for testing ---

// The new header must match the expected format (40+ fields)
const char *NEW_HEADER = "appid,name,release_date,required_age,price,dlc_count,detailed_description,about_the_game,short_description,reviews,header_image,website,support_url,support_email,windows,mac,linux,metacritic_score,metacritic_url,achievements,recommendations,notes,supported_languages,full_audio_languages,packages,developers,publishers,categories,genres,screenshots,movies,user_score,score_rank,positive,negative,estimated_owners,average_playtime_forever,average_playtime_2weeks,median_playtime_forever,median_playtime_2weeks,discount,peak_ccu,tags,pct_pos_total,num_reviews_total,pct_pos_recent,num_reviews_recent\n";

// Dummy fields to fill in fields 0, 2, 3, and all fields after 4
// Format: appid, name, release_date, required_age, price, <DUMMY DATA FOR REST>
#define DUMMY_PREFIX "100000," // appid (Field 0)
#define DUMMY_INFIX ",2024-01-01,0," // release_date, required_age (Fields 2 and 3)
#define DUMMY_SUFFIX ",0,\"desc\",\"about\",\"short desc\",\"reviews\",\"img\",\"web\",\"supp_url\",\"email\",1,1,1,90,\"meta_url\",0,0,\"notes\",\"lang\",\"audio\",0,\"dev\",\"pub\",\"cat\",\"gen\",\"screen\",\"movies\",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0\n" // Fields 5 onwards

// --- Unit Test Cases ---

// Test Case 1: Standard case - Max price in the middle
void test_find_max_price_standard_case(void) {
    // 1. SETUP: Create known data where "Massive Game" is the winner
    const char *test_data = 
        NEW_HEADER
        DUMMY_PREFIX "Low Price Game" DUMMY_INFIX "9.99" DUMMY_SUFFIX
        DUMMY_PREFIX "Massive Game" DUMMY_INFIX "999.99" DUMMY_SUFFIX  // Expected Max
        DUMMY_PREFIX "Mid Price Game" DUMMY_INFIX "59.99" DUMMY_SUFFIX;

    create_test_file(test_data); 

    // 2. EXECUTION: Call the function
    MaxPriceGame result = find_game_with_max_price(TEST_FILE_PATH);
    
    // 3. ASSERTION: Check the results
    TEST_ASSERT_FLOAT_WITHIN(0.001, 999.99, result.price);
    TEST_ASSERT_EQUAL_STRING("Massive Game", result.name);
}

// Test Case 2: Edge case - Highest price is tied (should return the first one found)
void test_find_max_price_tie(void) {
    const char *tie_data = 
        NEW_HEADER
        DUMMY_PREFIX "Game A" DUMMY_INFIX "75.00" DUMMY_SUFFIX // First max found
        DUMMY_PREFIX "Game B" DUMMY_INFIX "75.00" DUMMY_SUFFIX // Tied price
        DUMMY_PREFIX "Game C" DUMMY_INFIX "45.00" DUMMY_SUFFIX;
    
    create_test_file(tie_data);
    MaxPriceGame result = find_game_with_max_price(TEST_FILE_PATH);

    // We assert it returns the *first* game encountered at that price
    TEST_ASSERT_FLOAT_WITHIN(0.001, 75.00, result.price);
    TEST_ASSERT_EQUAL_STRING("Game A", result.name); 
}

// Test Case 3: Edge case - Invalid price data (should be skipped)
void test_find_max_price_invalid_data(void) {
    const char *invalid_data = 
        NEW_HEADER
        DUMMY_PREFIX "Valid Game 1" DUMMY_INFIX "75.00" DUMMY_SUFFIX
        DUMMY_PREFIX "Invalid Game" DUMMY_INFIX "FREE" DUMMY_SUFFIX // This should be skipped
        DUMMY_PREFIX "Valid Game 2" DUMMY_INFIX "85.00" DUMMY_SUFFIX; // This should be the max
    
    create_test_file(invalid_data);
    MaxPriceGame result = find_game_with_max_price(TEST_FILE_PATH);

    TEST_ASSERT_FLOAT_WITHIN(0.001, 85.00, result.price);
    TEST_ASSERT_EQUAL_STRING("Valid Game 2", result.name); 
}