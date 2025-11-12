#include "game_finder.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

// Helper function to extract a field (simplified, assumes no quoted commas)
// NOTE: This function iterates through the fields in the current line pointer.
char *get_field(char **line, int field_index) {
    if (!*line) return NULL;

    int current_field = 0;
    char *start = *line;
    char *end = start;
    
    // Find the start of the field (skip field_index number of fields/commas)
    // NOTE: This simplified logic relies on sequential calls to find the correct field.
    // In the main function, field_index is always passed as 0 because we handle
    // the skipping of fields by calling this function repeatedly up to PRICE_INDEX.
    while (current_field < field_index) {
        end = strchr(end, ',');
        if (end == NULL) return NULL; // Not enough fields
        end++;
        start = end;
        current_field++;
    }

    // Find the end of the field (next comma or end of string)
    end = strchr(start, ',');
    if (end) {
        *end = '\0'; // Null-terminate the field
        // Move the line pointer past the comma for the next call
        *line = end + 1; 
    } else {
        // Last field, handle potential newline
        size_t len = strlen(start);
        if (len > 0 && start[len - 1] == '\n') {
            start[len - 1] = '\0';
        }
        *line = NULL; // Signal end of line
    }
    
    return start;
}


MaxPriceGame find_game_with_max_price(const char *csv_filepath) {
    MaxPriceGame max_game = {"", 0.0};
    double current_max_price = -1.0;
    
    FILE *file = fopen(csv_filepath, "r");
    if (!file) {
        fprintf(stderr, "Error: Could not open file %s\n", csv_filepath);
        return max_game; // Return default empty structure
    }

    char line[1024]; // Increased line buffer size for the very long format
    
    // Skip the header row
    if (fgets(line, sizeof(line), file) == NULL) {
        // File is empty or only contains header
        fclose(file);
        return max_game;
    }

    // Indices in the new CSV format: [1] name, [4] price
    const int NAME_INDEX = 1;
    const int PRICE_INDEX = 4;

    // Iterate through the data rows
    while (fgets(line, sizeof(line), file)) {
        // Create a temporary, writable copy of the line to allow null termination
        char temp_line[1024]; 
        strncpy(temp_line, line, sizeof(temp_line) - 1);
        temp_line[sizeof(temp_line) - 1] = '\0';

        char *line_ptr = temp_line;
        
        char *title = NULL;
        char *price_str = NULL;
        
        // Loop through fields up to the price index
        for (int i = 0; i <= PRICE_INDEX; i++) { 
            // Get the next field (field_index is 0 because get_field advances line_ptr)
            char *field = get_field(&line_ptr, 0); 
            
            if (!field) {
                // Not enough fields in the row
                title = NULL; 
                price_str = NULL;
                break;
            }

            if (i == NAME_INDEX) {
                title = field; // Store the name
            } else if (i == PRICE_INDEX) {
                price_str = field; // Store the price string
                break; // Optimization: We have all needed data
            }
        }
        
        if (!title || !price_str) {
            fprintf(stderr, "Warning: Skipping malformed row.\n");
            continue;
        }


        // Convert price string to double
        char *endptr;
        double price = strtod(price_str, &endptr);
        
        // Check for conversion errors or invalid price data (e.g., text)
        // If endptr points to price_str, no conversion occurred.
        // If *endptr is not the null terminator, extra characters were found.
        if (endptr == price_str || (*endptr != '\0' && *endptr != '\n') || price < 0) {
            fprintf(stderr, "Warning: Skipping row with invalid price data: '%s'\n", price_str);
            continue; 
        }

        // Check if this game is more expensive
        if (price > current_max_price) {
            current_max_price = price;
            // Copy data to the result structure
            strncpy(max_game.name, title, sizeof(max_game.name) - 1);
            max_game.name[sizeof(max_game.name) - 1] = '\0'; // Ensure null termination
            max_game.price = price;
        }
    }

    fclose(file);
    return max_game;
}