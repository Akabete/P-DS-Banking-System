#include "definitions.h"
#include "transactions.h"
#include <stdio.h>
#include <stdbool.h>

bool confirm_operation(){
    char decision;
    printf("Are you sure you want to continue this operation (Y/N): ");
    while (getchar() != '\n');

    scanf("%c", &decision);

    if (decision == 'y' || decision == 'Y') return true;
    else return false;
}

void add_balance_manager(){
    if (confirm_operation()){
        unsigned short target_acc = 0;
        float balance_change = 0.0;

        add_balance_menu(&target_acc, &balance_change);
        add_balance(target_acc, balance_change);
    }
}

void remove_balance_manager(){
    if (confirm_operation()){
        unsigned short target_acc = 0;
        float balance_change = 0.0;
        remove_balance_menu(&target_acc, &balance_change);
        remove_balance(target_acc, balance_change);
    }
}

void transfer_manager(){
    if (confirm_operation()){
        unsigned short source_acc = 0;
        unsigned short target_acc = 0;
        float balance_change = 0.0;
        
        transfer_money_menu(&source_acc, &target_acc, &balance_change);
        transfer_money(source_acc, target_acc, balance_change);
    }
}

bool add_balance(int target_acc, float balance_change){

    FILE *file = fopen(ACCOUNTS_FILE_NAME, "r+b");
    if (file == NULL) return false;

    account_data temp;

    while (fread(&temp, sizeof(account_data), 1, file)){
        if (temp.account_number == target_acc){
            temp.current_balance += balance_change;

            fseek(file, -sizeof(account_data), SEEK_CUR);
            fwrite(&temp, sizeof(account_data), 1, file);

            return true;
        }
    }
    fclose(file);
    return false;
}

void add_balance_menu(unsigned short *target_acc, float *balance_change){
    do
    {
        printf("To which account do you want to add balance: ");
        scanf("%hu", target_acc);
    } while (*target_acc < 1000 || *target_acc > 9999);   

    do
    {
        printf("How much money do you want to input: ");
        scanf("%f", balance_change);
    } while (*balance_change < 0.01);
}

void remove_balance_menu(unsigned short *target_acc, float *balance_change){
    do
    {
        printf("From which account do you want to withdraw balance: ");
        scanf("%hu", target_acc);
    } while (*target_acc < 1000 || *target_acc > 9999);

    do
    {
        printf("How much money do you want to withdraw: ");
        scanf("%f", balance_change);
    } while (*balance_change < 0.01);
}

bool remove_balance(int target_acc, float balance_change){

    FILE *file = fopen(ACCOUNTS_FILE_NAME, "r+b");
    bool was_successful = false;
    if (file == NULL) return was_successful;

    account_data temp;

    while (fread(&temp, sizeof(account_data), 1, file)){
        if (temp.account_number == target_acc){
            if (temp.current_balance >= balance_change){
                temp.current_balance -= balance_change;

                fseek(file, -sizeof(account_data), SEEK_CUR);
                fwrite(&temp, sizeof(account_data), 1, file);
                printf("Balanced changed. New balance: %f", temp.current_balance);
                was_successful = true;
            }
            else{
                printf("Error: Wrong operation!");
            }
            break;
        }
    }
    fclose(file);
    return was_successful;
}

void transfer_money(int source_acc, int target_acc, float amount){
    
    bool can_transfer = remove_balance(source_acc, amount);

    if (can_transfer){
        add_balance(target_acc, amount);
        printf("Transfer realised!\n");
    }
    else{
        printf("Transfer has not been realised!\n");
    }

    return;
}

void transfer_money_menu(unsigned short *source_acc, unsigned short *target_acc, float *balance_change){
    do
    {
        printf("From which account do you want to transfer balance: ");
        scanf("%hu", source_acc);
    } while (*source_acc < 1000 || *source_acc > 9999);
    do
    {
        printf("To which account do you want to transfer balance: ");
        scanf("%hu", target_acc);
    } while (*target_acc < 1000 || *target_acc > 9999 || *target_acc == *source_acc);        

    do
    {
        printf("How much money do you want to input: ");
        scanf("%f", balance_change);
    } while (*balance_change < 0.01);
}