/*
 * UTILITY MODULE IMPLEMENTATION
 * Helper functions for input/output, validation, and utilities
 */

#include "utils.h"
#include <math.h>
#include <ctype.h>

// ============= INPUT VALIDATION =============

bool validate_email(char *email) {
    // Simple email validation - check for @ and .
    int at_pos = -1, dot_pos = -1;
    for (int i = 0; email[i] != '\0'; i++) {
        if (email[i] == '@') at_pos = i;
        if (email[i] == '.') dot_pos = i;
    }
    return (at_pos > 0 && dot_pos > at_pos + 1);
}

bool validate_phone(char *phone) {
    // Phone should be 10 digits
    if (strlen(phone) != 10) return false;
    for (int i = 0; i < 10; i++) {
        if (!isdigit(phone[i])) return false;
    }
    return true;
}

bool validate_account_number(int acct_num) {
    return (acct_num >= MIN_ACCOUNT_NUMBER && acct_num <= MAX_ACCOUNT_NUMBER);
}

bool validate_pin(int pin) {
    return (pin >= 1000 && pin <= 9999);
}

bool validate_amount(double amount) {
    return (amount > 0 && amount <= 9999999);
}

// ============= INPUT FUNCTIONS =============

int get_integer_input(char *prompt) {
    int value;
    char buffer[100];
    printf("%s", prompt);
    fgets(buffer, sizeof(buffer), stdin);
    sscanf(buffer, "%d", &value);
    return value;
}

double get_double_input(char *prompt) {
    double value;
    char buffer[100];
    printf("%s", prompt);
    fgets(buffer, sizeof(buffer), stdin);
    sscanf(buffer, "%lf", &value);
    return value;
}

void get_string_input(char *prompt, char *buffer, int size) {
    printf("%s", prompt);
    fgets(buffer, size, stdin);
    // Remove trailing newline
    buffer[strcspn(buffer, "\n")] = 0;
}

int get_menu_choice(int min, int max) {
    int choice;
    bool valid = false;
    while (!valid) {
        choice = get_integer_input("Enter your choice: ");
        if (choice >= min && choice <= max) {
            valid = true;
        } else {
            print_error("Invalid choice! Please try again.");
        }
    }
    return choice;
}

// ============= DISPLAY UTILITIES =============

void clear_screen() {
    system("cls"); // For Windows
}

void print_header(char *title) {
    printf("\n");
    print_line(70);
    printf("%-70s\n", title);
    print_line(70);
}

void print_line(int length) {
    for (int i = 0; i < length; i++) {
        printf("=");
    }
    printf("\n");
}

void print_success(char *message) {
    printf("\n[SUCCESS] %s\n\n", message);
}

void print_error(char *message) {
    printf("\n[ERROR] %s\n\n", message);
}

void print_info(char *message) {
    printf("\n[INFO] %s\n\n", message);
}

void pause_screen() {
    printf("\nPress ENTER to continue...");
    getchar();
}

// ============= DATE/TIME UTILITIES =============

char* get_current_date() {
    static char date_str[DATE_SIZE];
    time_t now = time(NULL);
    struct tm *timeinfo = localtime(&now);
    strftime(date_str, DATE_SIZE, "%Y-%m-%d", timeinfo);
    return date_str;
}

char* get_current_time() {
    static char time_str[DATE_SIZE];
    time_t now = time(NULL);
    struct tm *timeinfo = localtime(&now);
    strftime(time_str, DATE_SIZE, "%H:%M:%S", timeinfo);
    return time_str;
}

int get_days_difference(time_t date1, time_t date2) {
    return (int)((date2 - date1) / (60 * 60 * 24));
}

// ============= FILE UTILITIES =============

bool file_exists(char *filename) {
    FILE *file = fopen(filename, "r");
    if (file) {
        fclose(file);
        return true;
    }
    return false;
}

void backup_file(char *filename) {
    char backup_name[FILENAME_SIZE * 2];
    sprintf(backup_name, "%s.backup", filename);
    
    FILE *original = fopen(filename, "rb");
    if (!original) return;
    
    FILE *backup = fopen(backup_name, "wb");
    if (!backup) {
        fclose(original);
        return;
    }
    
    char buffer[1024];
    size_t bytes;
    while ((bytes = fread(buffer, 1, sizeof(buffer), original)) > 0) {
        fwrite(buffer, 1, bytes, backup);
    }
    
    fclose(original);
    fclose(backup);
}

// ============= PIN ENCODING (SIMPLE) =============

int simple_encode_pin(int pin) {
    int encoded = 0;
    int offset = 1234;
    int temp = pin;
    int reversed = 0;
    
    while (temp > 0) {
        reversed = reversed * 10 + (temp % 10);
        temp /= 10;
    }
    
    encoded = reversed + offset;
    return encoded;
}

int simple_decode_pin(int encoded) {
    int offset = 1234;
    int temp = encoded - offset;
    int decoded = 0;
    
    while (temp > 0) {
        decoded = decoded * 10 + (temp % 10);
        temp /= 10;
    }
    
    return decoded;
}

// ============= ATM DENOMINATION CALCULATION =============

void calculate_atm_denominations(double amount, ATMDenomination *denom) {
    int total = (int)amount;
    
    denom->denomination_500 = total / 500;
    total %= 500;
    
    denom->denomination_200 = total / 200;
    total %= 200;
    
    denom->denomination_100 = total / 100;
}

void display_atm_denominations(ATMDenomination denom) {
    printf("\n%-20s %-10s %-15s\n", "Denomination", "Count", "Total");
    printf("%-20s %-10s %-15s\n", "-----------", "-----", "-----");
    printf("Rs. 500           : %3d       x %7d\n", denom.denomination_500, 500);
    printf("Rs. 200           : %3d       x %7d\n", denom.denomination_200, 200);
    printf("Rs. 100           : %3d       x %7d\n", denom.denomination_100, 100);
}
