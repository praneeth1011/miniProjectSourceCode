/*
 * LOAN SYSTEM MODULE HEADER
 * Handles loan applications, approval, EMI calculation, tracking
 */

#ifndef LOANS_H
#define LOANS_H

#include "banking_system.h"
#include "accounts.h"
#include "transactions.h"
#include "utils.h"

// Constants for loan
#define MIN_LOAN_AMOUNT 10000
#define MAX_LOAN_AMOUNT 1000000
#define MIN_CREDIT_SCORE_FOR_LOAN 300
#define BASE_INTEREST_RATE 8.5

// Function prototypes
Loan* load_loans(int *count);
void save_loans(Loan *loans, int count);

// Loan operations
int apply_loan(Loan *loans, int *count, Account *account, double principal, int term_months);
bool approve_loan(Loan *loans, int index, Account *account);
double calculate_emi(double principal, double rate, int term_months);
double calculate_interest_rate(Account *account);
bool pay_emi(Loan *loan, Account *account, Transaction *transactions, int *trans_count);
Loan* find_loan(Loan *loans, int count, int loan_id);
Loan* find_active_loan(Loan *loans, int count, int account_number);
void view_loan_details(Loan *loan);
void view_all_loans(Account *account, Loan *loans, int count);

#endif // LOANS_H
