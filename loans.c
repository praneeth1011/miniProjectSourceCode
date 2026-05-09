/*
 * LOAN SYSTEM MODULE IMPLEMENTATION
 */

#include "loans.h"

static int loan_counter = 5000;

Loan* load_loans(int *count) {
    FILE *file = fopen(LOANS_FILE, "rb");
    Loan *loans = (Loan*)malloc(MAX_LOANS * sizeof(Loan));
    *count = 0;
    
    if (file == NULL) return loans;
    
    while (fread(&loans[*count], sizeof(Loan), 1, file) && *count < MAX_LOANS) {
        (*count)++;
    }
    fclose(file);
    return loans;
}

void save_loans(Loan *loans, int count) {
    FILE *file = fopen(LOANS_FILE, "wb");
    if (file == NULL) return;
    
    for (int i = 0; i < count; i++) {
        fwrite(&loans[i], sizeof(Loan), 1, file);
    }
    fclose(file);
}

int apply_loan(Loan *loans, int *count, Account *account, double principal, int term_months) {
    if (*count >= MAX_LOANS || principal < MIN_LOAN_AMOUNT || principal > MAX_LOAN_AMOUNT) {
        return -1;
    }
    
    Loan new_loan;
    new_loan.loan_id = ++loan_counter;
    new_loan.account_number = account->account_number;
    new_loan.principal = principal;
    new_loan.interest_rate = calculate_interest_rate(account);
    new_loan.term_months = term_months;
    new_loan.monthly_emi = calculate_emi(principal, new_loan.interest_rate, term_months);
    new_loan.remaining_balance = principal;
    new_loan.status = LOAN_PENDING;
    new_loan.applied_date = time(NULL);
    new_loan.approved_date = 0;
    new_loan.emis_paid = 0;
    
    loans[*count] = new_loan;
    (*count)++;
    save_loans(loans, *count);
    return new_loan.loan_id;
}

bool approve_loan(Loan *loans, int index, Account *account) {
    if (index < 0 || loans[index].status != LOAN_PENDING) return false;
    
    loans[index].status = LOAN_APPROVED;
    loans[index].approved_date = time(NULL);
    account->balance += loans[index].principal;
    update_credit_score(account, 50);
    
    return true;
}

double calculate_emi(double principal, double rate, int term_months) {
    double monthly_rate = (rate / 100) / 12;
    double emi = principal * (monthly_rate * pow(1 + monthly_rate, term_months)) / 
                (pow(1 + monthly_rate, term_months) - 1);
    return emi;
}

double calculate_interest_rate(Account *account) {
    double base_rate = BASE_INTEREST_RATE;
    
    // Adjust rate based on credit score
    if (account->credit_score >= 750) {
        return base_rate - 1.5;  // 7.0%
    } else if (account->credit_score >= 650) {
        return base_rate;        // 8.5%
    } else if (account->credit_score >= 550) {
        return base_rate + 1.5;  // 10.0%
    } else {
        return base_rate + 3.0;  // 11.5%
    }
}

bool pay_emi(Loan *loan, Account *account, Transaction *transactions, int *trans_count) {
    if (loan->status != LOAN_APPROVED) return false;
    if (loan->remaining_balance <= 0) return false;
    
    if (account->balance < loan->monthly_emi) {
        print_error("Insufficient balance for EMI");
        return false;
    }
    
    account->balance -= loan->monthly_emi;
    loan->remaining_balance -= loan->monthly_emi;
    loan->emis_paid++;
    
    record_transaction(transactions, trans_count, account->account_number,
                      TRANSACTION_EMI_PAID, loan->monthly_emi, account->balance,
                      "EMI Payment", 0);
    update_credit_score(account, 20);
    
    if (loan->remaining_balance <= 0) {
        loan->status = LOAN_CLOSED;
        update_credit_score(account, 50);
    }
    
    return true;
}

Loan* find_loan(Loan *loans, int count, int loan_id) {
    for (int i = 0; i < count; i++) {
        if (loans[i].loan_id == loan_id) {
            return &loans[i];
        }
    }
    return NULL;
}

Loan* find_active_loan(Loan *loans, int count, int account_number) {
    for (int i = 0; i < count; i++) {
        if (loans[i].account_number == account_number && loans[i].status == LOAN_APPROVED) {
            return &loans[i];
        }
    }
    return NULL;
}

void view_loan_details(Loan *loan) {
    printf("\n");
    print_header("LOAN DETAILS");
    printf("%-40s: %d\n", "Loan ID", loan->loan_id);
    printf("%-40s: Rs. %.2f\n", "Principal Amount", loan->principal);
    printf("%-40s: %.2f%%\n", "Interest Rate", loan->interest_rate);
    printf("%-40s: %d months\n", "Loan Term", loan->term_months);
    printf("%-40s: Rs. %.2f\n", "Monthly EMI", loan->monthly_emi);
    printf("%-40s: Rs. %.2f\n", "Remaining Balance", loan->remaining_balance);
    printf("%-40s: %d\n", "EMIs Paid", loan->emis_paid);
    printf("%-40s: %s\n", "Status", 
           (loan->status == LOAN_APPROVED) ? "Approved" :
           (loan->status == LOAN_PENDING) ? "Pending" : 
           (loan->status == LOAN_CLOSED) ? "Closed" : "Rejected");
    print_line(70);
}

void view_all_loans(Account *account, Loan *loans, int count) {
    printf("\n");
    print_header("YOUR LOANS");
    printf("%-10s %-15s %-15s %-15s %-15s\n", "Loan ID", "Principal", "EMI", "Remaining", "Status");
    print_line(70);
    
    for (int i = 0; i < count; i++) {
        if (loans[i].account_number == account->account_number) {
            printf("%-10d %-15.2f %-15.2f %-15.2f %-15d\n",
                   loans[i].loan_id, loans[i].principal, loans[i].monthly_emi,
                   loans[i].remaining_balance, loans[i].status);
        }
    }
    print_line(70);
}
