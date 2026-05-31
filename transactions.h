#ifndef TRANSACTIONS_H
#define TRANSACTIONS_H

    bool confirm_operation();
    void add_balance_manager();
    void remove_balance_manager();
    void transfer_manager();
    bool add_balance(int target_acc, float balance_change);
    void add_balance_menu(unsigned short *target_acc, float *balance_change);
    bool remove_balance(int target_acc, float balance_change);
    void remove_balance_menu(unsigned short *target_acc, float *balance_change);
    void transfer_money(int source_acc, int target_acc, float amount);
    void transfer_money_menu(unsigned short *source_acc, unsigned short *target_acc, float *balance_change);


#endif