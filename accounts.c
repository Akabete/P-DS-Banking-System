#include "definitions.h"
#include "insurances.h"
#include "accounts.h"
#include <stdio.h>
#include <stdbool.h>

bool is_acc_no_free(int potential_acc_no){
    FILE *file = fopen(ACCOUNTS_FILE_NAME, "rb");
    if (file != NULL){
        account_data temp;
        while (fread(&temp, sizeof(account_data), 1, file))
            if (temp.account_number == potential_acc_no) return false;
    }
    fclose(file);

    return true;
}

int generate_account_number(){
    srand(time(NULL));
    int potential_acc_no = 0;
    do{
        potential_acc_no = rand() % (ACC_NO_MAX - ACC_NO_MIN + 1) + ACC_NO_MIN;
    }while (!is_acc_no_free(potential_acc_no));

    return potential_acc_no;
}

bool validate_name(const char *potential_name){
    if (potential_name[0] == '\0') return false;

    for (int i = 0; potential_name[i] != '\0'; i++)
        if (!isalpha(potential_name[i]) && potential_name[i] != '-') return false;

    return true;
}

void input_name(char *target_buffor, const char *text, int max_length){
    bool is_correct = false;
    do {
    printf("%s: ", text);
    fgets(target_buffor, max_length, stdin);
    target_buffor[strcspn(target_buffor, "\n")] = '\0';

    if (validate_name(target_buffor)) is_correct = true;
    else printf("Invalid name.\n");

    } while(!is_correct);
}

void input_address(account_data* new_account){
    printf("Enter your address: ");
    fgets(new_account->address, MAX_ADDRESS_LENGTH, stdin);
    new_account->address[strcspn(new_account->address, "\n")] = '\0';
}

void input_pesel(account_data* new_account){
    do{
    printf("Input your PESEL number: ");
    fgets(new_account->pesel, PESEL_LENGTH, stdin);
    new_account->pesel[strcspn(new_account->pesel, "\n")] = '\0';
    } while (!validate_pesel(new_account->pesel));
}

bool validate_pesel(const char *potential_pesel){
    if (strlen(potential_pesel) != PESEL_LENGTH - 1) return false;
    for (int i = 0; i < PESEL_LENGTH - 1; i++)
        if (!isdigit(potential_pesel[i])) return false;

    int importance[] = {1, 3 ,7, 9, 1, 3, 7, 9, 1, 3};
    unsigned sum = 0;
    for (int i = 0; i < PESEL_LENGTH - 2; i++){
        sum += (potential_pesel[i] - '0') * importance[i];
    }
    int result = 10 -(sum % 10);
    if (result == 10) result = 0;
    
    if (result == (potential_pesel[PESEL_LENGTH - 2] - '0'))
        return true;
    
    return false;
}

void create_account() {
    while (getchar() !='\n');
    account_data new_account;
    new_account.account_number = generate_account_number();
    input_name(new_account.name, "Enter your name", NAME_MAX_LENGTH);
    input_name(new_account.surname, "Enter your name", SURNAME_MAX_LENGTH);
    input_address(&new_account);
    input_pesel(&new_account);
    new_account.current_balance = 0.0;


    FILE *file = fopen(ACCOUNTS_FILE_NAME, "ab");
    if (file != NULL){
        fwrite(&new_account, sizeof(account_data), 1, file);
        fclose(file);
    }
}

void print_accounts(){
    while (getchar() !='\n');
    FILE *file = fopen(ACCOUNTS_FILE_NAME, "rb");
    if (file == NULL) return;

    char search[MAX_ADDRESS_LENGTH];
    printf("Enter search query (Name, Surname, Address, PESEL or Account Number): ");
    fgets(search, sizeof(search), stdin);
    search[strcspn(search, "\n")] = '\0';
    
    char acc_no[ACC_NO_LENGTH];
    account_data temp;
    bool found = false;
    
    
    while (fread(&temp, sizeof(account_data), 1, file)){
        sprintf(acc_no, "%u", temp.account_number);
        if (strstr(temp.name, search) != NULL ||
            strstr(temp.surname, search) != NULL ||
            strstr(temp.address, search) != NULL ||
            strstr(temp.pesel, search) != NULL ||
            strstr(acc_no, search) != NULL ||
            strlen(search) == 0){
                printf("Account No.: %s\n Name: %s\n Surame: %s\n Address: %s\n PESEL: %s\n\n", acc_no, temp.name, temp.surname, temp.address, temp.pesel);
                print_insurances(temp.account_number);
                found = true;
            }
    }
    if (!found) printf("No accounts found.\n");
    fclose(file);
}
