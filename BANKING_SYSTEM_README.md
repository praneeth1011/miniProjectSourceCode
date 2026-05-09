# Complete Banking System in C

A comprehensive, modular console-based banking system with 12 advanced features built entirely in C.

## 🎯 Features (12+)

### 1. **Account Management**
   - Create new accounts with auto-generated account numbers
   - Store account details: name, email, phone, age, address, account type
   - View account information
   - Delete/modify accounts
   - Track account creation and last login dates

### 2. **Authentication System**
   - 4-digit PIN login system
   - PIN encoding (basic security)
   - Failed login attempt tracking
   - Account locking after 3 failed attempts
   - Session management

### 3. **Banking Operations**
   - Deposit money into account
   - Withdraw with overdraft prevention
   - Check account balance
   - Daily withdrawal limit (Rs. 50,000)
   - Real-time balance updates

### 4. **Complete Transaction History**
   - Record all transactions (deposits, withdrawals, transfers, EMI payments)
   - View last N transactions
   - Transaction timestamps and complete details
   - Transaction persistence to file

### 5. **Digital Wallet (UPI-like Transfers)**
   - Transfer money between accounts
   - Generate unique transaction IDs
   - Transfer receipts
   - Bidirectional transaction recording

### 6. **Comprehensive Loan System**
   - Apply for loans
   - Credit score-based loan approval
   - Intelligent EMI calculation
   - Monthly EMI payment tracking
   - Remaining balance monitoring
   - Interest rates adjusted by credit score

### 7. **Fixed Deposit System**
   - Create FDs with different durations (6 months, 1 year, 2 years, 3 years)
   - Automatic interest rate calculation (5.5% - 7.5% p.a.)
   - Maturity amount calculation
   - Early withdrawal with penalty (1%)
   - Auto-maturity detection

### 8. **Credit Score System**
   - Initial score: 500/900
   - Increases for positive behavior
   - Decreases for suspicious activities
   - Affects loan eligibility and interest rates
   - Real-time score updates

### 9. **Fraud Detection System**
   - Detect multiple withdrawals in short time
   - Identify unusually large transactions
   - Track failed login attempts
   - Flag suspicious accounts
   - Maintain fraud log with timestamps
   - Admin account review and unflagging

### 10. **ATM Simulation**
   - Withdraw using standard denominations (Rs. 100, 200, 500)
   - Calculate optimal denomination breakdown
   - Daily withdrawal limit enforcement
   - Account authentication required

### 11. **Queue Management (Bank Visit Simulation)**
   - Token-based queue system
   - Customer service tracking
   - Fair customer ordering (FIFO)
   - Wait time calculation
   - Token history

### 12. **Admin Panel**
   - View all accounts
   - Search accounts (by number or name)
   - Sort accounts by balance
   - Delete accounts
   - View fraud alerts
   - Generate daily/statistical reports
   - Reset daily withdrawal limits
   - System-wide statistics

## 📁 Project Structure

```
banking_system/
├── banking_system.h         # Main header with structures & enums
├── accounts.h/c             # Account management module
├── auth.h/c                 # Authentication & login
├── transactions.h/c         # Transaction handling
├── wallet.h/c              # Digital wallet/transfers
├── loans.h/c               # Loan management
├── fixed_deposit.h/c       # FD system
├── fraud.h/c               # Fraud detection
├── queue.h/c               # Queue/token system
├── admin.h/c               # Admin panel
├── utils.h/c               # Utility functions
├── main_banking.c          # Main program
├── Makefile_new            # Build configuration
└── Data files (generated):
    ├── accounts.dat        # Account data
    ├── transactions.dat    # Transaction log
    ├── loans.dat          # Loan information
    ├── fixeddeposits.dat  # FD records
    └── fraud_log.dat      # Fraud logs
```

## 🔧 Building & Compilation

### Prerequisites
- GCC compiler (MinGW on Windows, standard GCC on Linux/Mac)
- Make utility

### Build Commands

```bash
# Navigate to project directory
cd miniProjectSourceCode

# Build the project
make -f Makefile_new

# Build and run
make -f Makefile_new run

# Clean build artifacts
make -f Makefile_new clean

# Remove all data files
make -f Makefile_new distclean
```

### Manual Compilation (without Make)
```bash
gcc -o banking_system main_banking.c accounts.c auth.c transactions.c utils.c \
    wallet.c loans.c fixed_deposit.c fraud.c queue.c admin.c -lm
```

## 🚀 Running the Application

```bash
./banking_system
```

## 📋 Main Menu Quick Guide

### New User
1. **Create Account** (Option 1)
   - Enter personal details
   - Choose account type (Savings/Current/Salary)
   - Set 4-digit PIN
   - Receive account number

### Existing User  
2. **Customer Login** (Option 2)
   - Enter account number & PIN
   - Access personal banking menu

### Admin Access
3. **Admin Panel** (Option 3)
   - View all accounts
   - Search and manage accounts
   - Generate reports
   - Monitor fraudulent activities

### Special Features
4. **Token System** (Option 4) - Get token for bank visit
5. **ATM Withdrawal** (Option 5) - Withdraw with denominations
6. **Exit** (Option 6)

## 💳 Customer Operations Menu

Once logged in, customers can:
- ✅ Check balance
- ✅ Deposit money
- ✅ Withdraw money
- ✅ View transactions
- ✅ Transfer money (UPI)
- ✅ Apply for loans
- ✅ Create Fixed Deposits
- ✅ View credit score
- ✅ View account details

## 🔒 Security Features

- 4-digit PIN encoding
- Failed login attempt tracking
- Account locking mechanism
- Fraud detection system
- Suspicious activity flagging
- Transaction logging

## 📊 Data Persistence

All data is automatically saved to binary files:
- **accounts.dat** - Account information
- **transactions.dat** - All transactions
- **loans.dat** - Loan details
- **fixeddeposits.dat** - FD information
- **fraud_log.dat** - Fraud logs

Data is loaded automatically on startup.

## 🧮 Technical Details

### Structures Used
- **Account** - Complete customer profile
- **Transaction** - Individual transaction record
- **Loan** - Loan details with EMI tracking
- **FixedDeposit** - FD information with maturity
- **FraudLog** - Suspicious activity logs
- **QueueNode** - Bank visit tokens

### Key Functions
- **Account Management**: find_account, create_account, delete_account
- **Authentication**: authenticate_user, verify_pin, login_customer
- **Banking**: deposit, withdraw, check_balance
- **Transfers**: transfer_money, record_transaction
- **Loans**: apply_loan, calculate_emi, pay_emi
- **Admin**: generate_statistics, sort_accounts_by_balance

### Error Handling
- Input validation for all operations
- File I/O error checking
- Memory allocation verification
- Balance availability checks
- Limit enforcement

## 📚 Modular Architecture

Each major feature is in a separate module with:
- Dedicated header file (.h)
- Implementation file (.c)
- Clear function prototypes
- Well-commented code
- Beginner-friendly logic

## 💡 Sample Workflow

```
1. START APPLICATION
2. CREATE ACCOUNT
   - Name: John Doe
   - Email: john@example.com
   - Phone: 9876543210
   - PIN: 1234
   → Account: 1000 created
3. LOGIN
   - Account: 1000
   - PIN: 1234
4. DEPOSIT
   - Amount: Rs. 10000
   → Balance: Rs. 10000
5. TRANSFER
   - To: Account 1001
   - Amount: Rs. 2000
   → Balance: Rs. 8000
6. VIEW TRANSACTION
   → See transaction history
7. LOGOUT
```

## ⚠️ Limitations & Future Enhancements

### Current Limitations
- Terminal-based UI (no GUI)
- Basic PIN encoding (not cryptographically secure)
- Single-user admin access
- No database (uses flat files)

### Possible Enhancements
- Database integration (MySQL/SQLite)
- GUI using GTK/Qt
- Multi-user admin accounts
- Proper cryptographic hashing
- Email notifications
- Mobile APP API

## 📝 Code Comments

All code is heavily commented with:
- Function descriptions
- Parameter explanations
- Logic flow documentation
- Edge case notes
- Beginner-friendly explanations

## ✅ Testing Recommendations

1. **Create Test Accounts**
   - Account 1: Savings
   - Account 2: Current
   - Account 3: Salary

2. **Test Banking Operations**
   - Deposit/Withdraw
   - Check limits
   - Transfer between accounts

3. **Test Fraud Detection**
   - Multiple quick withdrawals
   - Large transactions
   - Failed logins

4. **Test Admin Functions**
   - View all accounts
   - Generate reports
   - Sort by balance

## 📞 Support

For issues or questions:
1. Check function comments in source files
2. Verify input formats
3. Ensure sufficient balance for operations
4. Recompile if modifications made

---

**Build Status**: ✅ Ready to Compile & Run
**Code Quality**: ✅ Production-Grade Modular Design
**Documentation**: ✅ Comprehensive & Beginner-Friendly

Happy Banking! 🏦💰
