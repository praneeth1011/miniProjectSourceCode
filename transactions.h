/*
 * TRANSACTIONS MODULE HEADER
 * Handles deposits, withdrawals, balance checking, transaction history
 */

#ifndef TRANSACTIONS_H
#define TRANSACTIONS_H

#include "banking_system.h"
#include "utils.h"
#include "accounts.h"

// Function prototypes
Transaction* load_transactions(int *count);
void save_transactions(Transaction *transactions, int count);

// Core operations
bool deposit(Account *account, double amount, Transaction *transactions, int *trans_count);
bool withdraw(Account *account, double amount, Transaction *transactions, int *trans_count);
double check_balance(Account *account);
void display_balance(Account *account);

// Transaction history
void view_last_n_transactions(Account *account, Transaction *transactions, 
                              int trans_count, int n);
void view_all_transactions(Account *account, Transaction *transactions, int trans_count);
int generate_transaction_id();

// Transaction recording
int record_transaction(Transaction *transactions, int *trans_count, 
                       int account_number, TransactionType type, double amount, 
                       double balance, char *description, int related_account);

// Daily limit management
bool check_daily_limit(Account *account, double amount);
void update_daily_withdrawal(Account *account, double amount);
void reset_daily_withdrawal(Account *account);

#endif // TRANSACTIONS_H
