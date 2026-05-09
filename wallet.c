/*
 * WALLET MODULE IMPLEMENTATION (UPI-like Transfers)
 */

#include "wallet.h"

static int wallet_transaction_id = 9000;

int generate_transaction_id_wallet() {
    return ++wallet_transaction_id;
}

bool transfer_money(Account *sender, Account *receiver, double amount, 
                    Transaction *transactions, int *trans_count) {
    
    if (!validate_amount(amount)) {
        print_error("Invalid amount");
        return false;
    }
    
    if (amount > sender->balance) {
        print_error("Insufficient balance!");
        update_credit_score(sender, -10);
        return false;
    }
    
    // Deduct from sender
    sender->balance -= amount;
    record_transaction(transactions, trans_count, sender->account_number,
                      TRANSACTION_TRANSFER, amount, sender->balance, 
                      "Transfer Sent", receiver->account_number);
    update_credit_score(sender, 5);
    
    // Add to receiver
    receiver->balance += amount;
    record_transaction(transactions, trans_count, receiver->account_number,
                      TRANSACTION_RECEIVED, amount, receiver->balance,
                      "Transfer Received", sender->account_number);
    update_credit_score(receiver, 5);
    
    return true;
}

void display_transfer_receipt(int transaction_id, int sender_acct, int receiver_acct, 
                             double amount) {
    printf("\n");
    print_header("TRANSFER RECEIPT");
    printf("%-40s: TXN%d\n", "Transaction ID", transaction_id);
    printf("%-40s: %d\n", "From Account", sender_acct);
    printf("%-40s: %d\n", "To Account", receiver_acct);
    printf("%-40s: Rs. %.2f\n", "Amount", amount);
    printf("%-40s: %s\n", "Date", get_current_date());
    printf("%-40s: %s\n", "Time", get_current_time());
    printf("%-40s: %s\n", "Status", "SUCCESS");
    print_line(70);
}

bool verify_receiver_account(Account *accounts, int count, int account_number) {
    return (find_account(accounts, count, account_number) != NULL);
}
