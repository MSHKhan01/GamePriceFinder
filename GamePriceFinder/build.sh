#!/bin/bash

# --- 1. Preparation ---
echo "--- Cleaning old build artifacts ---"
rm -rf build
mkdir build

# --- 2. Configure (using CMake) ---
echo "--- Configuring project with CMake ---"
cmake -S . -B build
if [ $? -ne 0 ]; then
    echo "ERROR: CMake configuration failed."
    exit 1
fi

# --- 3. Build ---
echo "--- Building project executables ---"
cmake --build build
if [ $? -ne 0 ]; then
    echo "ERROR: Build failed."
    exit 1
fi

# --- 4. Testing (using CTest) ---
echo "--- Running Unit Tests with CTest ---"
cd build
# The --verbose flag shows the status of each individual test
ctest --verbose 
TEST_RESULT=$?
cd ..  # FIX: Changed 'cd 嗯' back to 'cd ..' to return to the root directory

if [ $TEST_RESULT -ne 0 ]; then
    echo "--- Unit Tests FAILED ---"
    exit 1
else
    echo "--- All Unit Tests PASSED ---"
fi

# --- 5. Optional: Run Main Application Example ---
echo "--- Running Main Application Example ---"

# Define the full CSV header and dummy fields for the application example
# Indices: name=1, price=4

echo -e "appid,name,release_date,required_age,price,dlc_count,detailed_description,about_the_game,short_description,reviews,header_image,website,support_url,support_email,windows,mac,linux,metacritic_score,metacritic_url,achievements,recommendations,notes,supported_languages,full_audio_languages,packages,developers,publishers,categories,genres,screenshots,movies,user_score,score_rank,positive,negative,estimated_owners,average_playtime_forever,average_playtime_2weeks,median_playtime_forever,median_playtime_2weeks,discount,peak_ccu,tags,pct_pos_total,num_reviews_total,pct_pos_recent,num_reviews_recent
100001,Zelda (High),2024-01-01,0,59.99,0,\"desc\",\"about\",\"short desc\",\"reviews\",\"img\",\"web\",\"supp_url\",\"email\",1,1,1,90,\"meta_url\",0,0,\"notes\",\"lang\",\"audio\",0,\"dev\",\"pub\",\"cat\",\"gen\",\"screen\",\"movies\",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
100002,Minecraft (Low),2024-01-01,0,26.95,0,\"desc\",\"about\",\"short desc\",\"reviews\",\"img\",\"web\",\"supp_url\",\"email\",1,1,1,90,\"meta_url\",0,0,\"notes\",\"lang\",\"audio\",0,\"dev\",\"pub\",\"cat\",\"gen\",\"screen\",\"movies\",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
100003,Star Citizen (MAX),2024-01-01,0,499.00,0,\"desc\",\"about\",\"short desc\",\"reviews\",\"img\",\"web\",\"supp_url\",\"email\",1,1,1,90,\"meta_url\",0,0,\"notes\",\"lang\",\"audio\",0,\"dev\",\"pub\",\"cat\",\"gen\",\"screen\",\"movies\",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0" > example_games.csv

./build/game_finder example_games.csv
rm example_games.csv # Clean up example data

echo "--- Build and Test Complete ---"
exit 0