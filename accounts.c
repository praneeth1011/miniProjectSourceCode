/*
 * ACCOUNTS MODULE IMPLEMENTATION
 * Functions for account creation, management, and persistence
 */

#include "accounts.h"

// ============= FILE OPERATIONS =============

Account* load_accounts(int *count) {
    FILE *file = fopen(ACCOUNTS_FILE, "rb");
    Account *accounts = (Account*)malloc(MAX_ACCOUNTS * sizeof(Account));
    *count = 0;
    
    if (file == NULL) {
        // File doesn't exist, create empty array
        return accounts;
    }
    
    while (fread(&accounts[*count], sizeof(Account), 1, file) && *count < MAX_ACCOUNTS) {
        (*count)++;
    }
    
    fclose(file);
    return accounts;
}

void save_accounts(Account *accounts, int count) {
    FILE *file = fopen(ACCOUNTS_FILE, "wb");
    if (file == NULL) {
        printf("Error: Could not save accounts file\n");
        return;
    }
    
    for (int i = 0; i < count; i++) {
        fwrite(&accounts[i], sizeof(Account), 1, file);
    }
    
    fclose(file);
}

// ============= ACCOUNT CREATION =============

int generate_account_number(Account *accounts, int count) {
    static int next_account = MIN_ACCOUNT_NUMBER;
    
    // Check if account number already exists
    for (int i = 0; i < count; i++) {
        if (accounts[i].account_number == next_account) {
            next_account++;
            i = -1; // Restart search
        }
    }
    
    return next_account++;
}

int create_account(Account *accounts, int *count, char *name, char *email, 
                   char *phone, int age, char *address, AccountType type, int pin) {
    
    if (*count >= MAX_ACCOUNTS) {
        printf("Error: Maximum accounts reached\n");
        return -1;
    }
    
    // Validate inputs
    if (!validate_email(email)) {
        printf("Error: Invalid email format\n");
        return -1;
    }
    
    if (!validate_phone(phone)) {
        printf("Error: Invalid phone number (must be 10 digits)\n");
        return -1;
    }
    
    if (!validate_pin(pin)) {
        printf("Error: Invalid PIN (must be 4 digits)\n");
        return -1;
    }
    
    // Check if email already exists
    if (find_account_by_email(accounts, *count, email) != NULL) {
        printf("Error: Email already registered\n");
        return -1;
    }
    
    // Create new account
    Account new_account;
    new_account.account_number = generate_account_number(accounts, *count);
    strcpy(new_account.name, name);
    strcpy(new_account.email, email);
    strcpy(new_account.phone, phone);
    new_account.age = age;
    strcpy(new_account.address, address);
    new_account.account_type = type;
    new_account.balance = 0.0;
    new_account.pin = simple_encode_pin(pin);
    new_account.credit_score = INITIAL_CREDIT_SCORE;
    new_account.is_active = true;
    new_account.created_date = time(NULL);
    new_account.last_login = 0;
    new_account.daily_withdrawal = 0.0;
    new_account.failed_login_attempts = 0;
    new_account.is_flagged_fraud = false;
    
    accounts[*count] = new_account;
    (*count)++;
    save_accounts(accounts, *count);
    
    return new_account.account_number;
}

// ============= ACCOUNT SEARCH =============

Account* find_account(Account *accounts, int count, int account_number) {
    for (int i = 0; i < count; i++) {
        if (accounts[i].account_number == account_number && accounts[i].is_active) {
            return &accounts[i];
        }
    }
    return NULL;
}

Account* find_account_by_email(Account *accounts, int count, char *email) {
    for (int i = 0; i < count; i++) {
        if (strcmp(accounts[i].email, email) == 0 && accounts[i].is_active) {
            return &accounts[i];
        }
    }
    return NULL;
}

// ============= ACCOUNT DELETION =============

bool delete_account(Account *accounts, int *count, int account_number) {
    Account *acc = find_account(accounts, *count, account_number);
    if (acc == NULL) {
        printf("Error: Account not found\n");
        return false;
    }
    
    if (acc->balance > 0) {
        printf("Error: Cannot delete account with remaining balance\n");
        return false;
    }
    
    acc->is_active = false;
    save_accounts(accounts, *count);
    printf("Account deleted successfully\n");
    return true;
}

// ============= ACCOUNT DISPLAY =============

void view_account_details(Account *account) {
    if (account == NULL) {
        printf("Error: Account not found\n");
        return;
    }
    
    printf("\n");
    print_header("ACCOUNT DETAILS");
    printf("%-30s: %d\n", "Account Number", account->account_number);
    printf("%-30s: %s\n", "Name", account->name);
    printf("%-30s: %s\n", "Email", account->email);
    printf("%-30s: %s\n", "Phone", account->phone);
    printf("%-30s: %d years\n", "Age", account->age);
    printf("%-30s: %s\n", "Address", account->address);
    printf("%-30s: %s\n", "Account Type", 
           (account->account_type == ACCOUNT_TYPE_SAVINGS ? "Savings" : 
            account->account_type == ACCOUNT_TYPE_CURRENT ? "Current" : "Salary"));
    printf("%-30s: Rs. %.2f\n", "Current Balance", account->balance);
    printf("%-30s: %d\n", "Credit Score", account->credit_score);
    printf("%-30s: %s\n", "Status", account->is_active ? "Active" : "Inactive");
    printf("%-30s: %s", "Fraud Flag", account->is_flagged_fraud ? "YES (FLAGGED)" : "No");
    printf("\n");
    print_line(70);
}

void list_all_accounts(Account *accounts, int count) {
    printf("\n");
    print_header("ALL ACCOUNTS");
    printf("%-10s %-20s %-15s %-15s %-15s\n", "AcctNo", "Name", "Email", "Balance", "Credit");
    print_line(70);
    
    int active_count = 0;
    for (int i = 0; i < count; i++) {
        if (accounts[i].is_active) {
            printf("%-10d %-20s %-15s %-15.2f %-15d\n", 
                   accounts[i].account_number, accounts[i].name, accounts[i].email,
                   accounts[i].balance, accounts[i].credit_score);
            active_count++;
        }
    }
    
    printf("\nTotal Active Accounts: %d\n", active_count);
    print_line(70);
}

// ============= ACCOUNT UPDATE =============

void update_account_info(Account *account, char *field, char *value) {
    if (strcmp(field, "address") == 0) {
        strcpy(account->address, value);
    } else if (strcmp(field, "phone") == 0) {
        if (validate_phone(value)) {
            strcpy(account->phone, value);
        } else {
            printf("Error: Invalid phone format\n");
        }
    }
}

// ============= CREDIT SCORE MANAGEMENT =============

void update_credit_score(Account *account, int change) {
    account->credit_score += change;
    
    // Keep score within bounds
    if (account->credit_score > 900) account->credit_score = 900;
    if (account->credit_score < 0) account->credit_score = 0;
}

// ============= FRAUD MANAGEMENT =============

void set_fraud_flag(Account *account, bool flag) {
    account->is_flagged_fraud = flag;
    if (flag) {
        printf("⚠ Account flagged for suspicious activity\n");
    }
}
