#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

// Structure from the banking system
typedef struct {
    unsigned int accountNum;
    char firstName[15];
    char lastName[15];
    double balance;
} SimpleAccount;

typedef struct {
    unsigned int accountNum;
    char transactionType[20];
    double amount;
    double balanceAfter;
    time_t timestamp;
    char status[20];
} Transaction;

// Analytics structures
typedef struct {
    int totalTransactions;
    double totalWithdrawn;
    double totalDeposited;
    double highestTransaction;
    double lowestTransaction;
    double averageTransaction;
    char mostFrequentType[20];
} AccountAnalytics;

// Function prototypes
void displayHeader(const char *title);
void analyzeAllAccounts(void);
void generateFinancialReport(void);
void displayTransactionStats(void);
void createAccountSummary(void);
void generateRiskAssessment(void);

// Utility functions
void printBox(const char *text, int width);
void printBar(int width);
double calculateStandardDeviation(double *values, int count);

int main(void)
{
    int choice;

    while (1)
    {
        printf("\n");
        printf("╔════════════════════════════════════════════════════════════╗\n");
        printf("║     BANKING SYSTEM - ANALYTICS & REPORTING DASHBOARD      ║\n");
        printf("║                    Advanced Analytics Module               ║\n");
        printf("╠════════════════════════════════════════════════════════════╣\n");
        printf("║  1. Account Analytics Report                              ║\n");
        printf("║  2. Financial Summary & Statistics                        ║\n");
        printf("║  3. Transaction Analysis                                  ║\n");
        printf("║  4. Account Summary Cards                                 ║\n");
        printf("║  5. Risk Assessment Report                                ║\n");
        printf("║  6. Full System Report (All Options)                      ║\n");
        printf("║  7. Exit                                                  ║\n");
        printf("╚════════════════════════════════════════════════════════════╝\n");
        printf("\nEnter your choice: ");

        scanf("%d", &choice);
        getchar(); // Clear input buffer

        switch (choice)
        {
        case 1:
            analyzeAllAccounts();
            break;
        case 2:
            generateFinancialReport();
            break;
        case 3:
            displayTransactionStats();
            break;
        case 4:
            createAccountSummary();
            break;
        case 5:
            generateRiskAssessment();
            break;
        case 6:
            printf("\n\n");
            analyzeAllAccounts();
            printf("\n\n");
            generateFinancialReport();
            printf("\n\n");
            displayTransactionStats();
            printf("\n\n");
            createAccountSummary();
            printf("\n\n");
            generateRiskAssessment();
            break;
        case 7:
            printf("\n╔════════════════════════════════════════════════════════════╗\n");
            printf("║          Thank you for using Analytics Dashboard!         ║\n");
            printf("║                  Goodbye! Have a nice day!               ║\n");
            printf("╚════════════════════════════════════════════════════════════╝\n\n");
            exit(0);
        default:
            printf("✗ Invalid choice. Please try again.\n");
        }

        printf("\nPress Enter to continue...");
        getchar();
    }

    return 0;
}

void displayHeader(const char *title)
{
    printf("\n");
    printf("╔════════════════════════════════════════════════════════════╗\n");
    printf("║ %-60s║\n", title);
    printf("╠════════════════════════════════════════════════════════════╣\n");
}

void printBar(int width)
{
    for (int i = 0; i < width; i++)
        printf("═");
    printf("\n");
}

void analyzeAllAccounts(void)
{
    displayHeader("ACCOUNT ANALYTICS REPORT");

    FILE *file = fopen("accounts.dat", "rb");
    if (!file)
    {
        printf("No account data found.\n");
        printf("╚════════════════════════════════════════════════════════════╝\n");
        return;
    }

    SimpleAccount account;
    double totalBalance = 0;
    int accountCount = 0;
    double highestBalance = 0;
    double lowestBalance = 999999;
    char highestName[30], lowestName[30];

    printf("║ Account#  Name                    Balance         Status    ║\n");
    printf("╟────────────────────────────────────────────────────────────╢\n");

    while (fread(&account, sizeof(SimpleAccount), 1, file) == 1)
    {
        if (account.accountNum != 0)
        {
            accountCount++;
            totalBalance += account.balance;

            if (account.balance > highestBalance)
            {
                highestBalance = account.balance;
                snprintf(highestName, sizeof(highestName), "%s %s", 
                        account.firstName, account.lastName);
            }

            if (account.balance < lowestBalance && account.balance > 0)
            {
                lowestBalance = account.balance;
                snprintf(lowestName, sizeof(lowestName), "%s %s",
                        account.firstName, account.lastName);
            }

            // Determine status
            const char *status = (account.balance < 1000) ? "⚠️  Low" :
                                 (account.balance > 50000) ? "✓ High" : "Normal";

            printf("║ %-8u  %-18s   Rs. %10.2f    %s\n",
                   account.accountNum,
                   account.firstName,
                   account.balance,
                   status);
        }
    }

    fclose(file);

    double avgBalance = (accountCount > 0) ? (totalBalance / accountCount) : 0;

    printf("╟────────────────────────────────────────────────────────────╢\n");
    printf("║ SUMMARY STATISTICS:                                        ║\n");
    printf("║  Total Accounts:        %d                                 ║\n", accountCount);
    printf("║  Total System Balance:  Rs. %-45.2f║\n", totalBalance);
    printf("║  Average Balance:       Rs. %-45.2f║\n", avgBalance);
    printf("║  Highest Account:       Rs. %-45.2f║\n", highestBalance);
    printf("║  Lowest Active:         Rs. %-45.2f║\n", lowestBalance);
    printf("║                                                            ║\n");
    printf("║  Richest Account:       %s\n", highestName);
    printf("║  %-59s║\n", "");
    printf("║  Lowest Balance:        %s\n", lowestName);
    printf("║  %-59s║\n", "");
    printf("╚════════════════════════════════════════════════════════════╝\n");
}

void generateFinancialReport(void)
{
    displayHeader("FINANCIAL SUMMARY & STATISTICS");

    FILE *file = fopen("transactions.log", "r");
    if (!file)
    {
        printf("No transaction data found.\n");
        printf("╚════════════════════════════════════════════════════════════╝\n");
        return;
    }

    char line[256];
    double totalWithdrawals = 0, totalDeposits = 0;
    int withdrawalCount = 0, depositCount = 0;
    int successCount = 0, failureCount = 0;

    while (fgets(line, sizeof(line), file))
    {
        if (strstr(line, "Withdrawal") && strstr(line, "Success"))
        {
            char *amount_start = strstr(line, "Amount: ");
            if (amount_start)
            {
                double amount;
                sscanf(amount_start, "Amount: %lf", &amount);
                totalWithdrawals += amount;
                withdrawalCount++;
                successCount++;
            }
        }
        else if (strstr(line, "Deposit") && strstr(line, "Success"))
        {
            char *amount_start = strstr(line, "Amount: ");
            if (amount_start)
            {
                double amount;
                sscanf(amount_start, "Amount: %lf", &amount);
                totalDeposits += amount;
                depositCount++;
                successCount++;
            }
        }
        else if (strstr(line, "Success") == NULL && strstr(line, "Amount:"))
        {
            failureCount++;
        }
    }

    fclose(file);

    double avgWithdrawal = (withdrawalCount > 0) ? (totalWithdrawals / withdrawalCount) : 0;
    double avgDeposit = (depositCount > 0) ? (totalDeposits / depositCount) : 0;

    printf("║ TRANSACTION SUMMARY:                                       ║\n");
    printf("║  Total Withdrawals:     Rs. %-44.2f║\n", totalWithdrawals);
    printf("║  Total Deposits:        Rs. %-44.2f║\n", totalDeposits);
    printf("║  Net Flow:              Rs. %-44.2f║\n", totalDeposits - totalWithdrawals);
    printf("║                                                            ║\n");
    printf("║ TRANSACTION COUNT:                                         ║\n");
    printf("║  Withdrawal Count:      %-49d║\n", withdrawalCount);
    printf("║  Deposit Count:         %-49d║\n", depositCount);
    printf("║  Successful:            %-49d║\n", successCount);
    printf("║  Failed:                %-49d║\n", failureCount);
    printf("║                                                            ║\n");
    printf("║ AVERAGES:                                                  ║\n");
    printf("║  Avg Withdrawal Amount: Rs. %-44.2f║\n", avgWithdrawal);
    printf("║  Avg Deposit Amount:    Rs. %-44.2f║\n", avgDeposit);
    printf("║  Success Rate:          %-48.1f%%║\n",
           ((successCount + failureCount) > 0) ? 
           ((double)successCount / (successCount + failureCount) * 100) : 0);
    printf("╚════════════════════════════════════════════════════════════╝\n");
}

void displayTransactionStats(void)
{
    displayHeader("TRANSACTION ANALYSIS REPORT");

    FILE *file = fopen("transactions.log", "r");
    if (!file)
    {
        printf("No transaction data found.\n");
        printf("╚════════════════════════════════════════════════════════════╝\n");
        return;
    }

    char line[256];
    int inquiries = 0, fastcash = 0, changes = 0;
    int highValueTrans = 0;  // > 5000
    int mediumValueTrans = 0; // 1000-5000
    int lowValueTrans = 0;    // < 1000

    while (fgets(line, sizeof(line), file))
    {
        if (strstr(line, "Balance Inquiry"))
            inquiries++;
        else if (strstr(line, "FastCash"))
            fastcash++;
        else if (strstr(line, "PIN_CHANGE"))
            changes++;

        // Parse transaction value
        char *amount_start = strstr(line, "Amount: ");
        if (amount_start)
        {
            double amount;
            sscanf(amount_start, "Amount: %lf", &amount);

            if (amount > 5000)
                highValueTrans++;
            else if (amount >= 1000)
                mediumValueTrans++;
            else if (amount > 0)
                lowValueTrans++;
        }
    }

    fclose(file);

    printf("║ TRANSACTION TYPE DISTRIBUTION:                             ║\n");
    printf("║  Balance Inquiries:     %-49d║\n", inquiries);
    printf("║  Fast Cash Withdrawals: %-49d║\n", fastcash);
    printf("║  PIN Changes:           %-49d║\n", changes);
    printf("║                                                            ║\n");
    printf("║ TRANSACTION VALUE DISTRIBUTION:                            ║\n");
    printf("║  High Value (>5000):    %-49d║\n", highValueTrans);
    printf("║  Medium (1000-5000):    %-49d║\n", mediumValueTrans);
    printf("║  Low (<1000):           %-49d║\n", lowValueTrans);
    printf("║                                                            ║\n");
    printf("║ ANALYSIS:                                                  ║\n");
    printf("║  Most Common: %s                              ║\n",
           (inquiries > fastcash) ? "Balance Inquiries" : "Fast Cash");
    int total = inquiries + fastcash;
    printf("║  Inquiry Rate: %.1f%% of transactions                     ║\n",
           (total > 0) ? ((double)inquiries / total * 100) : 0);
    printf("╚════════════════════════════════════════════════════════════╝\n");
}

void createAccountSummary(void)
{
    displayHeader("INDIVIDUAL ACCOUNT SUMMARY CARDS");

    FILE *file = fopen("accounts.dat", "rb");
    if (!file)
    {
        printf("No account data found.\n");
        printf("╚════════════════════════════════════════════════════════════╝\n");
        return;
    }

    SimpleAccount account;
    int count = 0;

    while (fread(&account, sizeof(SimpleAccount), 1, file) == 1)
    {
        if (account.accountNum != 0)
        {
            count++;
            time_t now = time(NULL);

            printf("\n┌────────────────────────────────────────────────────────┐\n");
            printf("│ ACCOUNT CARD #%d\n", count);
            printf("├────────────────────────────────────────────────────────┤\n");
            printf("│ Account Number:       %-44u │\n", account.accountNum);
            printf("│ Account Holder:       %-44s │\n", account.firstName);
            printf("│ Current Balance:      Rs. %-38.2f │\n", account.balance);
            printf("│ Account Status:       %-44s │\n", 
                   (account.balance > 0) ? "✓ ACTIVE" : "⚠ INACTIVE");
            printf("│                                                        │\n");

            // Balance analysis
            if (account.balance > 100000)
                printf("│ Category:             %-44s │\n", "Premium Account");
            else if (account.balance > 50000)
                printf("│ Category:             %-44s │\n", "Standard Account");
            else if (account.balance > 10000)
                printf("│ Category:             %-44s │\n", "Basic Account");
            else
                printf("│ Category:             %-44s │\n", "Low Balance");

            printf("└────────────────────────────────────────────────────────┘\n");
        }
    }

    fclose(file);

    printf("\nTotal Accounts Processed: %d\n", count);
    printf("╚════════════════════════════════════════════════════════════╝\n");
}

void generateRiskAssessment(void)
{
    displayHeader("RISK ASSESSMENT REPORT");

    FILE *file = fopen("accounts.dat", "rb");
    if (!file)
    {
        printf("No account data found.\n");
        printf("╚════════════════════════════════════════════════════════════╝\n");
        return;
    }

    SimpleAccount account;
    int criticalRisk = 0;  // Balance < 500
    int highRisk = 0;      // Balance < 5000
    int mediumRisk = 0;    // Balance 5000-20000
    int lowRisk = 0;       // Balance > 20000

    while (fread(&account, sizeof(SimpleAccount), 1, file) == 1)
    {
        if (account.accountNum != 0)
        {
            if (account.balance < 500)
                criticalRisk++;
            else if (account.balance < 5000)
                highRisk++;
            else if (account.balance < 20000)
                mediumRisk++;
            else
                lowRisk++;
        }
    }

    fclose(file);

    int totalAccounts = criticalRisk + highRisk + mediumRisk + lowRisk;

    printf("║ ACCOUNT RISK CLASSIFICATION:                              ║\n");
    printf("║                                                            ║\n");
    printf("║ 🔴 CRITICAL (< Rs. 500):    %-37d║\n", criticalRisk);
    if (totalAccounts > 0)
        printf("║    Percentage:              %-37.1f%%║\n",
               ((double)criticalRisk / totalAccounts * 100));
    printf("║                                                            ║\n");
    printf("║ 🟠 HIGH RISK (Rs. 500-5000):%-37d║\n", highRisk);
    if (totalAccounts > 0)
        printf("║    Percentage:              %-37.1f%%║\n",
               ((double)highRisk / totalAccounts * 100));
    printf("║                                                            ║\n");
    printf("║ 🟡 MEDIUM (Rs. 5000-20000): %-37d║\n", mediumRisk);
    if (totalAccounts > 0)
        printf("║    Percentage:              %-37.1f%%║\n",
               ((double)mediumRisk / totalAccounts * 100));
    printf("║                                                            ║\n");
    printf("║ 🟢 LOW RISK (> Rs. 20000):  %-37d║\n", lowRisk);
    if (totalAccounts > 0)
        printf("║    Percentage:              %-37.1f%%║\n",
               ((double)lowRisk / totalAccounts * 100));
    printf("║                                                            ║\n");
    printf("║ RECOMMENDATIONS:                                           ║\n");
    if (criticalRisk > (totalAccounts * 0.2))
        printf("║  ⚠  Alert: High number of critical accounts detected    ║\n");
    if (highRisk > (totalAccounts * 0.3))
        printf("║  ⚠  Alert: Monitor high-risk accounts closely           ║\n");
    if (lowRisk > (totalAccounts * 0.5))
        printf("║  ✓  Healthy: Majority of accounts are well-funded       ║\n");
    printf("╚════════════════════════════════════════════════════════════╝\n");
}
