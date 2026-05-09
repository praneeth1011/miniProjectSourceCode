/*
 * FRAUD DETECTION MODULE IMPLEMENTATION
 */

#include "fraud.h"

FraudLog* load_fraud_logs(int *count) {
    FILE *file = fopen(FRAUD_FILE, "rb");
    FraudLog *logs = (FraudLog*)malloc(MAX_QUEUE * sizeof(FraudLog));
    *count = 0;
    
    if (file == NULL) return logs;
    
    while (fread(&logs[*count], sizeof(FraudLog), 1, file) && *count < MAX_QUEUE) {
        (*count)++;
    }
    fclose(file);
    return logs;
}

void save_fraud_logs(FraudLog *logs, int count) {
    FILE *file = fopen(FRAUD_FILE, "wb");
    if (file == NULL) return;
    
    for (int i = 0; i < count; i++) {
        fwrite(&logs[i], sizeof(FraudLog), 1, file);
    }
    fclose(file);
}

bool detect_multiple_withdrawals(Account *account, Transaction *transactions, int trans_count) {
    int withdrawal_count = 0;
    time_t recent_time = time(NULL) - FRAUD_WITHDRAWAL_INTERVAL;
    
    for (int i = 0; i < trans_count; i++) {
        if (transactions[i].account_number == account->account_number &&
            transactions[i].type == TRANSACTION_WITHDRAW &&
            transactions[i].timestamp > recent_time) {
            withdrawal_count++;
        }
    }
    
    return (withdrawal_count >= FRAUD_WITHDRAWAL_COUNT);
}

bool detect_large_transaction(double amount) {
    return (amount > FRAUD_LARGE_TRANSACTION);
}

bool detect_failed_logins(Account *account) {
    return (account->failed_login_attempts >= 2);
}

void flag_suspicious_activity(Account *account, FraudLog *logs, int *count, 
                             char *fraud_type, char *description) {
    set_fraud_flag(account, true);
    log_fraud_detection(logs, count, account->account_number, fraud_type, description);
}

void review_flagged_accounts(Account *accounts, int count) {
    printf("\n");
    print_header("FLAGGED ACCOUNTS");
    printf("%-10s %-20s %-30s %-10s\n", "AcctNo", "Name", "Reason", "Status");
    print_line(70);
    
    for (int i = 0; i < count; i++) {
        if (accounts[i].is_flagged_fraud) {
            printf("%-10d %-20s %-30s %-10s\n", 
                   accounts[i].account_number, accounts[i].name,
                   "Suspicious Activity", "FLAGGED");
        }
    }
    print_line(70);
}

void unflag_account(Account *account) {
    set_fraud_flag(account, false);
    printf("Account unflagged successfully\n");
}

void log_fraud_detection(FraudLog *logs, int *count, int account_number, 
                        char *fraud_type, char *description) {
    if (*count >= MAX_QUEUE) return;
    
    logs[*count].account_number = account_number;
    strcpy(logs[*count].fraud_type, fraud_type);
    strcpy(logs[*count].description, description);
    logs[*count].detected_time = time(NULL);
    logs[*count].is_resolved = false;
    
    (*count)++;
}
