/*
 * ADMIN PANEL MODULE HEADER
 * Admin functions: view accounts, search, sort, delete, statistics
 */

#ifndef ADMIN_H
#define ADMIN_H

#include "banking_system.h"
#include "accounts.h"
#include "transactions.h"
#include "utils.h"
#include "fraud.h"

// Function prototypes
void admin_menu(Account *accounts, int *acc_count, Transaction *transactions, 
                int *trans_count, Loan *loans, int *loan_count);
void view_all_accounts_admin(Account *accounts, int count);
void search_account_admin(Account *accounts, int count);
void sort_accounts_by_balance(Account *accounts, int count);
void search_accounts_by_name(Account *accounts, int count);
void delete_account_admin(Account *accounts, int *count);
void generate_statistics(Account *accounts, int count, Transaction *transactions, 
                         int trans_count);
void view_fraud_alerts(Account *accounts, int count);
void generate_daily_report(Account *accounts, int count, Transaction *transactions, 
                          int trans_count);
void reset_daily_withdrawals(Account *accounts, int count);

#endif // ADMIN_H
