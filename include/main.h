#ifndef MAIN_H
#define MAIN_H
#define CATEGORY_LEN 25

typedef struct {
    char category[CATEGORY_LEN];
    double budget;
} Budget;

typedef struct Date {
    int day;
    int month;
    int year;
} Date;

typedef struct Transaction {
    int id;
    Date date;
    double amount;
    char account[50];
    char category[25];
    char desc[100];
} Transaction;

// Function declarations

//Transaction functions
int read_transactions(const char *filename, Transaction *transactions, int max_transactions);
int write_transactions(const char *filename, Transaction *transactions, int count);
int add_transaction(const char *filename, Transaction *transactions, int *count, Transaction *new_t);
Transaction* find_transaction(Transaction *transactions, int count, int id);
int update_transaction_by_id(const char *filename, Transaction *transactions, int count, int id, Transaction *updated);
int delete_transaction_by_id(const char *filename, Transaction *transactions, int *count, int id);

// Budget functions
void set_category_budget();
void create_new_category();
void update_category();
int category_exists(const char *category);
void clear_input_buffer();
int read_budgets(const char *filename, void *budgets, int max_budgets);

#endif // MAIN_H