#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "banking.h"

#define ACCOUNTS_FILE "accounts.dat"
#define TRANSACTIONS_FILE "transactions.log"

void initializeDatabase(void)
{
    // Create accounts file if it doesn't exist
    FILE *file = fopen(ACCOUNTS_FILE, "rb");
    if (!file)
    {
        file = fopen(ACCOUNTS_FILE, "wb");
        if (file)
        {
            fclose(file);
            printf("✓ Accounts database initialized.\n");
            delay(500);
        }
    }
    else
    {
        fclose(file);
    }

    // Create transactions file if it doesn't exist
    file = fopen(TRANSACTIONS_FILE, "a");
    if (file)
    {
        fclose(file);
    }
}

int loadAccount(unsigned int accountNum, Account *account)
{
    FILE *file = fopen(ACCOUNTS_FILE, "rb");
    if (!file)
    {
        return 0;
    }

    Account temp;
    while (fread(&temp, sizeof(Account), 1, file) == 1)
    {
        if (temp.accountNum == accountNum)
        {
            *account = temp;
            fclose(file);
            return 1;
        }
    }

    fclose(file);
    return 0;
}

int saveAccount(Account *account)
{
    if (!account)
        return 0;

    FILE *file = fopen(ACCOUNTS_FILE, "r+b");
    if (!file)
    {
        file = fopen(ACCOUNTS_FILE, "wb");
        if (!file)
            return 0;
    }

    Account temp;
    int found = 0;

    // Search for existing account
    rewind(file);
    while (fread(&temp, sizeof(Account), 1, file) == 1)
    {
        if (temp.accountNum == account->accountNum)
        {
            // Move back and overwrite
            fseek(file, -(long)sizeof(Account), SEEK_CUR);
            fwrite(account, sizeof(Account), 1, file);
            found = 1;
            break;
        }
    }

    // If not found, append new account
    if (!found)
    {
        fseek(file, 0, SEEK_END);
        fwrite(account, sizeof(Account), 1, file);
    }

    fclose(file);
    return 1;
}

int saveTransaction(Transaction *trans)
{
    if (!trans)
        return 0;

    FILE *file = fopen(TRANSACTIONS_FILE, "a");
    if (!file)
        return 0;

    fprintf(file, "==========================================\n");
    fprintf(file, "Account Number: %u\n", trans->accountNum);
    fprintf(file, "Transaction Type: %s\n", trans->transactionType);
    fprintf(file, "Amount: %.2f\n", trans->amount);
    fprintf(file, "Balance After: %.2f\n", trans->balanceAfter);
    fprintf(file, "Status: %s\n", trans->status);
    fprintf(file, "Time: %s", ctime(&trans->timestamp));
    if (strlen(trans->remarks) > 0)
    {
        fprintf(file, "Remarks: %s\n", trans->remarks);
    }
    fprintf(file, "==========================================\n\n");

    fclose(file);
    return 1;
}

int loadAllAccounts(Account accounts[], int *count)
{
    FILE *file = fopen(ACCOUNTS_FILE, "rb");
    if (!file)
        return 0;

    *count = 0;
    while (fread(&accounts[*count], sizeof(Account), 1, file) == 1)
    {
        if (accounts[*count].accountNum != 0)
        {
            (*count)++;
            if (*count >= MAX_ACCOUNTS)
                break;
        }
    }

    fclose(file);
    return 1;
}

void displayAllAccounts(void)
{
    Account accounts[MAX_ACCOUNTS];
    int count = 0;

    if (!loadAllAccounts(accounts, &count))
    {
        printf("✗ Failed to load accounts.\n");
        return;
    }

    if (count == 0)
    {
        printf("✗ No accounts found.\n");
        return;
    }

    printf("\n");
    printf("╔═══════════════════════════════════════════════════════════════════════════╗\n");
    printf("║%-8s%-15s%-15s%-15s%-10s%-15s║\n", "Acct#", "Username", "First Name", "Last Name", "Balance", "Status");
    printf("╠═══════════════════════════════════════════════════════════════════════════╣\n");

    for (int i = 0; i < count; i++)
    {
        printf("║%-8u%-15s%-15s%-15s%-10.2f%-15s║\n",
               accounts[i].accountNum,
               accounts[i].username,
               accounts[i].firstName,
               accounts[i].lastName,
               accounts[i].balance,
               accounts[i].isLocked ? "LOCKED" : "ACTIVE");
    }

    printf("╚═══════════════════════════════════════════════════════════════════════════╝\n");
}

void loadTransactionHistory(unsigned int accountNum)
{
    FILE *file = fopen(TRANSACTIONS_FILE, "r");
    if (!file)
    {
        printf("✗ No transaction history found.\n");
        return;
    }

    char line[256];
    int found = 0;

    printf("\n");
    printf("╔═══════════════════════════════════════════════════════════╗\n");
    printf("║            TRANSACTION HISTORY                            ║\n");
    printf("╚═══════════════════════════════════════════════════════════╝\n");

    while (fgets(line, sizeof(line), file))
    {
        if (strstr(line, "Account Number:"))
        {
            unsigned int acctNum;
            sscanf(line, "Account Number: %u", &acctNum);
            if (acctNum == accountNum)
            {
                found = 1;
                // Print this transaction
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
        }
    }

    fclose(file);

    if (!found)
    {
        printf("No transactions found for this account.\n");
    }
}

int accountExists(unsigned int accountNum)
{
    Account account;
    return loadAccount(accountNum, &account);
}
