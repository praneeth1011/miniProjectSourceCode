/*
 * ============================================================
 * COMPREHENSIVE BANKING SYSTEM - MAIN PROGRAM
 * ============================================================
 * Features: 
 * 1. Account Management (Create, View, Delete, Modify)
 * 2. Authentication System (Login with PIN)
 * 3. Banking Operations (Deposit, Withdraw, Check Balance)
 * 4. Transaction History
 * 5. Digital Wallet (UPI-like Transfers)
 * 6. Loan System with EMI
 * 7. Fixed Deposit System
 * 8. Credit Scoring
 * 9. Fraud Detection
 * 10. Queue Management (Token System)
 * 11. ATM Simulation
 * 12. Admin Panel
 * ============================================================
 */

#include "banking_system.h"
#include "accounts.h"
#include "auth.h"
#include "transactions.h"
#include "utils.h"
#include "wallet.h"
#include "loans.h"
#include "fixed_deposit.h"
#include "fraud.h"
#include "admin.h"
#include "queue.h"

// ============= MAIN DATA STRUCTURES =============
Account *accounts = NULL;
Transaction *transactions = NULL;
Loan *loans = NULL;
FixedDeposit *fds = NULL;
FraudLog *fraud_logs = NULL;
Queue *bank_queue = NULL;

int account_count = 0;
int transaction_count = 0;
int loan_count = 0;
int fd_count = 0;
int fraud_log_count = 0;

// ============= MENU FUNCTIONS =============

void display_main_menu() {
    clear_screen();
    print_header("WELCOME TO XYZ BANKING SYSTEM");
    printf("%-60s\n", "");
    printf("%-60s\n", "1. Create New Account");
    printf("%-60s\n", "2. Customer Login");
    printf("%-60s\n", "3. Admin Panel");
    printf("%-60s\n", "4. Token System (Bank Visit)");
    printf("%-60s\n", "5. ATM Withdrawal");
    printf("%-60s\n", "6. Exit");
    printf("%-60s\n", "");
    print_line(70);
}

void display_customer_menu() {
    printf("\n");
    print_header("CUSTOMER MENU");
    printf("%-60s\n", "1. Check Balance");
    printf("%-60s\n", "2. Deposit Money");
    printf("%-60s\n", "3. Withdraw Money");
    printf("%-60s\n", "4. View Transactions");
    printf("%-60s\n", "5. Money Transfer (UPI)");
    printf("%-60s\n", "6. Loan System");
    printf("%-60s\n", "7. Fixed Deposit");
    printf("%-60s\n", "8. View Credit Score");
    printf("%-60s\n", "9. Account Details");
    printf("%-60s\n", "10. Logout");
    printf("%-60s\n", "");
    print_line(70);
}

// ============= CUSTOMER OPERATIONS =============

void customer_check_balance(Account *account) {
    save_accounts(accounts, account_count);
    display_balance(account);
    pause_screen();
}

void customer_deposit(Account *account) {
    clear_screen();
    print_header("DEPOSIT MONEY");
    
    double amount = get_double_input("Enter deposit amount (Rs.): ");
    
    if (deposit(account, amount, transactions, &transaction_count)) {
        print_success("Deposit completed!");
        printf("New Balance: Rs. %.2f\n", account->balance);
    } else {
        print_error("Deposit failed!");
    }
    
    save_accounts(accounts, account_count);
    pause_screen();
}

void customer_withdraw(Account *account) {
    clear_screen();
    print_header("WITHDRAW MONEY");
    
    printf("Current Balance: Rs. %.2f\n", account->balance);
    printf("Daily Limit: Rs. %.2lf\n", (double)MAX_WITHDRAWAL_DAILY);
    printf("Already Withdrawn: Rs. %.2f\n\n", account->daily_withdrawal);
    
    double amount = get_double_input("Enter withdrawal amount (Rs.): ");
    
    if (withdraw(account, amount, transactions, &transaction_count)) {
        print_success("Withdrawal completed!");
        printf("New Balance: Rs. %.2f\n", account->balance);
        
        // Show ATM denominations
        ATMDenomination denom;
        calculate_atm_denominations(amount, &denom);
        printf("\n");
        display_atm_denominations(denom);
    } else {
        print_error("Withdrawal failed!");
    }
    
    save_accounts(accounts, account_count);
    pause_screen();
}

void customer_view_transactions(Account *account) {
    clear_screen();
    int n = get_integer_input("How many recent transactions to view? ");
    view_last_n_transactions(account, transactions, transaction_count, n);
    pause_screen();
}

void customer_transfer_money(Account *account) {
    clear_screen();
    print_header("MONEY TRANSFER (UPI)");
    
    int receiver_acct = get_integer_input("Enter Receiver Account Number: ");
    Account *receiver = find_account(accounts, account_count, receiver_acct);
    
    if (receiver == NULL) {
        print_error("Receiver account not found!");
        pause_screen();
        return;
    }
    
    double amount = get_double_input("Enter transfer amount (Rs.): ");
    
    if (transfer_money(account, receiver, amount, transactions, &transaction_count)) {
        print_success("Transfer completed!");
        printf("Receiver: %s\n", receiver->name);
        printf("Amount: Rs. %.2f\n", amount);
        
        // Generate receipt
        int trans_id = generate_transaction_id_wallet();
        display_transfer_receipt(trans_id, account->account_number, receiver->account_number, amount);
    } else {
        print_error("Transfer failed!");
    }
    
    save_accounts(accounts, account_count);
    pause_screen();
}

void customer_loan_menu(Account *account) {
    clear_screen();
    print_header("LOAN SYSTEM");
    
    printf("1. Apply for Loan\n");
    printf("2. View Loan Details\n");
    printf("3. Pay EMI\n");
    printf("4. Back\n");
    
    int choice = get_menu_choice(1, 4);
    
    switch (choice) {
        case 1: {
            if (account->credit_score < MIN_CREDIT_SCORE_FOR_LOAN) {
                print_error("Insufficient credit score for loan!");
            } else {
                double principal = get_double_input("Enter loan amount (Rs.): ");
                int term = get_integer_input("Enter loan term (months): ");
                apply_loan(loans, &loan_count, account, principal, term);
                print_success("Loan application submitted!");
            }
            break;
        }
        case 2: {
            Loan *loan = find_active_loan(loans, loan_count, account->account_number);
            if (loan != NULL) {
                view_loan_details(loan);
            } else {
                print_info("No active loans found");
            }
            break;
        }
        case 3: {
            Loan *loan = find_active_loan(loans, loan_count, account->account_number);
            if (loan != NULL && loan->status == LOAN_APPROVED) {
                if (pay_emi(loan, account, transactions, &transaction_count)) {
                    print_success("EMI paid successfully!");
                }
            }
            break;
        }
    }
    
    save_accounts(accounts, account_count);
    save_loans(loans, loan_count);
    pause_screen();
}

void customer_fd_menu(Account *account) {
    clear_screen();
    print_header("FIXED DEPOSIT SYSTEM");
    
    printf("1. Create FD\n");
    printf("2. View FD Details\n");
    printf("3. Withdraw FD\n");
    printf("4. Back\n");
    
    int choice = get_menu_choice(1, 4);
    
    switch (choice) {
        case 1: {
            double principal = get_double_input("Enter FD amount (Rs.): ");
            printf("1. 6 Months (5.5%)\n");
            printf("2. 1 Year (6.5%)\n");
            printf("3. 2 Years (7.0%)\n");
            printf("4. 3 Years (7.5%)\n");
            int duration_choice = get_menu_choice(1, 4);
            int duration = (duration_choice == 1) ? 6 : (duration_choice == 2) ? 12 : 
                          (duration_choice == 3) ? 24 : 36;
            create_fd(fds, &fd_count, account, principal, duration);
            print_success("FD created successfully!");
            break;
        }
        case 2: {
            view_all_fds(account, fds, fd_count);
            break;
        }
        case 3: {
            // Find and withdraw FD
            printf("FD withdrawal functionality\n");
            break;
        }
    }
    
    save_accounts(accounts, account_count);
    save_fds(fds, fd_count);
    pause_screen();
}

void customer_view_credit_score(Account *account) {
    clear_screen();
    print_header("CREDIT SCORE");
    printf("%-40s: %d/900\n", "Your Credit Score", account->credit_score);
    printf("%-40s: %s\n", "Rating", 
           (account->credit_score >= 750) ? "Excellent" :
           (account->credit_score >= 650) ? "Good" :
           (account->credit_score >= 550) ? "Fair" : "Poor");
    print_line(70);
    pause_screen();
}

// ============= MAIN CUSTOMER SESSION =============

void customer_session() {
    int logged_in_account = -1;
    
    if (!login_customer(accounts, account_count, &logged_in_account)) {
        return;
    }
    
    Account *customer = find_account(accounts, account_count, logged_in_account);
    
    bool session_active = true;
    while (session_active) {
        clear_screen();
        printf("\nWelcome, %s!\n", customer->name);
        display_customer_menu();
        
        int choice = get_menu_choice(1, 10);
        
        switch (choice) {
            case 1:
                customer_check_balance(customer);
                break;
            case 2:
                customer_deposit(customer);
                break;
            case 3:
                customer_withdraw(customer);
                break;
            case 4:
                customer_view_transactions(customer);
                break;
            case 5:
                customer_transfer_money(customer);
                break;
            case 6:
                customer_loan_menu(customer);
                break;
            case 7:
                customer_fd_menu(customer);
                break;
            case 8:
                customer_view_credit_score(customer);
                break;
            case 9:
                view_account_details(customer);
                pause_screen();
                break;
            case 10:
                logout_customer(customer);
                session_active = false;
                break;
        }
    }
}

// ============= ACCOUNT CREATION =============

void create_new_account() {
    clear_screen();
    print_header("CREATE NEW ACCOUNT");
    
    char name[STRING_SIZE], email[STRING_SIZE], phone[15], address[STRING_SIZE];
    int age, account_type, pin;
    
    get_string_input("Enter Full Name: ", name, STRING_SIZE);
    get_string_input("Enter Email: ", email, STRING_SIZE);
    get_string_input("Enter Phone (10 digits): ", phone, 15);
    age = get_integer_input("Enter Age: ");
    get_string_input("Enter Address: ", address, STRING_SIZE);
    
    printf("\nAccount Type:\n1. Savings\n2. Current\n3. Salary\n");
    account_type = get_menu_choice(1, 3);
    
    pin = get_integer_input("Set 4-digit PIN: ");
    
    int new_acct = create_account(accounts, &account_count, name, email, phone, 
                                  age, address, account_type, pin);
    
    if (new_acct > 0) {
        print_success("Account created successfully!");
        printf("Your Account Number: %d\n", new_acct);
        printf("Please save this for future use.\n");
    } else {
        print_error("Account creation failed!");
    }
    
    save_accounts(accounts, account_count);
    pause_screen();
}

// ============= QUEUE/TOKEN SYSTEM =============

void token_system_menu() {
    clear_screen();
    print_header("BANK VISIT - TOKEN SYSTEM");
    
    printf("1. Get Token\n");
    printf("2. Check Token Status\n");
    printf("3. View Queue\n");
    printf("4. Process Next Customer\n");
    printf("5. Back\n");
    
    int choice = get_menu_choice(1, 5);
    
    switch (choice) {
        case 1: {
            int acct = get_integer_input("Enter Account Number: ");
            char name[STRING_SIZE];
            get_string_input("Enter Name: ", name, STRING_SIZE);
            char service[STRING_SIZE];
            get_string_input("Enter Service Type: ", service, STRING_SIZE);
            
            int token = enqueue(bank_queue, acct, name, service);
            printf("Your Token Number: %d\n", token);
            print_token_receipt(token, name, service);
            break;
        }
        case 2: {
            int token = get_integer_input("Enter Token Number: ");
            view_token_status(bank_queue, token);
            break;
        }
        case 3: {
            display_queue(bank_queue);
            break;
        }
        case 4: {
            process_next_customer(bank_queue);
            break;
        }
    }
    
    pause_screen();
}

// ============= ATM SIMULATION =============

void atm_withdrawal_menu() {
    clear_screen();
    print_header("ATM WITHDRAWAL");
    
    int acct_num = get_integer_input("Enter Account Number: ");
    int pin = get_integer_input("Enter PIN: ");
    
    if (!authenticate_user(accounts, account_count, acct_num, pin)) {
        print_error("Authentication failed!");
        pause_screen();
        return;
    }
    
    Account *acc = find_account(accounts, account_count, acct_num);
    
    printf("Current Balance: Rs. %.2f\n", acc->balance);
    printf("Daily Limit: Rs. %.2f\n", MAX_WITHDRAWAL_DAILY - acc->daily_withdrawal);
    
    double amount = get_double_input("Enter withdrawal amount (multiple of 100): ");
    
    if (withdraw(acc, amount, transactions, &transaction_count)) {
        print_success("Withdrawal successful!");
        ATMDenomination denom;
        calculate_atm_denominations(amount, &denom);
        display_atm_denominations(denom);
    } else {
        print_error("Withdrawal failed!");
    }
    
    save_accounts(accounts, account_count);
    pause_screen();
}

// ============= MAIN FUNCTION =============

int main() {
    // Initialize data structures
    accounts = load_accounts(&account_count);
    transactions = load_transactions(&transaction_count);
    loans = load_loans(&loan_count);
    fds = load_fds(&fd_count);
    fraud_logs = load_fraud_logs(&fraud_log_count);
    bank_queue = create_queue();
    
    // Main loop
    bool system_running = true;
    while (system_running) {
        display_main_menu();
        int choice = get_menu_choice(1, 6);
        
        switch (choice) {
            case 1:
                create_new_account();
                break;
            case 2:
                customer_session();
                break;
            case 3:
                admin_menu(accounts, &account_count, transactions, &transaction_count, 
                          loans, &loan_count);
                break;
            case 4:
                token_system_menu();
                break;
            case 5:
                atm_withdrawal_menu();
                break;
            case 6:
                print_success("Thank you for using our banking system!");
                system_running = false;
                break;
            default:
                print_error("Invalid choice!");
        }
    }
    
    // Save all data before exit
    save_accounts(accounts, account_count);
    save_transactions(transactions, transaction_count);
    save_loans(loans, loan_count);
    save_fds(fds, fd_count);
    save_fraud_logs(fraud_logs, fraud_log_count);
    
    // Free allocated memory
    free(accounts);
    free(transactions);
    free(loans);
    free(fds);
    free(fraud_logs);
    free(bank_queue);
    
    return 0;
}
