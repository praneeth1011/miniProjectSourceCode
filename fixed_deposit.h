/*
 * FIXED DEPOSIT (FD) MODULE HEADER
 * Handles fixed deposit creation, maturity calculation, withdrawal
 */

#ifndef FIXED_DEPOSIT_H
#define FIXED_DEPOSIT_H

#include "banking_system.h"
#include "accounts.h"
#include "transactions.h"
#include "utils.h"

// Fixed Deposit rates (annual)
#define FD_RATE_6_MONTHS 5.5
#define FD_RATE_1_YEAR 6.5
#define FD_RATE_2_YEARS 7.0
#define FD_RATE_3_YEARS 7.5
#define EARLY_WITHDRAWAL_PENALTY 1.0  // 1% penalty

// Function prototypes
FixedDeposit* load_fds(int *count);
void save_fds(FixedDeposit *fds, int count);

// FD operations
int create_fd(FixedDeposit *fds, int *count, Account *account, 
              double principal, int duration_months);
double calculate_maturity_amount(double principal, double rate, int months);
double get_fd_rate(int duration_months);
bool withdraw_fd(FixedDeposit *fd, Account *account, bool is_early, 
                 Transaction *transactions, int *trans_count);
void view_fd_details(FixedDeposit *fd);
void view_all_fds(Account *account, FixedDeposit *fds, int count);
void check_matured_fds(FixedDeposit *fds, int count, Account *accounts, 
                       int acc_count, Transaction *transactions, int *trans_count);

#endif // FIXED_DEPOSIT_H
