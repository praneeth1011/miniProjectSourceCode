/*
 * BANKING SYSTEM - Main Header File
 * Complete console-based banking system with 12+ features
 * Modular architecture with proper separation of concerns
 */

#ifndef BANKING_SYSTEM_H
#define BANKING_SYSTEM_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

// ============= CONSTANTS & MACROS =============
#define MAX_ACCOUNTS 1000
#define MAX_TRANSACTIONS 5000
#define MAX_LOANS 500
#define MAX_FD 500
#define MAX_QUEUE 100
#define FILENAME_SIZE 50
#define STRING_SIZE 50
#define DATE_SIZE 20

// File names
#define ACCOUNTS_FILE "accounts.dat"
#define TRANSACTIONS_FILE "transactions.dat"
#define LOANS_FILE "loans.dat"
#define FD_FILE "fixeddeposits.dat"
#define FRAUD_FILE "fraud_log.dat"

// Banking constants
#define MIN_DEPOSIT 100
#define MAX_WITHDRAWAL_DAILY 50000
#define INITIAL_CREDIT_SCORE 500
#define PIN_LENGTH 4
#define MIN_ACCOUNT_NUMBER 1000
#define MAX_ACCOUNT_NUMBER 9999

// Fraud thresholds
#define FRAUD_WITHDRAWAL_COUNT 5      // Multiple withdrawals in short time
#define FRAUD_WITHDRAWAL_INTERVAL 300  // 5 minutes
#define FRAUD_LARGE_TRANSACTION 100000 // Large transaction threshold

// ============= ENUMS =============
typedef enum {
    ACCOUNT_TYPE_SAVINGS = 1,
    ACCOUNT_TYPE_CURRENT = 2,
    ACCOUNT_TYPE_SALARY = 3
} AccountType;

typedef enum {
    TRANSACTION_DEPOSIT = 1,
    TRANSACTION_WITHDRAW = 2,
    TRANSACTION_TRANSFER = 3,
    TRANSACTION_RECEIVED = 4,
    TRANSACTION_FD_CREATED = 5,
    TRANSACTION_FD_MATURED = 6,
    TRANSACTION_LOAN_APPROVED = 7,
    TRANSACTION_EMI_PAID = 8
} TransactionType;

typedef enum {
    LOAN_APPROVED = 1,
    LOAN_PENDING = 2,
    LOAN_REJECTED = 3,
    LOAN_CLOSED = 4
} LoanStatus;

// ============= STRUCTURES =============

// Account structure
typedef struct {
    int account_number;
    char name[STRING_SIZE];
    char email[STRING_SIZE];
    char phone[15];
    int age;
    char address[STRING_SIZE];
    AccountType account_type;
    double balance;
    int pin;  // Encoded PIN
    int credit_score;
    bool is_active;
    time_t created_date;
    time_t last_login;
    double daily_withdrawal;
    int failed_login_attempts;
    bool is_flagged_fraud;
} Account;

// Transaction structure
typedef struct {
    int transaction_id;
    int account_number;
    TransactionType type;
    double amount;
    double balance_after;
    char description[STRING_SIZE];
    time_t timestamp;
    int related_account;  // For transfers
} Transaction;

// Loan structure
typedef struct {
    int loan_id;
    int account_number;
    double principal;
    double interest_rate;
    int term_months;
    double monthly_emi;
    double remaining_balance;
    LoanStatus status;
    time_t applied_date;
    time_t approved_date;
    int emis_paid;
} Loan;

// Fixed Deposit structure
typedef struct {
    int fd_id;
    int account_number;
    double principal;
    double rate;
    int duration_months;
    double maturity_amount;
    time_t created_date;
    time_t maturity_date;
    bool is_mature;
} FixedDeposit;

// Fraud Log structure
typedef struct {
    int account_number;
    char fraud_type[STRING_SIZE];
    char description[STRING_SIZE];
    time_t detected_time;
    bool is_resolved;
} FraudLog;

// Queue Node for bank visit simulation
typedef struct {
    int token_number;
    int account_number;
    char customer_name[STRING_SIZE];
    time_t arrival_time;
    char service_type[STRING_SIZE];
} QueueNode;

// ATM Withdrawal structure
typedef struct {
    int denomination_100;
    int denomination_200;
    int denomination_500;
} ATMDenomination;

#endif // BANKING_SYSTEM_H
