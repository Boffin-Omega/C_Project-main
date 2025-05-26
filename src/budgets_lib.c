#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void clear_input_buffer(); // Add this declaration

#define MAX_BUDGETS 100
#define CATEGORY_LEN 25

typedef struct {
    char category[CATEGORY_LEN];
    double budget;
} Budget;

// Reads all budgets from the CSV file into the array.
// Returns the number of budgets read.
int read_budgets(const char *filename, Budget *budgets, int max_budgets) {
    FILE *fp = fopen(filename, "r");
    if (!fp) return 0;
    int count = 0;
    while (count < max_budgets && fscanf(fp, "%24[^,],%lf\n", budgets[count].category, &budgets[count].budget) == 2) {
        count++;
    }
    fclose(fp);
    return count;
}

// Writes all budgets from the array to the CSV file.
int write_budgets(const char *filename, Budget *budgets, int count) {
    FILE *fp = fopen(filename, "w");
    if (!fp) return 0;
    for (int i = 0; i < count; ++i) {
        fprintf(fp, "%s,%.2lf\n", budgets[i].category, budgets[i].budget);
    }
    fclose(fp);
    return 1;
}

// Finds a budget by category. Returns pointer or NULL if not found.
Budget* find_budget(Budget *budgets, int count, const char *category) {
    for (int i = 0; i < count; ++i) {
        if (strcmp(budgets[i].category, category) == 0) return &budgets[i];
    }
    return NULL;
}

int category_exists(const char *category) {
    Budget budgets[MAX_BUDGETS];
    int count = read_budgets("Data/budgets.csv", budgets, MAX_BUDGETS);
    for (int i = 0; i < count; ++i) {
        if (strcmp(budgets[i].category, category) == 0) {
            return 1;
        }
    }
    return 0;
}

// Set budget for a category (add if not exists, update if exists)
void set_category_budget() {
    Budget budgets[MAX_BUDGETS];
    int count = read_budgets("Data/budgets.csv", budgets, MAX_BUDGETS);

    char category[CATEGORY_LEN];
    double budget;
    printf("Enter category name: ");
    getchar(); // clear newline
    fgets(category, sizeof(category), stdin);
    category[strcspn(category, "\n")] = 0;
    printf("Enter budget amount: ");
    scanf("%lf", &budget);

    Budget *b = find_budget(budgets, count, category);
    if (b) {
        b->budget = budget;
        printf("Budget updated for category '%s'.\n", category);
    } else if (count < MAX_BUDGETS) {
        strncpy(budgets[count].category, category, CATEGORY_LEN-1);
        budgets[count].category[CATEGORY_LEN-1] = '\0';
        budgets[count].budget = budget;
        count++;
        printf("Budget set for new category '%s'.\n", category);
    } else {
        printf("Budget list full.\n");
        return;
    }
    write_budgets("Data/budgets.csv", budgets, count);
}

// Create a new category with budget (fails if exists)
void create_new_category() {
    Budget budgets[MAX_BUDGETS];
    int count = read_budgets("Data/budgets.csv", budgets, MAX_BUDGETS);

    char category[CATEGORY_LEN];
    double budget;
    clear_input_buffer(); // Always clear before fgets to avoid leftover newlines
    printf("Enter new category name: ");
    fgets(category, sizeof(category), stdin);
    category[strcspn(category, "\n")] = 0;

    if (find_budget(budgets, count, category)) {
        printf("Category already exists.\n");
        return;
    }
    printf("Enter budget amount: ");
    if (scanf("%lf", &budget) != 1) {
        clear_input_buffer();
        printf("Invalid input.\n");
        return;
    }
    clear_input_buffer(); // Clear after scanf

    if (count < MAX_BUDGETS) {
        strncpy(budgets[count].category, category, CATEGORY_LEN-1);
        budgets[count].category[CATEGORY_LEN-1] = '\0';
        budgets[count].budget = budget;
        count++;
        write_budgets("Data/budgets.csv", budgets, count);
        printf("New category '%s' created with budget.\n", category);
    } else {
        printf("Budget list full.\n");
    }
}

// Update existing category budget (fails if not exists)
void update_category() {
    Budget budgets[MAX_BUDGETS];
    int count = read_budgets("Data/budgets.csv", budgets, MAX_BUDGETS);

    char category[CATEGORY_LEN];
    double budget;
    printf("Enter category name to update: ");
    getchar(); // clear newline
    fgets(category, sizeof(category), stdin);
    category[strcspn(category, "\n")] = 0;

    Budget *b = find_budget(budgets, count, category);
    if (!b) {
        printf("Category not found.\n");
        return;
    }
    printf("Enter new budget amount: ");
    scanf("%lf", &budget);
    b->budget = budget;
    write_budgets("Data/budgets.csv", budgets, count);
    printf("Budget updated for category '%s'.\n", category);
}