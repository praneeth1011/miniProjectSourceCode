/*
 * UTILITY MODULE HEADER
 * Helper functions for input/output, date/time, validation
 */

#ifndef UTILS_H
#define UTILS_H

#include "banking_system.h"

// Input validation
bool validate_email(char *email);
bool validate_phone(char *phone);
bool validate_account_number(int acct_num);
bool validate_pin(int pin);
bool validate_amount(double amount);

// Input functions
int get_integer_input(char *prompt);
double get_double_input(char *prompt);
void get_string_input(char *prompt, char *buffer, int size);
int get_menu_choice(int min, int max);

// Display utilities
void clear_screen();
void print_header(char *title);
void print_line(int length);
void print_success(char *message);
void print_error(char *message);
void print_info(char *message);
void pause_screen();

// Date/Time utilities
char* get_current_date();
char* get_current_time();
int get_days_difference(time_t date1, time_t date2);

// File utilities
bool file_exists(char *filename);
void backup_file(char *filename);

// PIN encoding (basic)
int simple_encode_pin(int pin);
int simple_decode_pin(int encoded);

// ATM denomination
void calculate_atm_denominations(double amount, ATMDenomination *denom);
void display_atm_denominations(ATMDenomination denom);

#endif // UTILS_H
