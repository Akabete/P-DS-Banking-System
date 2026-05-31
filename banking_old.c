#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <stdbool.h>

#define ACC_NO_MIN 1000
#define ACC_NO_MAX 9999
#define ACC_NO_LENGTH 5
#define NAME_MAX_LENGTH 51
#define SURNAME_MAX_LENGTH 101
#define MAX_ADDRESS_LENGTH 201
#define PESEL_LENGTH 12
#define LICENCE_PLATE_LENGTH 10
#define ACCOUNTS_FILE_NAME "account_data.dat"
#define INSURANCES_FILE_NAME "insurances.dat"

typedef struct {
    int account_id;
    char licence_plate[LICENCE_PLATE_LENGTH];
    float cost;
} insurance_data;

typedef struct {
    unsigned int account_number;
    char name[NAME_MAX_LENGTH];
    char surname[SURNAME_MAX_LENGTH];
    char address[MAX_ADDRESS_LENGTH];
    char pesel[PESEL_LENGTH];
    float current_balance;

} account_data;


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

void input_name(account_data* new_account){
    bool is_name_correct = false;
    do {
    printf("Enter your name: ");
    fgets(new_account->name, NAME_MAX_LENGTH, stdin);
    new_account->name[strcspn(new_account->name, "\n")] = '\0';

    if (validate_name(new_account->name)) is_name_correct = true;
    else printf("Invalid name.\n");

    } while(!is_name_correct);
}

void input_surname(account_data* new_account){
    bool is_surname_correct = false;
    do {
    printf("Enter your surname: ");
    fgets(new_account->surname, SURNAME_MAX_LENGTH, stdin);
    new_account->surname[strcspn(new_account->surname, "\n")] = '\0';

    if (validate_name(new_account->surname)) is_surname_correct = true;
    else printf("Invalid surname.\n");

    } while(!is_surname_correct);
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
    account_data new_account;
    new_account.account_number = generate_account_number();
    input_name(&new_account);
    input_surname(&new_account);
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
    FILE *file = fopen(ACCOUNTS_FILE_NAME, "rb");
    if (file == NULL){
        printf("Database is missing \n");
        return;
    }

    account_data temp;
    printf("---Accounts---\n");

    while (fread(&temp, sizeof(account_data), 1, file))
        printf("Account No.: %d\n Name: %s\n Surname: %s\n Address: %s\n, PESEL: %s\n Balance: %f",
            temp.account_number, temp.name, temp.surname, temp.address, temp.pesel, temp.current_balance);

    fclose(file);
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

void print_accounts(const char *search){
    FILE *file = fopen(ACCOUNTS_FILE_NAME, "rb");
    if (file == NULL) return;
    
    char acc_no[ACC_NO_LENGTH];
    account_data temp;
    bool found = false;
    
    
    while (fread(&temp, sizeof(account_data), 1, file)){
        sprintf(acc_no, "%u", temp.account_number);
        if (strstr(temp.name, search) != NULL ||
            strstr(temp.surname, search) != NULL ||
            strstr(temp.address, search) != NULL ||
            strstr(temp.pesel, search) != NULL ||
            strstr(acc_no, search) != NULL
            ){
                printf("Account No.: %s\n Name: %s\n Surame: %s\n Address: %s\n PESEL: %s\n ", acc_no, temp.name, temp.surname, temp.address, temp.pesel);
                found = true;
            }
    }
    if (!found) printf("No accounts found.\n");
    fclose(file);
}

bool confirm_operation(){
    char decision;
    printf("Are you sure you want to continue this operation (Y/N): ");
    while (getchar() != '\n');

    scanf("%c", &decision);

    if (decision == 'y' || decision == 'Y') return true;
    else return false;
}

void add_insurances(unsigned int acc_no){
    insurance_data temp; 

    temp.account_id = acc_no;

    printf("Enter your licence plate: ");
    fgets(temp.licence_plate, LICENCE_PLATE_LENGTH, stdin);
    temp.licence_plate[strcspn(temp.licence_plate, "\n")] = '\0';

    float insurance_amount = 0;
    printf("\n Enter the amount you want to insure: ");
    scanf("%f", &insurance_amount);
    temp.cost = insurance_amount;

    FILE *file = fopen(INSURANCES_FILE_NAME, "ab");
    if (file != NULL){
        fwrite(&temp, sizeof(insurance_data), 1, file);
        fclose(file);
        printf("\nInsurance added.\n");
    }
    
}

int main(void){


    unsigned short choice = 0;
    do
    {
        printf("Banking System\n\n");
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
        scanf("%hu", &choice);

        switch (choice)
        {
        case 1:{
            while (getchar() !='\n');
            create_account();
        }break;
        case 2:{
            while (getchar() !='\n');
            char search_query[MAX_ADDRESS_LENGTH];
            printf("Enter search query (Name, Surname, Address, PESEL or Account Number): ");
            fgets(search_query, sizeof(search_query), stdin);
            search_query[strcspn(search_query, '\n')] = '\0';

            print_accounts(search_query);
        }break;
        case 3:{
            if (confirm_operation()){
                unsigned short target_acc = 0;
                do
                {
                    printf("To which account do you want to add balance: ");
                    scanf("%hu", &target_acc);
                } while (target_acc < 1000 || target_acc > 9999);   
        
                float balance_change = 0.0;
                do
                {
                    printf("How much money do you want to input: ");
                    scanf("%f", &balance_change);
                } while (balance_change < 0.01);
                add_balance(target_acc, balance_change);
        }
        }break;
        case 4:{
            if (confirm_operation()){
                unsigned short target_acc = 0;
                do
                {
                    printf("From which account do you want to withdraw balance: ");
                    scanf("%hu", &target_acc);
                } while (target_acc < 1000 || target_acc > 9999);
        
                float balance_change = 0.0;
                do
                {
                    printf("How much money do you want to withdraw: ");
                    scanf("%f", &balance_change);
                } while (balance_change < 0.01);
                remove_balance(target_acc, balance_change);
        }
        }break;
        case 5:{
            if (confirm_operation()){
                unsigned short source_acc = 0;
                do
                {
                    printf("From which account do you want to transfer balance: ");
                    scanf("%hu", &source_acc);
                } while (source_acc < 1000 || source_acc > 9999);
                unsigned short target_acc = 0;
                do
                {
                    printf("To which account do you want to transfer balance: ");
                    scanf("%hu", &target_acc);
                } while (target_acc < 1000 || target_acc > 9999 || target_acc == source_acc);        
        
                float balance_change = 0.0;
                do
                {
                    printf("How much money do you want to input: ");
                    scanf("%f", &balance_change);
                } while (balance_change < 0.01);
                transfer_money(source_acc, target_acc, balance_change);
            }
        }break;
        case 6:{
            unsigned short target_acc = 0;
            printf("For whuch account do you want to buy insurance: ");
            scanf("%hu", &target_acc);
            while (getchar() != '\n');
            add_insurances(target_acc);
        }break;
        }

    } while (choice < 1 || choice > 6);

    

    return 0;
}