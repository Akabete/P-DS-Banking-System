#ifndef ACCOUNTS_H
#define ACCOUNTS_H

#include "definitions.h"

    bool is_acc_no_free(int potential_acc_no);
    int generate_account_number();
    bool validate_name(const char *potential_name);
    void input_name(char *target_buffor, const char *text, int max_length);
    void input_address(account_data *potential_name);
    void input_pesel(account_data *potential_pesel);
    bool validate_pesel(const char *potential_pesel);
    void create_account();
    void print_accounts();

#endif