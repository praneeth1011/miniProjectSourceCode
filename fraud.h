/*
 * FRAUD DETECTION MODULE HEADER
 * Detects suspicious activities and flags accounts
 */

#ifndef FRAUD_H
#define FRAUD_H

#include "banking_system.h"
#include "accounts.h"
#include "transactions.h"
#include "utils.h"

// Function prototypes
FraudLog* load_fraud_logs(int *count);
void save_fraud_logs(FraudLog *logs, int count);

// Fraud detection
bool detect_multiple_withdrawals(Account *account, Transaction *transactions, int trans_count);
bool detect_large_transaction(double amount);
bool detect_failed_logins(Account *account);
void flag_suspicious_activity(Account *account, FraudLog *logs, int *count, 
                             char *fraud_type, char *description);
void review_flagged_accounts(Account *accounts, int count);
void unflag_account(Account *account);
void log_fraud_detection(FraudLog *logs, int *count, int account_number, 
                        char *fraud_type, char *description);

#endif // FRAUD_H
