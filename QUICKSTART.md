# Quick Start Guide - Banking System

## 5-Minute Setup

### Step 1: Navigate to Project Directory
```bash
cd miniProjectSourceCode
```

### Step 2: Compile the Program

**On Linux/Mac:**
```bash
make
```

**On Windows (with MinGW):**
```bash
mingw32-make
# OR
gcc -o banking_system banking.c auth.c fileops.c transactions.c admin.c utils.c -lm
```

### Step 3: Run the Program

**On Linux/Mac:**
```bash
make run
# OR
./banking_system
```

**On Windows:**
```bash
banking_system.exe
```

## First Time Use

1. **Main Menu appears** - Select option "2" (Create New Account)
2. **Create Test Account**:
   - Account Number: 1
   - Username: testuser
   - First Name: Test
   - Last Name: User
   - PIN: 1234
   - Initial Balance: 10000

3. **Login** - Option "1" (Customer Login)
   - Account Number: 1
   - PIN: 1234

4. **Try Transactions**:
   - Check balance (Option 1)
   - Deposit money (Option 3) - Try 5000
   - Fast Cash (Option 4) - Select 1000
   - Mini Statement (Option 5) - See your transactions
   - Logout (Option 7)

5. **Admin Mode** - Option "3" from Main Menu
   - PIN: 1234
   - View all accounts
   - Check transaction history
   - Monitor system

## Key Features to Test

### Withdrawal Limits
- Try withdrawing Rs. 15000 (exceeds daily limit)
- Try withdrawing Rs. 50000 (exceeds single withdrawal limit)
- Try withdrawing Rs. 500 (below minimum)

### Account Locking
- Create an account
- Try logging in with wrong PIN 3 times
- Account will lock automatically
- Use Admin mode to unlock it

### Transaction History
- Make multiple transactions
- Use Mini Statement to view them
- Use Admin mode to see all transactions

### Session Management
- Login and logout multiple times
- Check sessions.log for session tracking
- Admin mode shows all session logs

## File Locations

After running the program, these files are created:

| File | Purpose |
|------|---------|
| accounts.dat | Account data (binary) |
| transactions.log | Transaction history |
| sessions.log | Login/logout tracking |
| system.log | System events |

## Troubleshooting

### "Command not found" on Windows
Use the full path or run from the project directory:
```bash
cd miniProjectSourceCode
banking_system.exe
```

### "File permission denied"
Ensure you have write permissions in the directory.

### "Compilation error"
Make sure all 6 source files are present:
- banking.c
- auth.c
- fileops.c
- transactions.c
- admin.c
- utils.c

And the header file:
- banking.h

### Admin PIN forgotten
The default admin PIN is: **1234**

## Default Test Credentials

| Detail | Value |
|--------|-------|
| Admin PIN | 1234 |
| Test Account Number | 1 |
| Test PIN | 1234 |
| Test Balance | 10000 |

## Next Steps

1. **Read DOCUMENTATION.md** for complete feature list
2. **Review source code** to understand implementation
3. **Examine log files** to see transaction tracking
4. **Create multiple accounts** to test multi-user functionality
5. **Test all admin features** to verify system integrity

## Common Tasks

### View All Accounts
1. Select Admin Mode (PIN: 1234)
2. Select "1" to view all accounts

### Check Transaction History
1. Select Admin Mode
2. Select "2" to view all transactions
3. Or select "6" to view specific account details

### Lock an Account (Security)
1. Select Admin Mode
2. Select "3" to lock account
3. Enter target account number

### Reset System
```bash
# Clean all data files (WARNING: Deletes all data!)
make distclean

# Recompile
make

# Run with fresh database
./banking_system
```

---

**Ready to go!** Your banking system is now set up and ready to use.

For detailed information, see [DOCUMENTATION.md](DOCUMENTATION.md)
