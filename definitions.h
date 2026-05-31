#ifndef DEFINITIONS_H
#define DEFINITIONS_H

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

#endif