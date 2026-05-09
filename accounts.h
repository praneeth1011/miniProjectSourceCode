/*
 * ACCOUNTS MODULE HEADER
 * Functions for account management: create, view, delete, modify accounts
 */

#ifndef ACCOUNTS_H
#define ACCOUNTS_H

#include "banking_system.h"
#include "utils.h"

// Function prototypes
Account* load_accounts(int *count);
void save_accounts(Account *accounts, int count);
int create_account(Account *accounts, int *count, char *name, char *email, 
                   char *phone, int age, char *address, AccountType type, int pin);
Account* find_account(Account *accounts, int count, int account_number);
Account* find_account_by_email(Account *accounts, int count, char *email);
bool delete_account(Account *accounts, int *count, int account_number);
void view_account_details(Account *account);
void list_all_accounts(Account *accounts, int count);
void update_account_info(Account *account, char *field, char *value);
int generate_account_number(Account *accounts, int count);
void update_credit_score(Account *account, int change);
void set_fraud_flag(Account *account, bool flag);

#endif // ACCOUNTS_H
