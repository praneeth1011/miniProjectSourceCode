#ifndef BANKING_H
#define BANKING_H

#include <time.h>

// Constants
#define MAX_USERNAME_LEN 20
#define MAX_PASSWORD_LEN 20
#define MAX_ACCOUNTS 100
#define MAX_TRANSACTIONS 1000
#define DAILY_WITHDRAWAL_LIMIT 5000.00
#define MAX_PIN_ATTEMPTS 3
#define MIN_WITHDRAWAL 100.00
#define MAX_WITHDRAWAL 10000.00

// Data structures

// Account structure
typedef struct {
    unsigned int accountNum;
    char username[MAX_USERNAME_LEN];
    char firstName[15];
    char lastName[15];
    int pin;
    double balance;
    int isLocked;           // 0 = unlocked, 1 = locked
    int failedAttempts;     // count of failed PIN attempts
    time_t lastLogin;
    double dailyWithdrawn;  // amount withdrawn today
    time_t withdrawalDate;  // date of daily withdrawal tracking
} Account;

// Transaction structure
typedef struct {
    unsigned int accountNum;
    char transactionType[20];  // "Withdrawal", "Deposit", "Inquiry", "FastCash"
    double amount;
    double balanceAfter;
    time_t timestamp;
    char status[20];           // "Success", "Failed"
    char remarks[100];
} Transaction;

// Session structure
typedef struct {
    unsigned int accountNum;
    char username[MAX_USERNAME_LEN];
    time_t loginTime;
    time_t logoutTime;
    int isActive;
} Session;

// Function prototypes

// Main menu functions (banking.c)
void displayMainMenu(void);
void displayATMMenu(void);
void atmInterface(Account *currentAccount);

// Authentication functions (auth.c)
int authenticateUser(Account *account, int pin);
Account* loginUser(void);
void logout(Session *session);

// File operations (fileops.c)
void initializeDatabase(void);
int loadAccount(unsigned int accountNum, Account *account);
int saveAccount(Account *account);
int saveTransaction(Transaction *trans);
int loadAllAccounts(Account accounts[], int *count);
void displayAllAccounts(void);
void loadTransactionHistory(unsigned int accountNum);
int accountExists(unsigned int accountNum);

// Transaction functions (transactions.c)
void withdrawCash(Account *currentAccount);
void depositMoney(Account *currentAccount);
void balanceInquiry(Account *currentAccount);
void displayMiniStatement(unsigned int accountNum);
void fastCashWithdrawal(Account *currentAccount);
void processTransaction(Account *account, Transaction *trans, const char *type, double amount, int success);

// Admin functions (admin.c)
void adminMode(void);
void displayAdminMenu(void);
void viewAllTransactions(void);
void lockAccount(void);
void unlockAccount(void);
void displaySessionLogs(void);

// Utility functions (utils.c)
void clearInputBuffer(void);
void displayFormattedOutput(const char *format, ...);
int validatePIN(int pin);
int validateAmount(double amount);
void updateDailyWithdrawalLimit(Account *account);
void logTransaction(const char *action, const char *details);
void displayHeader(const char *title);
void displayFooter(void);
void delay(int milliseconds);

#endif // BANKING_H
