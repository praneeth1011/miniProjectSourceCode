# Testing Guide for Banking System

## System Testing Plan

This document provides comprehensive testing scenarios and procedures to verify all features of the Multi-User ATM and Banking System.

## Pre-Testing Setup

1. **Compile the program:**
   ```bash
   gcc -o banking_system *.c -lm
   ```

2. **Verify executable creation:**
   ```bash
   ls -la banking_system.exe  # Windows
   ./banking_system --version  # Verify (if available)
   ```

3. **Ensure write permissions** in the project directory for creating data files.

---

## Test Cases

### Module 1: Account Creation

#### Test 1.1: Create Valid Account
**Steps:**
1. Run program
2. Select option 2 (Create New Account)
3. Enter Account Number: 1
4. Enter Username: testuser1
5. Enter First Name: Test
6. Enter Last Name: User
7. Enter PIN: 1234
8. Enter Initial Balance: 10000

**Expected Result:**
- ✓ Account created successfully
- ✓ Confirmation message displayed
- ✓ accounts.dat file created/updated

**Verification:**
- Check that account number 1 exists in system
- Can login with account 1 and PIN 1234

---

#### Test 1.2: Create Account with Duplicate Account Number
**Steps:**
1. Create Account 1 (Test 1.1)
2. Try to create Account 1 again

**Expected Result:**
- ✗ Error message: "Account already exists!"

---

#### Test 1.3: Create Account with Invalid Account Number
**Steps:**
1. Try Account Number: 0
2. Try Account Number: 101
3. Try Account Number: -5

**Expected Result:**
- ✗ Error: "Invalid account number."

---

#### Test 1.4: Create Multiple Accounts
**Steps:**
1. Create Account 1: testuser1/1234/10000
2. Create Account 2: testuser2/5678/5000
3. Create Account 5: testuser5/9999/20000
4. Create Account 50: testuser50/4321/15000

**Expected Result:**
- ✓ All accounts created successfully
- ✓ Each maintains independent balance
- ✓ Each has independent PIN

---

### Module 2: User Authentication

#### Test 2.1: Valid Login
**Steps:**
1. Create Account 1 with PIN: 1234
2. Select option 1 (Customer Login)
3. Enter Account Number: 1
4. Enter PIN: 1234

**Expected Result:**
- ✓ Login successful
- ✓ Welcome message shows correct name
- ✓ ATM Menu displayed

---

#### Test 2.2: Invalid PIN - First Attempt
**Steps:**
1. Select option 1 (Customer Login)
2. Enter Account Number: 1
3. Enter PIN: 9999 (wrong)

**Expected Result:**
- ✗ "Incorrect PIN."
- Display: "Attempts remaining: 2"

---

#### Test 2.3: PIN Attempt Lockout
**Steps:**
1. Select option 1 (Customer Login)
2. Enter Account Number: 1
3. Enter wrong PIN three times:
   - Attempt 1: 9999
   - Attempt 2: 8888
   - Attempt 3: 7777

**Expected Result:**
- ✗ After 3rd attempt: "Account locked due to multiple failed attempts!"

**Verification:**
- Account 1 is now locked
- Cannot login with correct PIN
- Admin mode shows account as LOCKED
- Admin must unlock to use again

---

#### Test 2.4: Login with Non-existent Account
**Steps:**
1. Select option 1 (Customer Login)
2. Enter Account Number: 999

**Expected Result:**
- ✗ "Account not found!"

---

### Module 3: Balance Inquiry

#### Test 3.1: Check Balance
**Steps:**
1. Login to Account 1 (Balance: 10000)
2. Select ATM option 1 (Balance Inquiry)

**Expected Result:**
- ✓ Display current balance: 10000
- ✓ Display daily withdrawal remaining: 5000
- ✓ Transaction logged

---

#### Test 3.2: Multiple Balance Inquiries
**Steps:**
1. Login to Account 1
2. Select Balance Inquiry 3 times

**Expected Result:**
- ✓ All inquiries succeed
- ✓ Balance remains unchanged

---

### Module 4: Cash Withdrawal

#### Test 4.1: Valid Withdrawal
**Steps:**
1. Login to Account 1 (Balance: 10000)
2. Select option 2 (Withdraw Cash)
3. Enter amount: 1000

**Expected Result:**
- ✓ Withdrawal successful
- ✓ New balance shown: 9000
- ✓ Transaction logged with amount and new balance

---

#### Test 4.2: Insufficient Balance
**Steps:**
1. Login to Account 1 (Balance: 9000)
2. Select option 2 (Withdraw Cash)
3. Enter amount: 10000

**Expected Result:**
- ✗ "Insufficient balance!"
- Display balance: 9000
- ✗ Balance unchanged

---

#### Test 4.3: Below Minimum Withdrawal
**Steps:**
1. Login to Account 1
2. Select option 2 (Withdraw Cash)
3. Enter amount: 50

**Expected Result:**
- ✗ "Minimum withdrawal amount: Rs. 100"

---

#### Test 4.4: Above Maximum Withdrawal
**Steps:**
1. Login to Account 1
2. Select option 2 (Withdraw Cash)
3. Enter amount: 15000

**Expected Result:**
- ✗ "Maximum single withdrawal amount: Rs. 10000"

---

#### Test 4.5: Daily Withdrawal Limit
**Steps:**
1. Login to Account 1 (Balance: 20000)
2. Withdraw: 3000
3. Withdraw: 2500
4. Try to withdraw: 1000

**Expected Result:**
- ✓ First withdrawal: Success (3000 withdrawn, 17000 remain)
- ✓ Second withdrawal: Success (2500 withdrawn, 14500 remain)
- ✗ Third attempt: "Daily withdrawal limit exceeded!"
- Display: "Remaining limit: Rs. -500"

---

#### Test 4.6: Negative Withdrawal Amount
**Steps:**
1. Login to Account 1
2. Select option 2 (Withdraw Cash)
3. Enter amount: -1000

**Expected Result:**
- ✗ "Invalid amount."

---

### Module 5: Deposit Money

#### Test 5.1: Valid Deposit
**Steps:**
1. Login to Account 1 (Balance: 9000)
2. Select option 3 (Deposit Money)
3. Enter amount: 5000

**Expected Result:**
- ✓ Deposit successful
- ✓ New balance shown: 14000
- ✓ Transaction logged

---

#### Test 5.2: Multiple Deposits
**Steps:**
1. Login to Account 2 (Balance: 5000)
2. Deposit: 1000
3. Deposit: 2000
4. Deposit: 500

**Expected Result:**
- ✓ All deposits successful
- ✓ Final balance: 8500

---

#### Test 5.3: Zero Amount Deposit
**Steps:**
1. Login to Account 1
2. Select option 3 (Deposit Money)
3. Enter amount: 0

**Expected Result:**
- ✗ "Invalid amount."

---

#### Test 5.4: Negative Deposit
**Steps:**
1. Login to Account 1
2. Select option 3 (Deposit Money)
3. Enter amount: -500

**Expected Result:**
- ✗ "Invalid amount."

---

### Module 6: Fast Cash Withdrawal

#### Test 6.1: Fast Cash - Rs. 500
**Steps:**
1. Login to Account 1 (Balance: 14000)
2. Select option 4 (Fast Cash)
3. Select option 1 (Rs. 500)

**Expected Result:**
- ✓ Withdrawal successful: 500
- ✓ New balance: 13500

---

#### Test 6.2: Fast Cash - Custom Amount
**Steps:**
1. Login to Account 1
2. Select option 4 (Fast Cash)
3. Select option 5 (Custom Amount)
4. Enter: 2000

**Expected Result:**
- ✓ Withdrawal successful: 2000
- ✓ Daily withdrawal limit respected

---

#### Test 6.3: Fast Cash - Exceed Daily Limit
**Steps:**
1. Login to Account 1 (Already withdrawn: 3000)
2. Select Fast Cash with Rs. 5000

**Expected Result:**
- ✗ "Daily withdrawal limit exceeded!"

---

#### Test 6.4: Fast Cash - Exceed Daily Limit Cumulative
**Steps:**
1. Login to Account 1
2. Fast Cash: Rs. 2000
3. Fast Cash: Rs. 2000
4. Fast Cash: Rs. 1500 (third)

**Expected Result:**
- ✓ First: Success (withdrawn: 2000)
- ✓ Second: Success (withdrawn: 4000)
- ✗ Third: "Daily withdrawal limit exceeded!"

---

### Module 7: Mini Statement

#### Test 7.1: View Mini Statement After Transactions
**Steps:**
1. Create Account 1 with balance 10000
2. Deposit 5000
3. Withdraw 2000
4. Balance Inquiry
5. Login and select option 5 (Mini Statement)

**Expected Result:**
- ✓ Show last transactions in order
- ✓ Display account number
- ✓ Display transaction type
- ✓ Display amount and balance after

---

#### Test 7.2: Empty Mini Statement
**Steps:**
1. Create Account 10 (new account)
2. Login
3. Select option 5 (Mini Statement)

**Expected Result:**
- ✓ Display message: "No transactions found for this account."

---

#### Test 7.3: Mini Statement - Limit to 10 Transactions
**Steps:**
1. Make 15 transactions on Account 1
2. Check mini statement

**Expected Result:**
- ✓ Display only last 10 transactions
- ✓ Oldest 5 transactions not shown

---

### Module 8: Admin Mode

#### Test 8.1: Admin Login with Correct PIN
**Steps:**
1. From main menu, select option 3 (Admin Mode)
2. Enter Admin PIN: 1234

**Expected Result:**
- ✓ "Admin mode activated."
- ✓ Admin menu displayed

---

#### Test 8.2: Admin Login with Wrong PIN
**Steps:**
1. Select option 3 (Admin Mode)
2. Enter PIN: 9999

**Expected Result:**
- ✗ "Invalid admin PIN."
- Return to main menu

---

#### Test 8.3: View All Accounts (Admin)
**Steps:**
1. Login as admin (PIN: 1234)
2. Select option 1 (View All Accounts)

**Expected Result:**
- ✓ Display table of all accounts
- ✓ Show account number, username, name, balance, status

---

#### Test 8.4: View Account Details (Admin)
**Steps:**
1. Login as admin
2. Select option 6 (View Account Details)
3. Enter Account Number: 1

**Expected Result:**
- ✓ Display complete account information:
  - Account number
  - Username
  - Name
  - Balance
  - Status (LOCKED/ACTIVE)
  - Failed attempts
  - Last login time
  - Daily withdrawn
- ✓ Display transaction history for account

---

#### Test 8.5: Lock Account (Admin)
**Steps:**
1. Login as admin
2. Select option 3 (Lock Account)
3. Enter Account Number: 1

**Expected Result:**
- ✓ "Account 1 has been locked."
- ✓ Account 1 now shows status: LOCKED
- ✓ Cannot login to Account 1

---

#### Test 8.6: Unlock Account (Admin)
**Steps:**
1. Account 1 is locked
2. Login as admin
3. Select option 4 (Unlock Account)
4. Enter Account Number: 1

**Expected Result:**
- ✓ "Account 1 has been unlocked."
- ✓ Account 1 now shows status: ACTIVE
- ✓ Can login to Account 1 again
- ✓ Failed attempts reset to 0

---

#### Test 8.7: View All Transactions (Admin)
**Steps:**
1. Login as admin
2. Select option 2 (View All Transactions)

**Expected Result:**
- ✓ Display all transactions for all accounts
- ✓ Show transaction count
- ✓ Each transaction shows status (Success/Failed)

---

#### Test 8.8: View Session Logs (Admin)
**Steps:**
1. Multiple users login and logout
2. Login as admin
3. Select option 5 (View Session Logs)

**Expected Result:**
- ✓ Display all session logs
- ✓ Show login time, logout time, duration
- ✓ Display session count

---

### Module 9: PIN Change

#### Test 9.1: Change PIN
**Steps:**
1. Login to Account 1 (current PIN: 1234)
2. Select option 6 (Change PIN)
3. Enter new PIN: 5555

**Expected Result:**
- ✓ "PIN changed successfully!"
- ✓ Old PIN (1234) no longer works
- ✓ New PIN (5555) works for login

---

#### Test 9.2: Change PIN to Invalid Format
**Steps:**
1. Login to Account 1
2. Select option 6 (Change PIN)
3. Enter: 123 (only 3 digits)

**Expected Result:**
- ✗ "Invalid PIN. Must be 4 digits."

---

### Module 10: Session Management

#### Test 10.1: Single User Session
**Steps:**
1. Login to Account 1
2. Perform transactions
3. Logout

**Expected Result:**
- ✓ Session logged with:
  - Login time
  - Logout time
  - Session duration
- ✓ Can view in admin mode

---

#### Test 10.2: Multiple User Sessions
**Steps:**
1. Login/logout Account 1
2. Login/logout Account 2
3. Login/logout Account 1 again

**Expected Result:**
- ✓ All 3 sessions logged separately
- ✓ Admin can see all sessions
- ✓ Correct account numbers and usernames

---

### Module 11: File Operations

#### Test 11.1: Data Persistence
**Steps:**
1. Create Account 1 and perform transactions
2. Exit program
3. Restart program
4. Login to Account 1

**Expected Result:**
- ✓ Account 1 exists with:
  - Same balance
  - Same PIN
  - Transaction history intact

---

#### Test 11.2: Transaction Log Format
**Steps:**
1. Perform a withdrawal
2. Check transactions.log file

**Expected Result:**
- ✓ Log entries are human-readable
- ✓ Include account number, type, amount
- ✓ Include balance after transaction
- ✓ Include timestamp
- ✓ Include status (Success/Failed)

---

#### Test 11.3: Session Log Format
**Steps:**
1. Login and logout
2. Check sessions.log file

**Expected Result:**
- ✓ Log entries include:
  - Account number
  - Username
  - Login time
  - Logout time
  - Session duration

---

## Edge Cases and Stress Tests

### Test E1: Large Balance Transactions
**Steps:**
1. Create account with balance: 999999.99
2. Deposit: 100000
3. Withdraw: 500000

**Expected Result:**
- ✓ All calculations correct
- ✓ Balance precision maintained

---

### Test E2: Rapid Fire Transactions
**Steps:**
1. Login to Account 1
2. Perform 20 transactions quickly:
   - Deposit, Withdraw, Inquiry (repeatedly)

**Expected Result:**
- ✓ All transactions processed
- ✓ Balance calculations accurate
- ✓ All logged correctly

---

### Test E3: Long Username/Names
**Steps:**
1. Create account with:
   - Username: verylongusernametesting
   - First Name: VeryLongFirstName
   - Last Name: VeryLongLastName

**Expected Result:**
- ✓ Accepted (truncated to max length)
- ✓ Displays correctly

---

### Test E4: Special Characters
**Steps:**
1. Create account with usernames containing:
   - underscores: test_user
   - numbers: user123
   - spaces: test user (if accepted)

**Expected Result:**
- ✓ Handles appropriately
- ✓ No system crash

---

## Regression Testing

After any code modifications:

1. **Test all core features:**
   - Account creation
   - Login/logout
   - Balance inquiry
   - Withdrawal
   - Deposit
   - Fast cash

2. **Verify data integrity:**
   - Balances accurate
   - Transactions logged
   - Sessions tracked

3. **Check admin functions:**
   - View accounts
   - Lock/unlock accounts
   - View transactions
   - View sessions

---

## Performance Testing

### Test P1: System Response Time
- Account creation: < 1 second
- Login: < 1 second
- Withdrawal: < 1 second
- Statement generation: < 2 seconds

### Test P2: Memory Usage
- Reasonable memory usage
- No memory leaks during extended use
- Handles 100 accounts efficiently

---

## Summary Checklist

- [ ] All accounts created successfully
- [ ] Authentication working correctly
- [ ] All transactions (withdrawal, deposit) functioning
- [ ] Fast cash working with limits
- [ ] Mini statement displaying correctly
- [ ] PIN change working
- [ ] Admin mode accessible and functional
- [ ] Session tracking logging correctly
- [ ] Data persistence verified
- [ ] Log files created and formatted properly
- [ ] Account locking working
- [ ] Daily withdrawal limits enforcing correctly
- [ ] Error handling appropriate
- [ ] No compilation warnings
- [ ] Multi-account scenarios working
- [ ] Admin overrides/controls functioning
- [ ] System stable over extended use
- [ ] File operations (read/write) reliable

---

## Reporting Test Results

For each test, record:
1. **Test ID**: Unique identifier
2. **Test Name**: Description
3. **Expected Result**: What should happen
4. **Actual Result**: What actually happened
5. **Status**: PASS/FAIL
6. **Notes**: Any observations or issues
7. **Date**: When test was performed
8. **Tester**: Who performed the test

---

**Test Plan Status**: Production Ready
**Last Updated**: March 2026
