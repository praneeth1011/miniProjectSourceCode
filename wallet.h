/*
 * WALLET MODULE HEADER (Digital Wallet / UPI-like Transfer)
 * Handles money transfers between accounts with transaction ID generation
 */

#ifndef WALLET_H
#define WALLET_H

#include "banking_system.h"
#include "accounts.h"
#include "transactions.h"
#include "utils.h"

// Function prototypes
int generate_transaction_id_wallet();
bool transfer_money(Account *sender, Account *receiver, double amount, 
                    Transaction *transactions, int *trans_count);
void display_transfer_receipt(int transaction_id, int sender_acct, int receiver_acct, 
                             double amount);
bool verify_receiver_account(Account *accounts, int count, int account_number);

#endif // WALLET_H
