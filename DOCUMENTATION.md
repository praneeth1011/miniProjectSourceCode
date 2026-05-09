# Multi-User ATM and Banking Simulation System in C - Complete Documentation

A comprehensive, interactive banking simulation system that replicates a real ATM network with multiple user accounts, secure authentication, and advanced banking features.

## Features

### Core Features
- ✅ **Multi-User Account Management**: Support for up to 100 accounts with persistent binary file storage
- ✅ **User Authentication**: Secure login with PIN-based authentication
- ✅ **ATM-Style Menu Interface**: Professional formatted console interface with easy navigation
- ✅ **Transaction Tracking**: Complete transaction history with timestamps and details
- ✅ **Account Management**: Create, manage, and track multiple independent accounts

### Interactive Enhancements
- ✅ **ATM Constraints**:
  - Daily withdrawal limit (Rs. 5000)
  - Single withdrawal limit (Rs. 100 - Rs. 10000)
  - Insufficient balance handling
  - Invalid PIN attempt tracking (lock after 3 consecutive failures)

- ✅ **Fast Cash Withdrawal**: Quick access to predefined amounts (500, 1000, 2000, 5000) or custom amounts
- ✅ **Mini Statement Generator**: View last 10 transactions with full details
- ✅ **Formatted Console Output**: Professional, user-friendly interface with borders and formatting

### Advanced Features
- ✅ **Session Management**: Track all user login/logout sessions with duration
- ✅ **Admin Control Panel**: Monitor all user activities and account status
- ✅ **Account Locking/Unlocking**: Security mechanism for compromised accounts
- ✅ **Transaction Logging**: Detailed transaction history in structured format
- ✅ **System Logging**: Track all system events and admin actions

## Project Structure

```
├── banking.h              # Main header file with structures & function prototypes
├── banking.c              # Main program, ATM menu, session management
├── auth.c                 # User authentication & login/logout
├── fileops.c              # File operations for accounts & transactions
├── transactions.c         # Transaction processing (withdrawal, deposit, etc.)
├── admin.c                # Admin control panel features
├── utils.c                # Utility functions (validation, logging, etc.)
├── Makefile               # Build configuration
└── README.md              # This file

Generated Files:
├── accounts.dat           # Binary file storing account data
├── transactions.log       # Transaction history
├── sessions.log           # User session logs
└── system.log             # System event logs
```

## Data Structures

### Account Structure
```c
struct {
    unsigned int accountNum;      // Account number (1-100)
    char username[20];            // Username
    char firstName[15];           // First name
    char lastName[15];            // Last name
    int pin;                      // 4-digit PIN
    double balance;               // Current balance
    int isLocked;                 // Account status (0=active, 1=locked)
    int failedAttempts;           // Failed login attempts
    time_t lastLogin;             // Last login timestamp
    double dailyWithdrawn;        // Amount withdrawn today
    time_t withdrawalDate;        // Date of withdrawal tracking
}
```

### Transaction Structure
```c
struct {
    unsigned int accountNum;      // Associated account
    char transactionType[20];     // Type (Withdrawal, Deposit, etc.)
    double amount;                // Transaction amount
    double balanceAfter;          // Balance after transaction
    time_t timestamp;             // Transaction time
    char status[20];              // Success/Failed
    char remarks[100];            // Additional notes
}
```

### Session Structure
```c
struct {
    unsigned int accountNum;      // Account number
    char username[20];            // Username
    time_t loginTime;             // Login timestamp
    time_t logoutTime;            // Logout timestamp
    int isActive;                 // Session status
}
```

## System Constraints

- **Daily Withdrawal Limit**: Rs. 5000
- **Single Withdrawal Limit**: Rs. 100 - Rs. 10000
- **PIN Attempts**: Lock account after 3 failed attempts
- **Maximum Accounts**: 100
- **PIN Format**: 4-digit number (1000-9999)
- **Account Number Range**: 1-100

## Compilation

### Prerequisites
- GCC compiler (Linux/Mac) or MinGW (Windows)
- Standard C library

### On Linux/Mac
```bash
cd miniProjectSourceCode
make              # Compile the program
make run          # Compile and run
make clean        # Remove object files
make distclean    # Remove all generated files
make rebuild      # Clean and recompile
```

### On Windows (with MinGW)
```bash
cd miniProjectSourceCode
mingw32-make      # or use: gcc -o banking_system *.c -lm
```

## Usage Guide

### Starting the Program
```bash
./banking_system
```

### Main Menu Options

**1. Customer Login**
- Enter account number
- Enter PIN
- Access ATM menu for transactions

**2. Create New Account**
- Enter unique account number (1-100)
- Set username, name, and PIN
- Set initial balance
- Account is immediately active

**3. Admin Mode**
- Default PIN: 1234
- View all accounts
- Monitor transactions
- Lock/unlock accounts
- View session logs

### ATM Menu Options

After login, users can:
- **Balance Inquiry**: Check current account balance
- **Withdraw Cash**: Withdraw with automatic validation
- **Deposit Money**: Add funds to account
- **Fast Cash**: Quick withdrawal of predefined amounts
- **Mini Statement**: View last 10 transactions
- **Change PIN**: Update security PIN
- **Logout**: End session securely

### Admin Panel Features

Administrators can:
- View all accounts and their balances
- Monitor all transactions in the system
- Lock suspicious accounts
- Unlock previously locked accounts
- View user login/logout session logs
- View detailed account information
- Access transaction history per account

## Transaction Processing

### Withdrawal
- Validates PIN and account status
- Checks daily and single transaction limits
- Verifies sufficient balance
- Updates account balance
- Logs transaction

### Deposit
- Validates amount
- Adds funds to account
- Records transaction
- Maintains balance integrity

### Balance Inquiry
- Displays current balance
- Shows daily withdrawal remaining limit
- Logs inquiry for audit trail

### Fast Cash
- Offers quick withdrawal options
- Applies same validation rules
- Single-click transactions

## File Management

### accounts.dat
Binary file storing all account records. Each account occupies a fixed-size slot, allowing random access for quick lookups and updates.

### transactions.log
Text file storing all transaction details with readable format including:
- Account information
- Transaction type and amount
- Balance after transaction
- Status (Success/Failed)
- Timestamp
- Remarks

### sessions.log
Log file tracking user session information:
- Login time
- Logout time
- Session duration
- Account details

### system.log
System event log tracking administrative actions and important events.

## Security Features

1. **PIN Protection**: 4-digit PIN required for authentication
2. **Account Locking**: Automatic lock after 3 failed PIN attempts
3. **Session Management**: Track all user sessions
4. **Admin Control**: Secure admin access with PIN (default: 1234)
5. **Audit Trail**: Complete transaction and activity logging
6. **Input Validation**: Comprehensive validation of all user inputs
7. **Balance Integrity**: Atomic transaction processing

## Validation Rules

### PIN Validation
- Must be 4 digits (1000-9999)
- Verified on every login attempt
- Account locks after 3 failures

### Amount Validation
- Must be positive
- Minimum: Rs. 100
- Maximum: Rs. 10000 per transaction
- Daily limit: Rs. 5000

### Account Number Validation
- Range: 1-100
- Must be unique for new accounts
- Verified before operations

## Sample Usage Scenario

```
1. System starts -> Main Menu displayed
2. User selects "Create New Account"
   - Account Number: 1
   - Username: john_doe
   - Name: John Doe
   - PIN: 1234
   - Balance: 10000
3. User selects "Customer Login"
   - Account: 1
   - PIN: 1234
   - Login successful
4. ATM Menu displayed
5. User selects "Deposit Money"
   - Deposits: 5000
   - New balance: 15000
6. User selects "Mini Statement"
   - Shows deposit transaction
7. User selects "Withdraw Cash"
   - Withdraws: 2000
   - New balance: 13000
8. User selects "Logout"
   - Session saved
   - Returns to main menu
9. Admin logs in
   - Verifies transactions
   - Checks account status
10. Exit program
```

## Error Handling

The system handles various error conditions:
- Invalid PIN attempts
- Insufficient balance
- Daily limit exceeded
- Account locked
- Invalid inputs
- File operation failures
- Memory allocation errors

## Performance Considerations

- **Binary file format**: Fast random access to account data
- **Indexed lookups**: Direct account retrieval by account number
- **Efficient logging**: Appended transaction logs
- **Memory management**: Dynamic allocation where needed

## Future Enhancements

Possible improvements:
1. Network support for multiple ATM terminals
2. Database integration (SQLite/MySQL)
3. Advanced encryption for sensitive data
4. Mobile app interface
5. Email/SMS notifications
6. Investment and loan modules
7. Multi-currency support
8. Scheduled transfers
9. Account statement export (PDF)
10. Two-factor authentication

## Testing Recommendations

### Test Accounts

To test the system, create accounts with these details:

**Account 1 - Test User**
- Account Number: 1
- Username: testuser
- Name: Test User
- PIN: 1234
- Initial Balance: 10000

**Account 2 - Demo Account**
- Account Number: 2
- Username: demo
- Name: Demo Account
- PIN: 5678
- Initial Balance: 5000

## Known Limitations

1. Limited to 100 accounts (can be extended by modifying MAX_ACCOUNTS)
2. Single process operation (no concurrent transactions)
3. No actual ATM card simulation
4. Local file-based storage (no network capability)
5. Console-based interface (no GUI)

## Troubleshooting

### Compilation Issues
```bash
# If compilation fails, ensure all .c and .h files are in the same directory
# Make sure gcc/mingw32-make is installed and in PATH

# For Windows MinGW:
gcc -o banking_system banking.c auth.c fileops.c transactions.c admin.c utils.c -lm
```

### File Protection Errors
```bash
# If you get permission errors, ensure write permissions in the directory
# Delete accounts.dat, transactions.log if corrupted and restart
```

### Admin PIN Error
```bash
# Default admin PIN: 1234
# If forgotten, modify the hardcoded value in banking.c
```

## Key Implementation Highlights

### Modular Design
- Separate modules for authentication, transactions, admin features
- Header file with all function prototypes
- Clear separation of concerns

### File-Based Storage
- Binary format for random access performance
- Transaction logs in human-readable format
- Session tracking and audit logs

### Comprehensive Validation
- PIN format and length validation
- Amount range checks
- Daily withdrawal limit enforcement
- Account status verification

### User-Friendly Interface
- Professional formatted menus with borders
- Clear error messages
- Confirmation messages for transactions
- Navigation prompts

## Technical Specifications

**Language**: C (C99 standard)
**Compiler**: GCC or compatible
**Platform**: Windows, Linux, macOS
**File Format**: Binary (accounts), Text (logs)
**Storage**: File-based (no database)
**Concurrency**: Single-user per session

## Author Notes

This system demonstrates:
- Modular programming principles
- File I/O operations (binary and text)
- Data structures and memory management
- User authentication and security
- Session management
- System logging and audit trails
- Error handling
- Input validation

---

**System Version**: 1.0 (Production Ready)
**Last Updated**: March 2026
**Purpose**: Educational Banking System Simulation
