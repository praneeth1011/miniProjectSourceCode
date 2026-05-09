/*
 * ADMIN PANEL MODULE IMPLEMENTATION
 */

#include "admin.h"

void admin_menu(Account *accounts, int *acc_count, Transaction *transactions, 
                int *trans_count, Loan *loans, int *loan_count) {
    
    while (true) {
        clear_screen();
        print_header("ADMIN PANEL");
        printf("%-60s\n", "1. View All Accounts");
        printf("%-60s\n", "2. Search Account");
        printf("%-60s\n", "3. Sort Accounts by Balance");
        printf("%-60s\n", "4. Delete Account");
        printf("%-60s\n", "5. View Fraud Alerts");
        printf("%-60s\n", "6. Generate Daily Report");
        printf("%-60s\n", "7. Statistics");
        printf("%-60s\n", "8. Reset Daily Withdrawals");
        printf("%-60s\n", "9. Back to Main Menu");
        printf("%-60s\n", "");
        print_line(70);
        
        int choice = get_menu_choice(1, 9);
        
        switch (choice) {
            case 1:
                view_all_accounts_admin(accounts, *acc_count);
                break;
            case 2:
                search_account_admin(accounts, *acc_count);
                break;
            case 3:
                sort_accounts_by_balance(accounts, *acc_count);
                break;
            case 4:
                delete_account_admin(accounts, acc_count);
                break;
            case 5:
                review_flagged_accounts(accounts, *acc_count);
                break;
            case 6:
                generate_daily_report(accounts, *acc_count, transactions, *trans_count);
                break;
            case 7:
                generate_statistics(accounts, *acc_count, transactions, *trans_count);
                break;
            case 8:
                reset_daily_withdrawals(accounts, *acc_count);
                break;
            case 9:
                return;
        }
        
        pause_screen();
    }
}

void view_all_accounts_admin(Account *accounts, int count) {
    list_all_accounts(accounts, count);
}

void search_account_admin(Account *accounts, int count) {
    clear_screen();
    print_header("SEARCH ACCOUNT");
    
    printf("1. Search by Account Number\n");
    printf("2. Search by Name\n");
    int choice = get_menu_choice(1, 2);
    
    if (choice == 1) {
        int acct_num = get_integer_input("Enter Account Number: ");
        Account *acc = find_account(accounts, count, acct_num);
        if (acc != NULL) {
            view_account_details(acc);
        } else {
            print_error("Account not found!");
        }
    } else {
        search_accounts_by_name(accounts, count);
    }
}

void sort_accounts_by_balance(Account *accounts, int count) {
    clear_screen();
    print_header("ACCOUNTS SORTED BY BALANCE");
    
    Account *sorted = (Account*)malloc(sizeof(Account) * count);
    for (int i = 0; i < count; i++) {
        sorted[i] = accounts[i];
    }
    
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (sorted[j].balance < sorted[j + 1].balance) {
                Account temp = sorted[j];
                sorted[j] = sorted[j + 1];
                sorted[j + 1] = temp;
            }
        }
    }
    
    printf("\n%-10s %-20s %-20s %-15s\n", "AcctNo", "Name", "Email", "Balance");
    print_line(70);
    
    for (int i = 0; i < count; i++) {
        if (sorted[i].is_active) {
            printf("%-10d %-20s %-20s %-15.2f\n",
                   sorted[i].account_number, sorted[i].name, sorted[i].email,
                   sorted[i].balance);
        }
    }
    
    print_line(70);
    free(sorted);
}

void search_accounts_by_name(Account *accounts, int count) {
    clear_screen();
    char search_name[STRING_SIZE];
    get_string_input("Enter Name or partial name: ", search_name, STRING_SIZE);
    
    printf("\n%-10s %-20s %-20s %-15s\n", "AcctNo", "Name", "Email", "Balance");
    print_line(70);
    
    int found = 0;
    for (int i = 0; i < count; i++) {
        if (accounts[i].is_active && strstr(accounts[i].name, search_name) != NULL) {
            printf("%-10d %-20s %-20s %-15.2f\n",
                   accounts[i].account_number, accounts[i].name, accounts[i].email,
                   accounts[i].balance);
            found++;
        }
    }
    
    if (found == 0) {
        print_error("No accounts found!");
    } else {
        printf("\nTotal found: %d\n", found);
    }
    print_line(70);
}

void delete_account_admin(Account *accounts, int *count) {
    clear_screen();
    print_header("DELETE ACCOUNT");
    
    int acct_num = get_integer_input("Enter Account Number to delete: ");
    
    printf("Are you sure? (1=Yes, 0=No): ");
    int confirm = get_integer_input("");
    
    if (confirm == 1) {
        if (delete_account(accounts, count, acct_num)) {
            print_success("Account deleted successfully!");
            save_accounts(accounts, *count);
        } else {
            print_error("Failed to delete account!");
        }
    }
}

void generate_statistics(Account *accounts, int count, Transaction *transactions, int trans_count) {
    clear_screen();
    print_header("SYSTEM STATISTICS");
    
    double total_balance = 0;
    double avg_balance = 0;
    int active_accounts = 0;
    double high_balance = 0;
    double low_balance = 9999999;
    
    for (int i = 0; i < count; i++) {
        if (accounts[i].is_active) {
            total_balance += accounts[i].balance;
            active_accounts++;
            if (accounts[i].balance > high_balance) high_balance = accounts[i].balance;
            if (accounts[i].balance < low_balance) low_balance = accounts[i].balance;
        }
    }
    
    if (active_accounts > 0) avg_balance = total_balance / active_accounts;
    
    printf("%-40s: %d\n", "Total Active Accounts", active_accounts);
    printf("%-40s: Rs. %.2f\n", "Total Balance", total_balance);
    printf("%-40s: Rs. %.2f\n", "Average Balance", avg_balance);
    printf("%-40s: Rs. %.2f\n", "Highest Balance", high_balance);
    printf("%-40s: Rs. %.2f\n", "Lowest Balance", low_balance == 9999999 ? 0 : low_balance);
    printf("%-40s: %d\n", "Total Transactions", trans_count);
    print_line(70);
}

void generate_daily_report(Account *accounts, int count, Transaction *transactions, int trans_count) {
    clear_screen();
    print_header("DAILY REPORT");
    
    time_t now = time(NULL);
    struct tm *timeinfo = localtime(&now);
    
    int daily_deposits = 0;
    int daily_withdrawals = 0;
    double total_deposits = 0;
    double total_withdrawals = 0;
    
    for (int i = 0; i < trans_count; i++) {
        struct tm *trans_time = localtime(&transactions[i].timestamp);
        if (trans_time->tm_mday == timeinfo->tm_mday &&
            trans_time->tm_mon == timeinfo->tm_mon &&
            trans_time->tm_year == timeinfo->tm_year) {
            
            if (transactions[i].type == TRANSACTION_DEPOSIT) {
                daily_deposits++;
                total_deposits += transactions[i].amount;
            } else if (transactions[i].type == TRANSACTION_WITHDRAW) {
                daily_withdrawals++;
                total_withdrawals += transactions[i].amount;
            }
        }
    }
    
    printf("%-40s: %s\n", "Report Date", get_current_date());
    printf("%-40s: ---\n", "---");
    printf("%-40s: %d\n", "Total Deposits", daily_deposits);
    printf("%-40s: Rs. %.2f\n", "Total Deposit Amount", total_deposits);
    printf("%-40s: %d\n", "Total Withdrawals", daily_withdrawals);
    printf("%-40s: Rs. %.2f\n", "Total Withdrawal Amount", total_withdrawals);
    printf("%-40s: Rs. %.2f\n", "Net Flow", total_deposits - total_withdrawals);
    print_line(70);
}

void reset_daily_withdrawals(Account *accounts, int count) {
    for (int i = 0; i < count; i++) {
        if (accounts[i].is_active) {
            reset_daily_withdrawal(&accounts[i]);
        }
    }
    save_accounts(accounts, count);
    print_success("Daily withdrawal limits reset for all accounts!");
}
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "banking.h"

void displayAdminMenu(void)
{
    printf("\n");
    printf("╔═══════════════════════════════════════════════════════════╗\n");
    printf("║            ADMIN CONTROL PANEL                            ║\n");
    printf("╠═══════════════════════════════════════════════════════════╣\n");
    printf("║  1. View All Accounts                                     ║\n");
    printf("║  2. View All Transactions                                 ║\n");
    printf("║  3. Lock Account                                          ║\n");
    printf("║  4. Unlock Account                                        ║\n");
    printf("║  5. View Session Logs                                     ║\n");
    printf("║  6. View Account Details                                  ║\n");
    printf("║  7. Logout                                                ║\n");
    printf("╚═══════════════════════════════════════════════════════════╝\n");
    printf("\nEnter your choice: ");
}

void adminMode(void)
{
    int choice;

    printf("\n✓ Admin mode activated.\n");
    delay(500);

    while (1)
    {
        displayAdminMenu();
        scanf("%d", &choice);
        clearInputBuffer();

        switch (choice)
        {
        case 1:
            printf("\n");
            printf("╔─────────────────────────────────────────────────────────╗\n");
            printf("║ ALL ACCOUNTS                                            ║\n");
            printf("╚─────────────────────────────────────────────────────────╝\n");
            displayAllAccounts();
            break;

        case 2:
            printf("\n");
            printf("╔─────────────────────────────────────────────────────────╗\n");
            printf("║ ALL TRANSACTIONS                                        ║\n");
            printf("╚─────────────────────────────────────────────────────────╝\n");
            viewAllTransactions();
            break;

        case 3:
            printf("\n");
            printf("╔─────────────────────────────────────────────────────────╗\n");
            printf("║ LOCK ACCOUNT                                            ║\n");
            printf("╚─────────────────────────────────────────────────────────╝\n");
            lockAccount();
            break;

        case 4:
            printf("\n");
            printf("╔─────────────────────────────────────────────────────────╗\n");
            printf("║ UNLOCK ACCOUNT                                          ║\n");
            printf("╚─────────────────────────────────────────────────────────╝\n");
            unlockAccount();
            break;

        case 5:
            printf("\n");
            printf("╔─────────────────────────────────────────────────────────╗\n");
            printf("║ SESSION LOGS                                            ║\n");
            printf("╚─────────────────────────────────────────────────────────╝\n");
            displaySessionLogs();
            break;

        case 6:
            printf("\n");
            printf("╔─────────────────────────────────────────────────────────╗\n");
            printf("║ ACCOUNT DETAILS                                         ║\n");
            printf("╚─────────────────────────────────────────────────────────╝\n");
            {
                unsigned int accountNum;
                Account account;

                printf("Enter Account Number: ");
                scanf("%u", &accountNum);
                clearInputBuffer();

                if (loadAccount(accountNum, &account))
                {
                    printf("\n");
                    printf("╔─────────────────────────────────────────────┐\n");
                    printf("║ Account Number     : %u\n", account.accountNum);
                    printf("║ Username           : %s\n", account.username);
                    printf("║ Name               : %s %s\n", account.firstName, account.lastName);
                    printf("║ Balance            : Rs. %.2f\n", account.balance);
                    printf("║ Status             : %s\n", account.isLocked ? "LOCKED" : "ACTIVE");
                    printf("║ Failed Attempts    : %d\n", account.failedAttempts);
                    printf("║ Last Login         : %s", ctime(&account.lastLogin));
                    printf("║ Daily Withdrawn    : Rs. %.2f\n", account.dailyWithdrawn);
                    printf("╚─────────────────────────────────────────────┘\n");
                    
                    printf("\nTransaction History for this account:\n");
                    loadTransactionHistory(accountNum);
                }
                else
                {
                    printf("✗ Account not found.\n");
                }
            }
            break;

        case 7:
            printf("\n✓ Exiting admin mode.\n");
            return;

        default:
            printf("✗ Invalid choice. Please try again.\n");
        }

        printf("\nPress Enter to continue...");
        getchar();
    }
}

void viewAllTransactions(void)
{
    FILE *file = fopen("transactions.log", "r");
    if (!file)
    {
        printf("✗ No transaction history found.\n");
        return;
    }

    char line[256];
    int transCount = 0;

    printf("\n");
    printf("╔═══════════════════════════════════════════════════════════╗\n");
    printf("║           ALL TRANSACTIONS IN SYSTEM                      ║\n");
    printf("╚═══════════════════════════════════════════════════════════╝\n\n");

    while (fgets(line, sizeof(line), file))
    {
        printf("%s", line);
        if (strstr(line, "Account Number:"))
        {
            transCount++;
        }
    }

    printf("\nTotal Transactions: %d\n", transCount);
    fclose(file);
}

void lockAccount(void)
{
    unsigned int accountNum;
    Account account;

    printf("Enter Account Number to lock: ");
    scanf("%u", &accountNum);
    clearInputBuffer();

    if (!loadAccount(accountNum, &account))
    {
        printf("✗ Account not found.\n");
        return;
    }

    if (account.isLocked)
    {
        printf("✗ Account is already locked.\n");
        return;
    }

    account.isLocked = 1;
    if (saveAccount(&account))
    {
        printf("✓ Account %u has been locked.\n", accountNum);
        logTransaction("ACCOUNT_LOCKED", "Admin locked account");
    }
    else
    {
        printf("✗ Failed to lock account.\n");
    }
}

void unlockAccount(void)
{
    unsigned int accountNum;
    Account account;

    printf("Enter Account Number to unlock: ");
    scanf("%u", &accountNum);
    clearInputBuffer();

    if (!loadAccount(accountNum, &account))
    {
        printf("✗ Account not found.\n");
        return;
    }

    if (!account.isLocked)
    {
        printf("✗ Account is already active.\n");
        return;
    }

    account.isLocked = 0;
    account.failedAttempts = 0;

    if (saveAccount(&account))
    {
        printf("✓ Account %u has been unlocked.\n", accountNum);
        logTransaction("ACCOUNT_UNLOCKED", "Admin unlocked account");
    }
    else
    {
        printf("✗ Failed to unlock account.\n");
    }
}

void displaySessionLogs(void)
{
    FILE *file = fopen("sessions.log", "r");
    if (!file)
    {
        printf("✗ No session logs found.\n");
        return;
    }

    char line[512];
    int sessionCount = 0;

    printf("\n");
    printf("╔═══════════════════════════════════════════════════════════════════════════╗\n");
    printf("║              SESSION LOGS - USER LOGIN/LOGOUT HISTORY                    ║\n");
    printf("╠═══════════════════════════════════════════════════════════════════════════╣\n");

    while (fgets(line, sizeof(line), file))
    {
        printf("║ %s", line);
        sessionCount++;
    }

    printf("╚═══════════════════════════════════════════════════════════════════════════╝\n");
    printf("\nTotal Sessions: %d\n", sessionCount);

    fclose(file);
}
