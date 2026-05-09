/*
 * FIXED DEPOSIT MODULE IMPLEMENTATION
 */

#include "fixed_deposit.h"

static int fd_counter = 7000;

FixedDeposit* load_fds(int *count) {
    FILE *file = fopen(FD_FILE, "rb");
    FixedDeposit *fds = (FixedDeposit*)malloc(MAX_FD * sizeof(FixedDeposit));
    *count = 0;
    
    if (file == NULL) return fds;
    
    while (fread(&fds[*count], sizeof(FixedDeposit), 1, file) && *count < MAX_FD) {
        (*count)++;
    }
    fclose(file);
    return fds;
}

void save_fds(FixedDeposit *fds, int count) {
    FILE *file = fopen(FD_FILE, "wb");
    if (file == NULL) return;
    
    for (int i = 0; i < count; i++) {
        fwrite(&fds[i], sizeof(FixedDeposit), 1, file);
    }
    fclose(file);
}

double get_fd_rate(int duration_months) {
    if (duration_months == 6) return FD_RATE_6_MONTHS;
    if (duration_months == 12) return FD_RATE_1_YEAR;
    if (duration_months == 24) return FD_RATE_2_YEARS;
    if (duration_months == 36) return FD_RATE_3_YEARS;
    return FD_RATE_1_YEAR;  // Default
}

double calculate_maturity_amount(double principal, double rate, int months) {
    double interest = principal * (rate / 100) * (months / 12.0);
    return principal + interest;
}

int create_fd(FixedDeposit *fds, int *count, Account *account, 
              double principal, int duration_months) {
    
    if (*count >= MAX_FD || principal > account->balance) {
        return -1;
    }
    
    FixedDeposit new_fd;
    new_fd.fd_id = ++fd_counter;
    new_fd.account_number = account->account_number;
    new_fd.principal = principal;
    new_fd.rate = get_fd_rate(duration_months);
    new_fd.duration_months = duration_months;
    new_fd.maturity_amount = calculate_maturity_amount(principal, new_fd.rate, duration_months);
    new_fd.created_date = time(NULL);
    new_fd.maturity_date = new_fd.created_date + (duration_months * 30 * 24 * 60 * 60);
    new_fd.is_mature = false;
    
    // Deduct from account balance
    account->balance -= principal;
    
    fds[*count] = new_fd;
    (*count)++;
    save_fds(fds, *count);
    
    printf("FD Created with ID: %d\n", new_fd.fd_id);
    printf("Maturity Amount: Rs. %.2f\n", new_fd.maturity_amount);
    
    return new_fd.fd_id;
}

bool withdraw_fd(FixedDeposit *fd, Account *account, bool is_early, 
                 Transaction *transactions, int *trans_count) {
    
    double amount;
    
    if (is_early) {
        // Early withdrawal with penalty
        amount = fd->maturity_amount * (1 - EARLY_WITHDRAWAL_PENALTY / 100);
        printf("Early withdrawal penalty applied: %.1f%%\n", EARLY_WITHDRAWAL_PENALTY);
    } else {
        amount = fd->maturity_amount;
    }
    
    account->balance += amount;
    fd->is_mature = true;
    
    record_transaction(transactions, trans_count, account->account_number,
                      TRANSACTION_FD_MATURED, amount, account->balance,
                      "FD Maturity", 0);
    
    return true;
}

void view_fd_details(FixedDeposit *fd) {
    printf("\n");
    print_header("FIXED DEPOSIT DETAILS");
    printf("%-40s: %d\n", "FD ID", fd->fd_id);
    printf("%-40s: Rs. %.2f\n", "Principal Amount", fd->principal);
    printf("%-40s: %.2f%%\n", "Annual Rate", fd->rate);
    printf("%-40s: %d months\n", "Duration", fd->duration_months);
    printf("%-40s: Rs. %.2f\n", "Maturity Amount", fd->maturity_amount);
    printf("%-40s: %s\n", "Status", fd->is_mature ? "Matured" : "Active");
    
    struct tm *tm_info = localtime(&fd->maturity_date);
    char date_str[20];
    strftime(date_str, 20, "%Y-%m-%d", tm_info);
    printf("%-40s: %s\n", "Maturity Date", date_str);
    print_line(70);
}

void view_all_fds(Account *account, FixedDeposit *fds, int count) {
    printf("\n");
    print_header("YOUR FIXED DEPOSITS");
    printf("%-10s %-15s %-15s %-15s %-15s\n", "FD ID", "Principal", "Rate", "Maturity", "Status");
    print_line(70);
    
    for (int i = 0; i < count; i++) {
        if (fds[i].account_number == account->account_number) {
            printf("%-10d %-15.2f %-15.2f %-15.2f %-15s\n",
                   fds[i].fd_id, fds[i].principal, fds[i].rate,
                   fds[i].maturity_amount, fds[i].is_mature ? "Matured" : "Active");
        }
    }
    print_line(70);
}

void check_matured_fds(FixedDeposit *fds, int count, Account *accounts, 
                       int acc_count, Transaction *transactions, int *trans_count) {
    time_t now = time(NULL);
    
    for (int i = 0; i < count; i++) {
        if (!fds[i].is_mature && fds[i].maturity_date <= now) {
            fds[i].is_mature = true;
            
            Account *acc = find_account(accounts, acc_count, fds[i].account_number);
            if (acc != NULL) {
                acc->balance += fds[i].maturity_amount;
                record_transaction(transactions, trans_count, acc->account_number,
                                  TRANSACTION_FD_MATURED, fds[i].maturity_amount,
                                  acc->balance, "FD Auto Maturity", 0);
            }
        }
    }
}
