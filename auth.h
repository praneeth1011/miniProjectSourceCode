/*
 * AUTHENTICATION MODULE HEADER
 * Handles login, PIN verification, session management
 */

#ifndef AUTH_H
#define AUTH_H

#include "banking_system.h"
#include "utils.h"
#include "accounts.h"

// Function prototypes
bool authenticate_user(Account *accounts, int count, int account_number, int pin);
void encode_pin(int pin, int *encoded);
bool verify_pin(int pin, int encoded_pin);
bool login_customer(Account *accounts, int count, int *logged_in_account);
void handle_failed_login(Account *account);
void reset_failed_attempts(Account *account);
void logout_customer(Account *account);

#endif // AUTH_H
