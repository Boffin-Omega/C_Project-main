#define MAX_TRANSACTIONS 1000
#define MAX_BUDGETS 100
#include "../include/main.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Function prototypes
void view_transactions_menu();
void add_transaction_menu();
void update_transaction_menu();
void delete_transaction_menu();
void budget_manipulation_menu();
void financial_report_menu();

void show_all_transactions();
void sort_transactions();
void group_by_categories();
void search_transactions();

void clear_input_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void show_menu() {
    printf("\n========================================\n");
    printf("      \033[1mPersonal Finance Manager\033[0m\n");
    printf("========================================\n");
    printf(" 1. View Transactions\n");
    printf(" 2. Add Transaction\n");
    printf(" 3. Update Transaction\n");
    printf(" 4. Delete Transaction\n");
    printf(" 5. Budget Manipulation\n");
    printf(" 6. Financial Report\n");
    printf(" 0. Exit\n");
    printf("----------------------------------------\n");
    printf("Enter your choice: ");
}

int main() {
    int choice;
    while (1) {
        show_menu();
        if (scanf("%d", &choice) != 1) {
            clear_input_buffer(); // Clearing buffer
            printf("Invalid input. Try again.\n");
            continue;
        }
        switch (choice) {
            case 1:
                view_transactions_menu();
                break;
            case 2:
                add_transaction_menu();
                break;
            case 3:
                update_transaction_menu();
                break;
            case 4:
                delete_transaction_menu();
                break;
            case 5:
                budget_manipulation_menu();
                break;
            case 6:
                financial_report_menu();
                break;
            case 0:
                printf("Goodbye!\n");
                exit(0);
            default:
                printf("Invalid choice. Try again.\n");
        }
        printf("\n");
    }
    return 0;
}

void view_transactions_menu() {
    int done = 1;
    int choice;
    while (done) {
        printf("\n\n[View Transactions]\n\n");
        printf("1. Show all transactions\n");
        printf("2. Sort transactions\n");
        printf("3. Group by categories\n");
        printf("4. Search transactions\n");
        printf("0. Exit from [View Transactions]\n");
        printf("Enter your choice: ");
        if (scanf("%d", &choice) != 1) {
            clear_input_buffer(); // Clearing buffer
            printf("Invalid input. Try again.\n");
            continue;
        }
        switch (choice) {
            case 1:
                show_all_transactions();
                break;
            case 2:
                sort_transactions();
                break;
            case 3:
                group_by_categories();
                break;
            case 4:
                search_transactions();
                break;
            case 0:
                done = 0;
                break;
            default:
                printf("Invalid choice. Try again.\n");
        }
        printf("\n");
    }
}

void add_transaction_menu() {
    Transaction t;
    Transaction transactions[MAX_TRANSACTIONS];
    int count = read_transactions("Data/transactions.csv", transactions, MAX_TRANSACTIONS);
    char choice;

    printf("\n\n[Add Transaction]\n\n");
    printf("Enter the day/month/year: ");

    if(scanf("%d/%d/%d", &t.date.day, &t.date.month, &t.date.year) != 3 
        ||t.date.day < 1 || t.date.day > 31 || t.date.month < 1 || t.date.month  > 12 ||
        t.date.year < 1900 || t.date.year > 2100){
            clear_input_buffer();
            printf("Invalid date. Please enter a valid day, month and year in dd/mm/yyyy format\n");
            return;
        }

    printf("Enter the amount transferred: "); //we want user to input positive amts only
    if(scanf("%lf", &t.amount)!=1 || t.amount < 0){
        clear_input_buffer();
        printf("Invalid amount input\n.");
        return;
    }
    getchar(); // consume newline
    printf("Is the amount transferred to your account(y)? or transferred from your account(n)? ");

    scanf(" %c", &choice); // Note the space before %c to skip whitespace
    if(choice!='n' && choice!='y' && choice!='Y' && choice!='N'){
        // printf("%c\n",choice);
        printf("Invalid choice.\n");
        return;
    }
    clear_input_buffer();  // Clear buffer after scanf

    if (choice == 'n')
        t.amount *= -1;

    printf("Enter the account name: ");
    fgets(t.account, sizeof(t.account), stdin);
    t.account[strcspn(t.account, "\n")] = 0;

    // Clear buffer before next fgets
    // clear_input_buffer();

    do {
        printf("Enter the category: ");
        fgets(t.category, sizeof(t.category), stdin);
        t.category[strcspn(t.category, "\n")] = 0;
        if (!category_exists(t.category)) {
            printf("Category does not exist. Redirecting to create a new category...\n");
            create_new_category();
            // DO NOT call clear_input_buffer() here!
        }
    } while (!category_exists(t.category));

    printf("Enter the description: ");
    fgets(t.desc, sizeof(t.desc), stdin);
    t.desc[strcspn(t.desc, "\n")] = 0;

    // Generate new id
    t.id = (count > 0) ? transactions[count-1].id + 1 : 1;

    if (add_transaction("Data/transactions.csv", transactions, &count, &t))
        printf("[SUCCESS] Transaction added successfully!\n");
    else
        printf("[ERROR]   Failed to add transaction.\n");
}

void update_transaction_menu() {
    printf("\n\n[Update Transaction]\n\n");
    Transaction transactions[MAX_TRANSACTIONS];
    int count = read_transactions("Data/transactions.csv", transactions, MAX_TRANSACTIONS);

    int id;
    printf("Enter transaction id: ");
    if (scanf("%d", &id) != 1) {
        clear_input_buffer();
        printf("Invalid input.\n");
        return;
    }
    clear_input_buffer(); // Clear buffer after scanf

    Transaction *t = find_transaction(transactions, count, id);
    if (!t) {
        printf("Transaction not found.\n");
        return;
    }

    printf("Enter the day/month/year: ");
    if (scanf("%d/%d/%d", &t->date.day, &t->date.month, &t->date.year) != 3
        ||t->date.day < 1 || t->date.day > 31 || t->date.month < 1 || t->date.month  > 12 ||
        t->date.year < 1900 || t->date.year > 2100)
        {
        clear_input_buffer();
        printf("Invalid input.\n");
        return;
    }
    clear_input_buffer();

    printf("Enter the amount transferred: ");
    if (scanf("%lf", &t->amount) != 1) {
        clear_input_buffer();
        printf("Invalid input.\n");
        return;
    }
    clear_input_buffer();

    printf("Is the amount transferred to your account(y)? or transferred from your account(n)? ");
    char choice;
    if (scanf(" %c", &choice) != 1) {
        clear_input_buffer();
        printf("Invalid input.\n");
        return;
    }
    clear_input_buffer();

    if (choice == 'n')
        t->amount *= -1;

    printf("Enter the account name: ");
    fgets(t->account, sizeof(t->account), stdin);
    t->account[strcspn(t->account, "\n")] = 0;

    // Category input and verification
    do {
        printf("Enter the category: ");
        fgets(t->category, sizeof(t->category), stdin);
        t->category[strcspn(t->category, "\n")] = 0;
        if (!category_exists(t->category)) {
            printf("Category does not exist. Redirecting to create a new category...\n");
            create_new_category();
        }
    } while (!category_exists(t->category));

    printf("Enter the description: ");
    fgets(t->desc, sizeof(t->desc), stdin);
    t->desc[strcspn(t->desc, "\n")] = 0;

    if (update_transaction_by_id("Data/transactions.csv", transactions, count, id, t))
        printf("Transaction updated successfully!\n");
    else
        printf("Failed to update transaction.\n");
}

void delete_transaction_menu() {
    printf("\n\n[Delete Transaction]\n\n");
    Transaction transactions[MAX_TRANSACTIONS];
    int count = read_transactions("Data/transactions.csv", transactions, MAX_TRANSACTIONS);

    int id;
    printf("Enter transaction id to delete: ");
    if (scanf("%d", &id) != 1) {
        clear_input_buffer();
        printf("Invalid input.\n");
        return;
    }
    clear_input_buffer();

    if (delete_transaction_by_id("Data/transactions.csv", transactions, &count, id))
        printf("Transaction deleted successfully!\n");
    else
        printf("Transaction not found or failed to delete.\n");
}

void display_all_budgets() {
    Budget budgets[MAX_BUDGETS];
    int count = read_budgets("Data/budgets.csv", budgets, MAX_BUDGETS);

    if (count == 0) {
        printf("[INFO]    No budgets found.\n");
        return;
    }

    printf("\n%-25s %-10s\n", "Category", "Budget");
    printf("------------------------- ----------\n");
    for (int i = 0; i < count; ++i) {
        printf("%-25s %-10.2lf\n", budgets[i].category, budgets[i].budget);
    }
}

void budget_manipulation_menu() {
    int done = 1;
    int choice;
    while (done) {
        printf("\n\n[Budget Manipulation]\n\n");
        printf("1. Set or update budget for category\n");
        printf("2. Create new category\n");
        printf("3. Display all budget categories\n");
        printf("0. Exit from [Budget Manipulation]\n");
        printf("Enter your choice: ");
        if (scanf("%d", &choice) != 1) {
            clear_input_buffer(); // Clearing buffer
            printf("Invalid input. Try again.\n");
            continue;
        }
        switch (choice) {
            case 1:
                set_category_budget();
                break;
            case 2:
                create_new_category();
                break;
            case 3:
                display_all_budgets();
                break;
            case 0:
                done = 0;
                break;
            default:
                printf("Invalid choice. Try again.\n");
        }
        printf("\n");
    }
}

void financial_report_menu() {
    printf("\n\n[Financial Report]\n\n");

    Transaction transactions[MAX_TRANSACTIONS];
    Budget budgets[MAX_BUDGETS];
    int t_count = read_transactions("Data/transactions.csv", transactions, MAX_TRANSACTIONS);

    int c_count = 0, ex_count = 0,i,j;
    //split transactions into two arrays, one with expenses and one where money is credited
    Transaction credits[MAX_TRANSACTIONS/2];
    Transaction expenses[MAX_TRANSACTIONS/2];

    if (t_count == 0) {
        printf("[INFO]    No transactions found.\n");
        return;
    }

    double total_income = 0, total_expense = 0;
    for (i = 0; i < t_count; ++i) {
        if (transactions[i].amount > 0){
            total_income += transactions[i].amount; //have the user input
            credits[c_count] = transactions[i];
            c_count++;
        }
        else{ //store expenses
            total_expense += transactions[i].amount;
            expenses[ex_count] = transactions[i];
            ex_count++;
        }
    }

    printf("Total Income:    %.2lf\n", total_income);
    printf("Total Expenses:  %.2lf\n", total_expense);
    printf("Net Balance:     %.2lf\n", total_income + total_expense);

    // Read budgets
    int b_count = read_budgets("Data/budgets.csv", budgets, MAX_BUDGETS);

    if (b_count == 0) {
        printf("No budgets found.\n");
        return;
    }

    printf("\nIncome Breakdown\n");
    printf("%-20s %10s\n","Category","Credited Amt");
    printf("----------------------------------\n");

    for(i = 0; i < b_count; ++i){
        double credited = 0;
        for(j = 0; j < c_count; ++j){
            if (strcmp(credits[j].category, budgets[i].category) == 0) {
                credited += credits[j].amount; //calculating amt recieved for each category
            }
        }
        if(credited!=0){
            printf("%-20s %10.2lf\n",
                budgets[i].category,
                credited
            );
        }
    }
    
    printf("\n");
    // Budget vs. Spending per category
    printf("\nBudget vs. Spending by Category:\n\n");
    printf("%-20s %10s %10s %10s\n","Category","Budget","Spent","Remarks");
    printf("------------------------------------------------------\n");

    for (i = 0; i < b_count; ++i) {
        double spent = 0;
        for (j = 0; j < ex_count; ++j) {
            if (strcmp(expenses[j].category, budgets[i].category) == 0) {
                spent += expenses[j].amount; //calculating spent amount for each category
            }
        }
        if(spent!=0){
            printf("%-20s %10.2lf %10.2lf %10s\n",
            budgets[i].category,
            budgets[i].budget,
            -1*spent,
            (-1*spent > budgets[i].budget) ? "Over Budget!" : "Within Budget");
        }
    }
}

void show_all_transactions() {
    Transaction transactions[MAX_TRANSACTIONS];
    int count = read_transactions("Data/transactions.csv", transactions, MAX_TRANSACTIONS);

    if (count == 0) {
        printf("[INFO]    No transactions found.\n");
        return;
    }

    printf("\n%-5s %-12s %-10s %-15s %-15s %-20s\n", 
        "ID", "Date", "Amount", "Account", "Category", "Description");
    printf("-------------------------------------------------------------------------------\n");
    for (int i = 0; i < count; ++i) {
        printf("%-5d %02d/%02d/%04d %-10.2lf %-15s %-15s %-20s\n",
            transactions[i].id,
            transactions[i].date.day, transactions[i].date.month, transactions[i].date.year,
            transactions[i].amount,
            transactions[i].account,
            transactions[i].category,
            transactions[i].desc
        );
    }
}

int compare_by_amount(const void *a, const void *b) {
    const Transaction *t1 = (const Transaction *)a;
    const Transaction *t2 = (const Transaction *)b;
    if (t1->amount < t2->amount) return -1;
    if (t1->amount > t2->amount) return 1;
    return 0;
}

int compare_by_date(const void *a, const void *b) {
    const Transaction *t1 = (const Transaction *)a;
    const Transaction *t2 = (const Transaction *)b;
    if (t1->date.year != t2->date.year)
        return t1->date.year - t2->date.year;
    if (t1->date.month != t2->date.month)
        return t1->date.month - t2->date.month;
    return t1->date.day - t2->date.day;
}

int compare_by_category(const void *a, const void *b) {
    const Transaction *t1 = (const Transaction *)a;
    const Transaction *t2 = (const Transaction *)b;
    return strcmp(t1->category, t2->category);
}

void sort_transactions() {
    Transaction transactions[MAX_TRANSACTIONS];
    int count = read_transactions("Data/transactions.csv", transactions, MAX_TRANSACTIONS);

    if (count == 0) {
        printf("No transactions to sort.\n");
        return;
    }

    printf("Sort by:\n");
    printf("1. Amount\n");
    printf("2. Date\n");
    printf("3. Category\n");
    printf("Enter your choice: ");
    int choice;
    if (scanf("%d", &choice) != 1) {
        clear_input_buffer();
        printf("Invalid input.\n");
        return;
    }

    switch (choice) {
        case 1:
            qsort(transactions, count, sizeof(Transaction), compare_by_amount);
            printf("Sorted by amount.\n");
            break;
        case 2:
            qsort(transactions, count, sizeof(Transaction), compare_by_date);
            printf("Sorted by date.\n");
            break;
        case 3:
            qsort(transactions, count, sizeof(Transaction), compare_by_category);
            printf("Sorted by category.\n");
            break;
        default:
            printf("Invalid choice.\n");
            return;
    }

    printf("\n%-5s %-12s %-10s %-15s %-15s %-20s\n", 
        "ID", "Date", "Amount", "Account", "Category", "Description");
    printf("-------------------------------------------------------------------------------\n");
    for (int i = 0; i < count; ++i) {
        printf("%-5d %02d/%02d/%04d %-10.2lf %-15s %-15s %-20s\n",
            transactions[i].id,
            transactions[i].date.day, transactions[i].date.month, transactions[i].date.year,
            transactions[i].amount,
            transactions[i].account,
            transactions[i].category,
            transactions[i].desc
        );
    }
}

void group_by_categories() {
    Transaction transactions[MAX_TRANSACTIONS];
    int count = read_transactions("Data/transactions.csv", transactions, MAX_TRANSACTIONS);

    if (count == 0) {
        printf("No transactions to group.\n");
        return;
    }

    // Find unique categories
    char categories[MAX_TRANSACTIONS][25];
    int cat_count = 0;
    for (int i = 0; i < count; ++i) {
        int found = 0;
        for (int j = 0; j < cat_count; ++j) {
            if (strcmp(categories[j], transactions[i].category) == 0) {
                found = 1;
                break;
            }
        }
        if (!found && cat_count < MAX_TRANSACTIONS) {
            strncpy(categories[cat_count], transactions[i].category, 24);
            categories[cat_count][24] = '\0';
            cat_count++;
        }
    }

    // Print transactions grouped by category
    for (int i = 0; i < cat_count; ++i) {
        double total = 0;
        printf("\nCategory: %s\n", categories[i]);
        printf("%-5s %-12s %-10s %-15s %-20s\n", 
            "ID", "Date", "Amount", "Account", "Description");
        printf("---------------------------------------------------------------\n");
        for (int j = 0; j < count; ++j) {
            if (strcmp(transactions[j].category, categories[i]) == 0) {
                printf("%-5d %02d/%02d/%04d %-10.2lf %-15s %-20s\n",
                    transactions[j].id,
                    transactions[j].date.day, transactions[j].date.month, transactions[j].date.year,
                    transactions[j].amount,
                    transactions[j].account,
                    transactions[j].desc
                );
                total += transactions[j].amount;
            }
        }
        printf("Total for %s: %.2lf\n", categories[i], total);
    }
}

// Helper: Convert string to lowercase (for case-insensitive search)
void to_lower(char *str) {
    for (; *str; ++str) *str = tolower(*str);
}

void search_transactions() {
    Transaction transactions[MAX_TRANSACTIONS];
    int count = read_transactions("Data/transactions.csv", transactions, MAX_TRANSACTIONS);

    if (count == 0) {
        printf("No transactions to search.\n");
        return;
    }

    printf("Search by:\n");
    printf("1. Category\n");
    printf("2. Account\n");
    printf("3. Description\n");
    printf("4. All fields\n");
    printf("Enter your choice: ");
    int choice;
    if (scanf("%d", &choice) != 1) {
        clear_input_buffer();
        printf("Invalid input.\n");
        return;
    }
    clear_input_buffer();

    char query[100], query_lc[100];
    printf("Enter search term: ");
    fgets(query, sizeof(query), stdin);
    query[strcspn(query, "\n")] = 0;
    strncpy(query_lc, query, sizeof(query_lc));
    to_lower(query_lc);

    printf("\n%-5s %-12s %-10s %-15s %-15s %-20s\n", 
        "ID", "Date", "Amount", "Account", "Category", "Description");
    printf("-------------------------------------------------------------------------------\n");

    int found = 0;
    for (int i = 0; i < count; ++i) {
        char field1[100], field2[100], field3[100], field4[300];
        int match = 0;
        switch (choice) {
            case 1:
                strncpy(field1, transactions[i].category, sizeof(field1));
                to_lower(field1);
                if (strstr(field1, query_lc)) match = 1;
                break;
            case 2:
                strncpy(field2, transactions[i].account, sizeof(field2));
                to_lower(field2);
                if (strstr(field2, query_lc)) match = 1;
                break;
            case 3:
                strncpy(field3, transactions[i].desc, sizeof(field3));
                to_lower(field3);
                if (strstr(field3, query_lc)) match = 1;
                break;
            case 4:
                snprintf(field4, sizeof(field4), "%s %s %s", transactions[i].category, transactions[i].account, transactions[i].desc);
                to_lower(field4);
                if (strstr(field4, query_lc)) match = 1;
                break;
            default:
                printf("Invalid choice.\n");
                return;
        }
        if (match) {
            found = 1;
            printf("%-5d %02d/%02d/%04d %-10.2lf %-15s %-15s %-20s\n",
                transactions[i].id,
                transactions[i].date.day, transactions[i].date.month, transactions[i].date.year,
                transactions[i].amount,
                transactions[i].account,
                transactions[i].category,
                transactions[i].desc
            );
        }
    }
    if (!found) {
        printf("No transactions found matching your search.\n");
    }
}