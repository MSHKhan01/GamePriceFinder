#include <stdio.h>
#include <stdlib.h>
#include "game_finder.h"

int main(int argc, char *argv[]) {
    // Check for the required CSV file argument
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <path_to_csv_file>\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char *filepath = argv[1];
    printf("Searching for the most expensive game in: %s\n", filepath);

    MaxPriceGame most_expensive = find_game_with_max_price(filepath);

    if (most_expensive.price > 0.0) {
        printf("\n--- Result ---\n");
        printf("Most Expensive Game: %s\n", most_expensive.name);
        printf("Price: $%.2f\n", most_expensive.price);
        printf("--------------\n");
        return EXIT_SUCCESS;
    } else {
        printf("\nNo valid game data found or file could not be opened.\n");
        return EXIT_FAILURE;
    }
}