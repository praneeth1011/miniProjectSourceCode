#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "banking.h"

void updateDailyWithdrawalLimit(Account *account)
{
    time_t now = time(NULL);
    struct tm *timeinfo = localtime(&now);
    struct tm *lastWithdrawalInfo = localtime(&account->withdrawalDate);

    // Reset daily withdrawal if different day
    if (timeinfo->tm_mday != lastWithdrawalInfo->tm_mday ||
        timeinfo->tm_mon != lastWithdrawalInfo->tm_mon ||
        timeinfo->tm_year != lastWithdrawalInfo->tm_year)
    {
        account->dailyWithdrawn = 0.0;
        account->withdrawalDate = now;
    }
}

void processTransaction(Account *account, Transaction *trans, const char *type, double amount, int success)
{
    if (!account || !trans)
        return;

    trans->accountNum = account->accountNum;
    strcpy(trans->transactionType, type);
    trans->amount = amount;
    trans->balanceAfter = account->balance;
    trans->timestamp = time(NULL);
    strcpy(trans->status, success ? "Success" : "Failed");

    saveTransaction(trans);
}

void withdrawCash(Account *currentAccount)
{
    if (!currentAccount)
    {
        printf("✗ Invalid account.\n");
        return;
    }

    double amount;
    Transaction trans;
    memset(&trans, 0, sizeof(Transaction));

    printf("Current Balance: Rs. %.2f\n\n", currentAccount->balance);

    printf("Enter amount to withdraw: Rs. ");
    scanf("%lf", &amount);
    clearInputBuffer();

    // Update daily withdrawal limit
    updateDailyWithdrawalLimit(currentAccount);

    // Validate amount
    if (amount <= 0)
    {
        printf("✗ Invalid amount.\n");
        strcpy(trans.remarks, "Invalid amount entered");
        processTransaction(currentAccount, &trans, "Withdrawal", amount, 0);
        return;
    }

    if (amount < MIN_WITHDRAWAL)
    {
        printf("✗ Minimum withdrawal amount: Rs. %.2f\n", MIN_WITHDRAWAL);
        strcpy(trans.remarks, "Amount below minimum limit");
        processTransaction(currentAccount, &trans, "Withdrawal", amount, 0);
        return;
    }

    if (amount > MAX_WITHDRAWAL)
    {
        printf("✗ Maximum single withdrawal amount: Rs. %.2f\n", MAX_WITHDRAWAL);
        strcpy(trans.remarks, "Amount exceeds single withdrawal limit");
        processTransaction(currentAccount, &trans, "Withdrawal", amount, 0);
        return;
    }

    if (currentAccount->dailyWithdrawn + amount > DAILY_WITHDRAWAL_LIMIT)
    {
        printf("✗ Daily withdrawal limit exceeded!\n");
        printf("Daily limit: Rs. %.2f\n", DAILY_WITHDRAWAL_LIMIT);
        printf("Already withdrawn today: Rs. %.2f\n", currentAccount->dailyWithdrawn);
        printf("Remaining limit: Rs. %.2f\n", DAILY_WITHDRAWAL_LIMIT - currentAccount->dailyWithdrawn);
        strcpy(trans.remarks, "Daily withdrawal limit exceeded");
        processTransaction(currentAccount, &trans, "Withdrawal", amount, 0);
        return;
    }

    if (amount > currentAccount->balance)
    {
        printf("✗ Insufficient balance!\n");
        printf("Your balance: Rs. %.2f\n", currentAccount->balance);
        strcpy(trans.remarks, "Insufficient balance");
        processTransaction(currentAccount, &trans, "Withdrawal", amount, 0);
        return;
    }

    // Process withdrawal
    currentAccount->balance -= amount;
    currentAccount->dailyWithdrawn += amount;

    if (saveAccount(currentAccount))
    {
        printf("\n✓ Withdrawal successful!\n");
        printf("Amount withdrawn: Rs. %.2f\n", amount);
        printf("New balance: Rs. %.2f\n", currentAccount->balance);
        processTransaction(currentAccount, &trans, "Withdrawal", amount, 1);
    }
    else
    {
        printf("✗ Transaction failed. Please try again.\n");
        currentAccount->balance += amount;
        currentAccount->dailyWithdrawn -= amount;
        saveAccount(currentAccount);
        processTransaction(currentAccount, &trans, "Withdrawal", amount, 0);
    }
}

void depositMoney(Account *currentAccount)
{
    if (!currentAccount)
    {
        printf("✗ Invalid account.\n");
        return;
    }

    double amount;
    Transaction trans;
    memset(&trans, 0, sizeof(Transaction));

    printf("Current Balance: Rs. %.2f\n\n", currentAccount->balance);

    printf("Enter amount to deposit: Rs. ");
    scanf("%lf", &amount);
    clearInputBuffer();

    // Validate amount
    if (amount <= 0)
    {
        printf("✗ Invalid amount.\n");
        strcpy(trans.remarks, "Invalid amount entered");
        processTransaction(currentAccount, &trans, "Deposit", amount, 0);
        return;
    }

    // Process deposit
    currentAccount->balance += amount;

    if (saveAccount(currentAccount))
    {
        printf("\n✓ Deposit successful!\n");
        printf("Amount deposited: Rs. %.2f\n", amount);
        printf("New balance: Rs. %.2f\n", currentAccount->balance);
        processTransaction(currentAccount, &trans, "Deposit", amount, 1);
    }
    else
    {
        printf("✗ Transaction failed. Please try again.\n");
        currentAccount->balance -= amount;
        saveAccount(currentAccount);
        processTransaction(currentAccount, &trans, "Deposit", amount, 0);
    }
}

void balanceInquiry(Account *currentAccount)
{
    if (!currentAccount)
    {
        printf("✗ Invalid account.\n");
        return;
    }

    Transaction trans;
    memset(&trans, 0, sizeof(Transaction));

    printf("\n");
    printf("╔─────────────────────────────────────────╗\n");
    printf("║  Account Holder: %s %s\n", currentAccount->firstName, currentAccount->lastName);
    printf("║  Account Number: %u\n", currentAccount->accountNum);
    printf("║  Current Balance: Rs. %.2f\n", currentAccount->balance);
    printf("╚─────────────────────────────────────────╝\n");

    updateDailyWithdrawalLimit(currentAccount);
    printf("\nDaily withdrawal limit remaining: Rs. %.2f\n", 
           DAILY_WITHDRAWAL_LIMIT - currentAccount->dailyWithdrawn);

    processTransaction(currentAccount, &trans, "Balance Inquiry", 0, 1);
}

void displayMiniStatement(unsigned int accountNum)
{
    FILE *file = fopen("transactions.log", "r");
    if (!file)
    {
        printf("✗ No transactions found.\n");
        return;
    }

    char line[256];
    int transactionCount = 0;

    printf("\n");
    printf("╔═══════════════════════════════════════════════════════════╗\n");
    printf("║            MINI STATEMENT (Last 10 Transactions)          ║\n");
    printf("╚═══════════════════════════════════════════════════════════╝\n");

    // Count total transactions for this account
    while (fgets(line, sizeof(line), file))
    {
        if (strstr(line, "Account Number:"))
        {
            unsigned int acctNum;
            sscanf(line, "Account Number: %u", &acctNum);
            if (acctNum == accountNum)
            {
                transactionCount++;
            }
        }
    }

    // Display last 10 transactions
    rewind(file);
    int skipFirst = (transactionCount > 10) ? (transactionCount - 10) : 0;
    int currentTrans = 0;

    while (fgets(line, sizeof(line), file))
    {
        if (strstr(line, "Account Number:"))
        {
            unsigned int acctNum;
            sscanf(line, "Account Number: %u", &acctNum);
            if (acctNum == accountNum)
            {
                if (currentTrans >= skipFirst)
                {
                    printf("%s", line);
                    for (int i = 0; i < 5; i++)
                    {
                        if (fgets(line, sizeof(line), file))
                        {
                            printf("%s", line);
                        }
                    }
                    printf("\n");
                }
                else
                {
                    // Skip this transaction
                    for (int i = 0; i < 5; i++)
                    {
                        fgets(line, sizeof(line), file);
                    }
                }
                currentTrans++;
            }
        }
    }

    fclose(file);

    if (transactionCount == 0)
    {
        printf("No transactions found for this account.\n");
    }
}

void fastCashWithdrawal(Account *currentAccount)
{
    if (!currentAccount)
    {
        printf("✗ Invalid account.\n");
        return;
    }

    int choice;
    double amount;
    Transaction trans;
    memset(&trans, 0, sizeof(Transaction));

    printf("Current Balance: Rs. %.2f\n\n", currentAccount->balance);

    printf("╔─────────────────────────────────────────┐\n");
    printf("║  Select Fast Cash Amount:                │\n");
    printf("║  1. Rs. 500                              │\n");
    printf("║  2. Rs. 1000                             │\n");
    printf("║  3. Rs. 2000                             │\n");
    printf("║  4. Rs. 5000                             │\n");
    printf("║  5. Custom Amount                        │\n");
    printf("║  6. Cancel                               │\n");
    printf("╚─────────────────────────────────────────┘\n");

    printf("Enter your choice: ");
    scanf("%d", &choice);
    clearInputBuffer();

    switch (choice)
    {
    case 1:
        amount = 500;
        break;
    case 2:
        amount = 1000;
        break;
    case 3:
        amount = 2000;
        break;
    case 4:
        amount = 5000;
        break;
    case 5:
        printf("Enter amount: Rs. ");
        scanf("%lf", &amount);
        clearInputBuffer();
        break;
    case 6:
        printf("Transaction cancelled.\n");
        return;
    default:
        printf("✗ Invalid choice.\n");
        return;
    }

    // Update daily withdrawal limit
    updateDailyWithdrawalLimit(currentAccount);

    // Validate amount
    if (amount <= 0)
    {
        printf("✗ Invalid amount.\n");
        strcpy(trans.remarks, "Invalid amount");
        processTransaction(currentAccount, &trans, "FastCash", amount, 0);
        return;
    }

    if (currentAccount->dailyWithdrawn + amount > DAILY_WITHDRAWAL_LIMIT)
    {
        printf("✗ Daily withdrawal limit exceeded!\n");
        printf("Remaining limit: Rs. %.2f\n", DAILY_WITHDRAWAL_LIMIT - currentAccount->dailyWithdrawn);
        strcpy(trans.remarks, "Daily limit exceeded");
        processTransaction(currentAccount, &trans, "FastCash", amount, 0);
        return;
    }

    if (amount > currentAccount->balance)
    {
        printf("✗ Insufficient balance!\n");
        strcpy(trans.remarks, "Insufficient balance");
        processTransaction(currentAccount, &trans, "FastCash", amount, 0);
        return;
    }

    // Process withdrawal
    currentAccount->balance -= amount;
    currentAccount->dailyWithdrawn += amount;

    if (saveAccount(currentAccount))
    {
        printf("\n✓ Fast cash withdrawal successful!\n");
        printf("Amount withdrawn: Rs. %.2f\n", amount);
        printf("New balance: Rs. %.2f\n", currentAccount->balance);
        processTransaction(currentAccount, &trans, "FastCash", amount, 1);
    }
    else
    {
        printf("✗ Transaction failed.\n");
        currentAccount->balance += amount;
        currentAccount->dailyWithdrawn -= amount;
        saveAccount(currentAccount);
        processTransaction(currentAccount, &trans, "FastCash", amount, 0);
    }
}
