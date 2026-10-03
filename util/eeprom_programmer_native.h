#include <windows.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>

#define COM "COM5"
#define DATA_FILE "ingreds_recipes.csv"
#define CHECKSUM_LEN 2

/** Sizes of specific items within structures */
#define MAX_NINGS 8
#define MAX_NAMEC 18
#define MAX_RECIB MAX_NAMEC + 2 * MAX_NINGS + 1

#define RCPT_SIZ 10
#define RCPT_MSG "Received\n"

typedef struct {
    char name[MAX_NAMEC];
} Ingredient;

typedef struct {
    uint8_t ningred;
    uint8_t ingred_amounts[MAX_NINGS];
    uint8_t ingred_indexes[MAX_NINGS];
    char name[MAX_NAMEC];
} Recipe;

/** counter is one of these, but it is the current max index */
typedef struct {
    uint8_t address[2];
} IAccount;

/** counter is one of these, but it is the current max index */
typedef struct {
    uint8_t address[2];
} RAccount;

#define MAX_LINE_LEN sizeof(Recipe) * 2

/** COM interaction functions */
int open_com();
boolean write_to_com(char *data);
int close_com();

/** file interaction functions */
int open_file();
int process_all_file_lines();
int close_file();
