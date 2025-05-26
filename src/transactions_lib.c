#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/main.h"

#define MAX_TRANSACTIONS 1000

// Reads all transactions from the CSV file into the array.
// Returns the number of transactions read.
int read_transactions(const char *filename, Transaction *transactions, int max_transactions) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        perror("Failed to open transactions file");
        return 0;
    }

    int count = 0;
    Transaction t;
    char account[50], category[50], desc[256];

    while (count < max_transactions && 
           fscanf(fp, "%d,%d,%d,%d,%lf,%[^,],%[^,],%[^\n]\n",
                 &t.id, &t.date.day, &t.date.month, &t.date.year,
                 &t.amount, account, category, desc) == 8) {
        
        strncpy(t.account, account, sizeof(t.account) - 1);
        strncpy(t.category, category, sizeof(t.category) - 1);
        strncpy(t.desc, desc, sizeof(t.desc) - 1);
        
        t.account[sizeof(t.account) - 1] = '\0';
        t.category[sizeof(t.category) - 1] = '\0';
        t.desc[sizeof(t.desc) - 1] = '\0';

        transactions[count++] = t;
    }

    fclose(fp);
    return count;
}

// Writes all transactions from the array to the CSV file.
int write_transactions(const char *filename, Transaction *transactions, int count) {
    FILE *fp = fopen(filename, "w");
    if (!fp) {
        perror("Failed to open transactions file for writing");
        return 0;
    }

    for (int i = 0; i < count; ++i) {
        Transaction *t = &transactions[i]; //why use pointers here tho, jus used Transaction t = transactions[i], and accessed properties using t.property
        fprintf(fp, "%d,%d,%d,%d,%.2lf,%s,%s,%s\n",
            t->id, t->date.day, t->date.month, t->date.year,
            t->amount, t->account, t->category, t->desc);
    }

    fclose(fp);
    return 1;
}

// Adds a transaction to the array and writes to file.
int add_transaction(const char *filename, Transaction *transactions, int *count, Transaction *new_t) {
    if (*count >= MAX_TRANSACTIONS) return 0;
    transactions[*count] = *new_t;
    (*count)++;
    return write_transactions(filename, transactions, *count); //again why pass integer pointers here and in the definition
}

// Finds a transaction by id. Returns pointer or NULL if not found.
Transaction* find_transaction(Transaction *transactions, int count, int id) {
    for (int i = 0; i < count; ++i) {
        if (transactions[i].id == id) return &transactions[i];
    }
    return NULL;
}

// Updates a transaction by id. Returns 1 if updated, 0 if not found.
int update_transaction_by_id(const char *filename, Transaction *transactions, int count, int id, Transaction *updated) {
    for (int i = 0; i < count; ++i) {
        if (transactions[i].id == id) {
            transactions[i] = *updated;
            return write_transactions(filename, transactions, count);
        }
    }
    return 0;
}

// Deletes a transaction by id. Returns 1 if deleted, 0 if not found.
int delete_transaction_by_id(const char *filename, Transaction *transactions, int *count, int id) {
    int found = 0;
    for (int i = 0; i < *count; ++i) {
        if (transactions[i].id == id) {
            found = 1;
            // Shift remaining transactions left
            for (int j = i; j < *count - 1; ++j) {
                transactions[j] = transactions[j + 1];
            }
            (*count)--;
            break;
        }
    }
    if (found) {
        return write_transactions(filename, transactions, *count);
    }
    return 0;
}