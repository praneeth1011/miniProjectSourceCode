/*
 * AUTHENTICATION MODULE IMPLEMENTATION
 * User login, PIN verification, session management
 */

#include "auth.h"

// ============= PIN ENCODING/DECODING =============

void encode_pin(int pin, int *encoded) {
    *encoded = simple_encode_pin(pin);
}

bool verify_pin(int pin, int encoded_pin) {
    int encoded = simple_encode_pin(pin);
    return (encoded == encoded_pin);
}

// ============= AUTHENTICATION =============

bool authenticate_user(Account *accounts, int count, int account_number, int pin) {
    Account *account = find_account(accounts, count, account_number);
    
    if (account == NULL) {
        printf("Error: Account not found\n");
        return false;
    }
    
    if (account->is_flagged_fraud) {
        printf("Error: Account is flagged for suspicious activity\n");
        return false;
    }
    
    if (!verify_pin(pin, account->pin)) {
        handle_failed_login(account);
        return false;
    }
    
    reset_failed_attempts(account);
    return true;
}

// ============= LOGIN SYSTEM =============

bool login_customer(Account *accounts, int count, int *logged_in_account) {
    int max_attempts = 3;
    int attempts = 0;
    
    while (attempts < max_attempts) {
        clear_screen();
        print_header("CUSTOMER LOGIN");
        
        int account_num = get_integer_input("Enter Account Number: ");
        int pin = get_integer_input("Enter PIN: ");
        
        if (authenticate_user(accounts, count, account_num, pin)) {
            Account *acc = find_account(accounts, count, account_num);
            acc->last_login = time(NULL);
            *logged_in_account = account_num;
            print_success("Login successful!");
            pause_screen();
            return true;
        } else {
            attempts++;
            printf("Login failed. Attempts remaining: %d\n", max_attempts - attempts);
            pause_screen();
        }
    }
    
    print_error("Maximum login attempts exceeded");
    return false;
}

// ============= FAILED LOGIN HANDLING =============

void handle_failed_login(Account *account) {
    account->failed_login_attempts++;
    printf("Incorrect PIN. Attempts: %d/3\n", account->failed_login_attempts);
    
    if (account->failed_login_attempts >= 3) {
        account->is_flagged_fraud = true;
        print_error("Account has been locked due to failed login attempts");
    }
}

void reset_failed_attempts(Account *account) {
    account->failed_login_attempts = 0;
}

// ============= LOGOUT =============

void logout_customer(Account *account) {
    printf("\nYou have been logged out successfully.\n");
    printf("Thank you for using our banking system!\n");
}
