#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <stdbool.h>
#include "definitions.h"
#include "accounts.h"
#include "transactions.h"
#include "insurances.h"

void display_menu(unsigned short *choice){
    printf("\n----------Banking System--------------\n\n");
    printf("----------------------------------------\n");
    printf("1. Create an account\n");
    printf("----------------------------------------\n");
    printf("2. Search for an account\n");
    printf("----------------------------------------\n");
    printf("3. Add balance\n");
    printf("----------------------------------------\n");
    printf("4. Withdraw money\n");
    printf("----------------------------------------\n");
    printf("5. Transfer money\n");
    printf("----------------------------------------\n");
    printf("6. Buy car insurance\n");
    printf("----------------------------------------\n");
    printf("Input your choice: ");
    
    scanf("%hu", choice);
}

int main(void){
    unsigned short choice = 0;
    do
    {
        display_menu(&choice);

        switch (choice)
        {
        case 0:{
            printf("System is shutting down.");
        }break;
        case 1:{
            create_account();
        }break;
        case 2:{
            print_accounts();
        }break;
        case 3:{
            add_balance_manager();
        }break;
        case 4:{
            remove_balance_manager();
        }break;
        case 5:{
            transfer_manager();
        }break;
        case 6:{
            insurance_manager();
        }break;
        default:{
            printf("Invalid action.");
        }break;

        }

    } while (choice != 0);

    

    return 0;
}