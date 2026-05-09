#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "banking.h"

void displayMainMenu(void)
{
    printf("\n");
    printf("╔═══════════════════════════════════════════════════════════╗\n");
    printf("║          MULTI-USER ATM BANKING SYSTEM v1.0              ║\n");
    printf("║                   Welcome to XYZ Bank                     ║\n");
    printf("╠═══════════════════════════════════════════════════════════╣\n");
    printf("║  1. Customer Login                                        ║\n");
    printf("║  2. Create New Account                                    ║\n");
    printf("║  3. Admin Mode                                            ║\n");
    printf("║  4. Exit                                                  ║\n");
    printf("╚═══════════════════════════════════════════════════════════╝\n");
    printf("\nEnter your choice: ");
}

void displayATMMenu(void)
{
    printf("\n");
    printf("╔═══════════════════════════════════════════════════════════╗\n");
    printf("║              ATM MAIN MENU - TRANSACTION OPTIONS          ║\n");
    printf("╠═══════════════════════════════════════════════════════════╣\n");
    printf("║  1. Balance Inquiry                                       ║\n");
    printf("║  2. Withdraw Cash                                         ║\n");
    printf("║  3. Deposit Money                                         ║\n");
    printf("║  4. Fast Cash Withdrawal                                  ║\n");
    printf("║  5. Mini Statement                                        ║\n");
    printf("║  6. Change PIN                                            ║\n");
    printf("║  7. Logout                                                ║\n");
    printf("╚═══════════════════════════════════════════════════════════╝\n");
    printf("\nEnter your choice: ");
}

void atmInterface(Account *currentAccount)
{
    int choice;
    char newPin[10];
    int tempPin;

    if (!currentAccount)
    {
        printf("Error: Invalid account\n");
        return;
    }

    Session session;
    session.accountNum = currentAccount->accountNum;
    strcpy(session.username, currentAccount->username);
    session.loginTime = time(NULL);
    session.isActive = 1;

    printf("\n╔═══════════════════════════════════════════════════════════╗\n");
    printf("║  Welcome, %s!                                      \n", currentAccount->firstName);
    printf("╚═══════════════════════════════════════════════════════════╝\n");

    while (1)
    {
        displayATMMenu();
        scanf("%d", &choice);
        clearInputBuffer();

        switch (choice)
        {
        case 1:
            printf("\n");
            printf("╔─────────────────────────────────────────────────────────╗\n");
            printf("║ BALANCE INQUIRY                                         ║\n");
            printf("╚─────────────────────────────────────────────────────────╝\n");
            balanceInquiry(currentAccount);
            break;

        case 2:
            printf("\n");
            printf("╔─────────────────────────────────────────────────────────╗\n");
            printf("║ CASH WITHDRAWAL                                         ║\n");
            printf("╚─────────────────────────────────────────────────────────╝\n");
            withdrawCash(currentAccount);
            break;

        case 3:
            printf("\n");
            printf("╔─────────────────────────────────────────────────────────╗\n");
            printf("║ DEPOSIT MONEY                                           ║\n");
            printf("╚─────────────────────────────────────────────────────────╝\n");
            depositMoney(currentAccount);
            break;

        case 4:
            printf("\n");
            printf("╔─────────────────────────────────────────────────────────╗\n");
            printf("║ FAST CASH WITHDRAWAL                                    ║\n");
            printf("╚─────────────────────────────────────────────────────────╝\n");
            fastCashWithdrawal(currentAccount);
            break;

        case 5:
            printf("\n");
            printf("╔─────────────────────────────────────────────────────────╗\n");
            printf("║ MINI STATEMENT                                          ║\n");
            printf("╚─────────────────────────────────────────────────────────╝\n");
            displayMiniStatement(currentAccount->accountNum);
            break;

        case 6:
            printf("\n");
            printf("╔─────────────────────────────────────────────────────────╗\n");
            printf("║ CHANGE PIN                                              ║\n");
            printf("╚─────────────────────────────────────────────────────────╝\n");
            printf("Enter new PIN (4 digits): ");
            scanf("%4s", newPin);
            clearInputBuffer();
            tempPin = atoi(newPin);
            if (validatePIN(tempPin))
            {
                currentAccount->pin = tempPin;
                if (saveAccount(currentAccount))
                {
                    printf("✓ PIN changed successfully!\n");
                    logTransaction("PIN_CHANGE", "PIN changed");
                }
            }
            else
            {
                printf("✗ Invalid PIN. Must be 4 digits.\n");
            }
            break;

        case 7:
            printf("\n✓ Thank you for banking with us!\n");
            printf("╔═══════════════════════════════════════════════════════════╗\n");
            printf("║              Please collect your card                     ║\n");
            printf("╚═══════════════════════════════════════════════════════════╝\n");
            session.logoutTime = time(NULL);
            session.isActive = 0;
            logout(&session);
            return;

        default:
            printf("✗ Invalid choice. Please try again.\n");
        }

        printf("\nPress Enter to continue...");
        getchar();
    }
}

int createNewAccount(void)
{
    Account newAccount;
    unsigned int accountNum;
    char username[MAX_USERNAME_LEN];
    char firstName[15];
    char lastName[15];
    int pin;
    char pinStr[10];
    double initialBalance;

    printf("\n");
    printf("╔─────────────────────────────────────────────────────────╗\n");
    printf("║ CREATE NEW ACCOUNT                                      ║\n");
    printf("╚─────────────────────────────────────────────────────────╝\n");

    printf("Enter Account Number (1-100): ");
    scanf("%u", &accountNum);
    clearInputBuffer();

    if (accountExists(accountNum))
    {
        printf("✗ Account already exists!\n");
        return 0;
    }

    if (accountNum < 1 || accountNum > 100)
    {
        printf("✗ Invalid account number.\n");
        return 0;
    }

    printf("Enter Username: ");
    scanf("%19s", username);
    clearInputBuffer();

    printf("Enter First Name: ");
    scanf("%14s", firstName);
    clearInputBuffer();

    printf("Enter Last Name: ");
    scanf("%14s", lastName);
    clearInputBuffer();

    printf("Enter PIN (4 digits): ");
    scanf("%4s", pinStr);
    clearInputBuffer();

    pin = atoi(pinStr);
    if (!validatePIN(pin))
    {
        printf("✗ Invalid PIN. Must be 4 digits.\n");
        return 0;
    }

    printf("Enter Initial Balance: ");
    scanf("%lf", &initialBalance);
    clearInputBuffer();

    if (initialBalance < 0)
    {
        printf("✗ Invalid balance amount.\n");
        return 0;
    }

    // Initialize new account
    newAccount.accountNum = accountNum;
    strcpy(newAccount.username, username);
    strcpy(newAccount.firstName, firstName);
    strcpy(newAccount.lastName, lastName);
    newAccount.pin = pin;
    newAccount.balance = initialBalance;
    newAccount.isLocked = 0;
    newAccount.failedAttempts = 0;
    newAccount.lastLogin = time(NULL);
    newAccount.dailyWithdrawn = 0.0;
    newAccount.withdrawalDate = time(NULL);

    if (saveAccount(&newAccount))
    {
        printf("\n✓ Account created successfully!\n");
        printf("Account Number: %u\n", accountNum);
        printf("Username: %s\n", username);
        logTransaction("ACCOUNT_CREATED", "New account created");
        return 1;
    }
    else
    {
        printf("✗ Failed to create account.\n");
        return 0;
    }
}

int main(void)
{
    int choice;
    Account *currentAccount = NULL;

    // Initialize database
    initializeDatabase();

    while (1)
    {
        displayMainMenu();
        scanf("%d", &choice);
        clearInputBuffer();

        switch (choice)
        {
        case 1:
            printf("\n");
            printf("╔─────────────────────────────────────────────────────────╗\n");
            printf("║ CUSTOMER LOGIN                                          ║\n");
            printf("╚─────────────────────────────────────────────────────────╝\n");

            currentAccount = loginUser();
            if (currentAccount)
            {
                atmInterface(currentAccount);
                free(currentAccount);
                currentAccount = NULL;
            }
            break;

        case 2:
            createNewAccount();
            break;

        case 3:
            printf("\n");
            printf("╔─────────────────────────────────────────────────────────╗\n");
            printf("║ ADMIN MODE LOGIN                                        ║\n");
            printf("╚─────────────────────────────────────────────────────────╝\n");

            printf("Enter Admin PIN (default: 1234): ");
            int adminPin;
            scanf("%d", &adminPin);
            clearInputBuffer();

            if (adminPin == 1234)
            {
                adminMode();
            }
            else
            {
                printf("✗ Invalid admin PIN.\n");
            }
            break;

        case 4:
            printf("\n╔═══════════════════════════════════════════════════════════╗\n");
            printf("║          Thank you for using our ATM service!            ║\n");
            printf("║                    Have a nice day!                      ║\n");
            printf("╚═══════════════════════════════════════════════════════════╝\n\n");
            exit(0);

        default:
            printf("✗ Invalid choice. Please try again.\n");
        }
    }

    return 0;
}
