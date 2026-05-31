#include "definitions.h"
#include "insurances.h"
#include <stdlib.h>
#include <stdio.h>

void insurance_manager(){
    unsigned short target_acc = 0;
    printf("For whuch account do you want to buy insurance: ");
    scanf("%hu", &target_acc);
    while (getchar() != '\n');
    add_insurances(target_acc);
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

void print_insurances(unsigned short target_acc_no){
    FILE *file = fopen(INSURANCES_FILE_NAME, "rb");
    if (file == NULL) return;

    insurance_data temp;
    while (fread(&temp, sizeof(insurance_data), 1, file)){
        if (temp.account_id == target_acc_no)
            printf("\t - Licence Plate: %s\n \t - Insured Amount: %.2f\n", temp.licence_plate, temp.cost);
    }
    fclose(file);
}