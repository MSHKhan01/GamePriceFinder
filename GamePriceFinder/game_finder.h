#ifndef GAME_FINDER_H
#define GAME_FINDER_H

// Structure to hold the result (game name and price)
typedef struct {
    char name[100];
    double price;
} MaxPriceGame;

/**
 * @brief Parses a CSV file to find the game with the highest price.
 * * It safely extracts data from index 1 (name) and index 4 (price) 
 * * based on the new, long CSV header provided by the user.
 * * @param csv_filepath The path to the CSV file.
 * @return MaxPriceGame The structure containing the name and price of the most expensive game.
 */
MaxPriceGame find_game_with_max_price(const char *csv_filepath);

#endif // GAME_FINDER_H