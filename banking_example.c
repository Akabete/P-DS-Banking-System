#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MAX_NAME_LEN 50
#define MAX_ADDRESS_LEN 100
#define MAX_PESEL_LEN 12
#define FILENAME "bank_accounts.dat"

typedef struct {
    int account_number;
    char name[MAX_NAME_LEN];
    char surname[MAX_NAME_LEN];
    char address[MAX_ADDRESS_LEN];
    char pesel[MAX_PESEL_LEN];
    double regular_balance;
    double loan_balance;
    double loan_interest;
} BankAccount;

typedef struct {
	int car_register_number;
	double insurance_price;
} CarInsurance;

int clearInputBuffer() {
    int c;
    int didclear = 0;
    while ((c = getchar()) != '\n' && c != EOF)
		didclear = 1;
	return didclear;
}

void printChoices() {
	printf("\nSearch by:\n");
    printf("1. Account number\n");
    printf("2. Name\n");
    printf("3. Surname\n");
    printf("4. Address\n");
    printf("5. PESEL\n");
    printf("Enter your choice: ");
}

int checkNameValidity(const char *name) {
    if (name[0] == '\0') {
        return 0;
    }

    for (int i = 0; name[i] != '\0'; i++) {
        if (!isalpha(name[i]) && name[i] != '-') {
            return 0;
        }
    }
    return 1;
}

int generateAccountNumber() {
    int initialized = 0;
    int next_account_number = 0;
    
    if (!initialized) {
        srand(time(NULL));
        next_account_number = 1000 + rand() % 9000;
        initialized = 1;
    } else {
        next_account_number++;
    }
    return next_account_number;
}

void enterName(BankAccount* new_account) {
	printf("Enter name: ");
    fgets(new_account->name, MAX_NAME_LEN, stdin);
    new_account->name[strcspn(new_account->name, "\n")] = '\0';
    while (!checkNameValidity(new_account->name)) {
        printf("Invalid name.\n");
        printf("Enter name: ");
        fgets(new_account->name, MAX_NAME_LEN, stdin);
        new_account->name[strcspn(new_account->name, "\n")] = '\0';
    }
}

void enterSurname(BankAccount* new_account) {
	printf("Enter surname: ");
    fgets(new_account->surname, MAX_NAME_LEN, stdin);
    new_account->surname[strcspn(new_account->surname, "\n")] = '\0';
    while (!checkNameValidity(new_account->surname)) {
        printf("Invalid surname.\n");
        printf("Enter surname: ");
        fgets(new_account->surname, MAX_NAME_LEN, stdin);
        new_account->surname[strcspn(new_account->surname, "\n")] = '\0';
    }
}

void enterAdress(BankAccount* new_account) {
    printf("Enter address: ");
    fgets(new_account->address, MAX_ADDRESS_LEN, stdin);
    new_account->address[strcspn(new_account->address, "\n")] = '\0';	
}

void enterPesel(BankAccount* new_account) {
	printf("Enter PESEL (11 digits): ");
    fgets(new_account->pesel, MAX_PESEL_LEN, stdin);
    new_account->pesel[strcspn(new_account->pesel, "\n")] = '\0';
    while (strlen(new_account->pesel) != 11 || clearInputBuffer()) {
        printf("Error: PESEL must be exactly 11 digits.\n");
        printf("Enter PESEL (11 digits): ");
        fgets(new_account->pesel, MAX_PESEL_LEN, stdin);
        new_account->pesel[strcspn(new_account->pesel, "\n")] = '\0';
    }
    for (int i = 0; i < 11; i++) { 
        while (!isdigit(new_account->pesel[i])) {
            printf("Error: PESEL must contain only digits.\n");
            printf("Enter PESEL (11 digits): ");
            fgets(new_account->pesel, MAX_PESEL_LEN, stdin);
            new_account->pesel[strcspn(new_account->pesel, "\n")] = '\0';
            i = 0;
            if (strlen(new_account->pesel) != 11) break;
        }
    }
}

void createAccount() {
    FILE *file = fopen(FILENAME, "ab+");
    if (!file) {
        perror("Failed to open accounts file");
        return;
    }
	BankAccount new_account = {0};
    new_account.account_number = generateAccountNumber();
    new_account.regular_balance = 0.0;
    new_account.loan_balance = 0.0;
    new_account.loan_interest = 0.0;
	printf("\nCreating new account (Account #%d)\n", new_account.account_number);
    clearInputBuffer();
	enterName(&new_account);
	enterSurname(&new_account);
	enterAdress(&new_account);
	enterPesel(&new_account);

    if (fwrite(&new_account, sizeof(BankAccount), 1, file) != 1) {
        perror("Failed to write new account");
    } else {
        printf("Account created successfully.\n");
    }
	fclose(file);
}

void printListBuffer() {
	printf("\nList of all accounts:\n");
    printf("------------------------------------------------------------------------------------------------------------------\n");
    printf("| Acc #  | Name            | Surname         | Address                    | PESEL        | Balance    | Loan      |\n");
    printf("------------------------------------------------------------------------------------------------------------------\n");
}

void listAccounts() {
    FILE *file = fopen(FILENAME, "rb");
    if (!file) {
        printf("No accounts found.\n");
        return;
    }
	BankAccount account;
    int count = 0;
    printListBuffer();
    
    while (fread(&account, sizeof(BankAccount), 1, file) == 1) {
        if (account.account_number == 0) { // Skip empty accounts
            continue;
        }
        printf("| %-6d | %-15s | %-15s | %-27s | %-11s | %-10.2f | %-9.2f |\n",
               account.account_number,
               account.name,
               account.surname,
               account.address,
               account.pesel,
               account.regular_balance,
               account.loan_balance);
        count++;
    }
    if (ferror(file)) {perror("Error reading accounts file");}
    
    printf("------------------------------------------------------------------------------------------------------------------\n");
    printf("Total accounts: %d\n", count);
    fclose(file);
}

void showFoundAcc(BankAccount account) {
	printf("| %-6d | %-15s | %-15s | %-27s | %-11s | %-10.2f | %-9.2f |\n",
                   account.account_number,
                   account.name,
                   account.surname,
                   account.address,
                   account.pesel,
                   account.regular_balance,
                   account.loan_balance);
}

void printFoundAcc(int choice, char* search_term) {
	FILE *file = fopen(FILENAME, "rb");
    if (!file) {
        printf("No accounts found.\n");
        return;
    }
	BankAccount account;
    int found = 0;
    while (fread(&account, sizeof(BankAccount), 1, file) == 1) {
        int match = 0;
        switch (choice) {
            case 1: {
                int search_num = atoi(search_term); //from string to int value
                match = (account.account_number == search_num);
                break; }
            case 2:
                match = (strstr(account.name, search_term) != NULL);
                break;
            case 3:
                match = (strstr(account.surname, search_term) != NULL);
                break;
            case 4:
                match = (strstr(account.address, search_term) != NULL);
                break;
            case 5:
                match = (strstr(account.pesel, search_term) != NULL);
                break;
        }
        if (match) {
            showFoundAcc(account);
            found++;
        }
    }
    printf("Found %d account(s).\n", found);
    fclose(file);
}

void searchResultPrint() {
	printf("\nSearch results:\n");
    printf("------------------------------------------------------------------------------------------------------------------\n");
    printf("| Acc #  | Name            | Surname         | Address                    | PESEL        | Balance    | Loan      |\n");
    printf("------------------------------------------------------------------------------------------------------------------\n");
}

void searchAccount() {
    FILE *file = fopen(FILENAME, "rb");
    if (!file) {
        printf("No accounts found.\n");
        return;
    }
    int choice;
    printChoices();
    
    if (scanf("%d", &choice) != 1 || choice < 1 || choice > 5) {
        printf("Invalid choice.\n");
        clearInputBuffer();
        fclose(file);
        return;
    }
    clearInputBuffer();
    
    char search_term[MAX_ADDRESS_LEN];
    printf("Enter search term: ");
    fgets(search_term, sizeof(search_term), stdin);
    search_term[strcspn(search_term, "\n")] = '\0'; 
    
    searchResultPrint()
    printFoundAcc(choice, search_term);
}

int confirmOperation(const char *message) {
    char response;
    printf("%s (y/n): ", message);
    scanf(" %c", &response);
    return (response == 'y' || response == 'Y');
}

void addMoney(int account_number, double amount) {
	FILE *file = fopen(FILENAME, "rb+");
    if (!file) {
        printf("No accounts found.\n");
        return;
    }
	
	BankAccount account;
    int found = 0;
    long pos = 0;
    
    while (fread(&account, sizeof(BankAccount), 1, file) == 1) {
        if (account.account_number == account_number) {
            found = 1;
            account.regular_balance += amount;
            fseek(file, pos, SEEK_SET);
            fwrite(&account, sizeof(BankAccount), 1, file); 
            printf("Deposit successful. New balance: %.2f\n", account.regular_balance);
            break;
        }
        pos = ftell(file); //Save position before next read
    }
    
    if (!found) {
        printf("Account not found.\n");
    }
    
    fclose(file);
	}

void deposit() {
    int account_number;
    printf("Enter account number: ");
    if (scanf("%d", &account_number) != 1) {
        printf("Invalid account number.\n");
        clearInputBuffer();
        return;
    }
    
    double amount;
    printf("Enter amount to deposit: ");
    if (scanf("%lf", &amount) != 1 || amount <= 0) {
        printf("Invalid amount.\n");
        clearInputBuffer();
        return;
    }
    
    if (!confirmOperation("Are you sure you want to make this deposit?")) {
        printf("Deposit canceled.\n");
        return;
    }
    
    addMoney(account_number, amount);
}

void takeMoney(int account_number, double amount) {
	FILE *file = fopen(FILENAME, "rb+");
    if (!file) {
        printf("No accounts found.\n");
        return;
    }
    
    BankAccount account;
    int found = 0;
    long pos = 0;
    
    while (fread(&account, sizeof(BankAccount), 1, file) == 1) {
        if (account.account_number == account_number) {
            found = 1;
            if (amount > account.regular_balance) {
                printf("Insufficient funds.\n");
                break;
            }
            account.regular_balance -= amount;
            fseek(file, pos, SEEK_SET);
            fwrite(&account, sizeof(BankAccount), 1, file);
            printf("Withdrawal successful. New balance: %.2f\n", account.regular_balance);
            break;
        }
        pos = ftell(file);
    }
    if (!found) {
        printf("Account not found.\n");
    }
	fclose(file);
}

void withdraw() {
    int account_number;
    printf("Enter account number: ");
    if (scanf("%d", &account_number) != 1) {
        printf("Invalid account number.\n");
        clearInputBuffer();
        return;
    }
    
    double amount;
    printf("Enter amount to withdraw: ");
    if (scanf("%lf", &amount) != 1 || amount <= 0) {
        printf("Invalid amount.\n");
        clearInputBuffer();
        return;
    }
    
    if (!confirmOperation("Are you sure you want to make this withdrawal?")) {
        printf("Withdrawal canceled.\n");
        return;
    }
    takeMoney(account_number, amount);
}

void exchangeMoney(int from_account, int to_account, double amount) {
	FILE *file = fopen(FILENAME, "rb+");
    if (!file) {
        printf("No accounts found.\n");
        return;
    }
    BankAccount from_acc, to_acc;
    int from_found = 0, to_found = 0;
    long from_pos = -1, to_pos = -1;
    
    while (fread(&from_acc, sizeof(BankAccount), 1, file) == 1) {
        if (from_acc.account_number == from_account) {
            from_found = 1;
            from_pos = ftell(file) - sizeof(BankAccount);
        }
        if (from_acc.account_number == to_account) {
            to_found = 1;
            to_pos = ftell(file) - sizeof(BankAccount);
        }
        if (from_found && to_found) break;
    }
    if (!from_found || !to_found) {
        printf("%s account not found.\n", !from_found ? "Source" : "Destination");
        fclose(file);
        return;
    }
    fseek(file, from_pos, SEEK_SET);
    fread(&from_acc, sizeof(BankAccount), 1, file);
    if (amount > from_acc.regular_balance) {
        printf("Insufficient funds in source account.\n");
        fclose(file);
        return;
    }
    from_acc.regular_balance -= amount;
    fseek(file, from_pos, SEEK_SET);
    fwrite(&from_acc, sizeof(BankAccount), 1, file);
    
    fseek(file, to_pos, SEEK_SET);
    fread(&to_acc, sizeof(BankAccount), 1, file);
    to_acc.regular_balance += amount;
    fseek(file, to_pos, SEEK_SET);
    fwrite(&to_acc, sizeof(BankAccount), 1, file);
    fclose(file);
}

void transfer() {
    int from_account, to_account;
    printf("Enter source account number: ");
    if (scanf("%d", &from_account) != 1) {
        printf("Invalid account number.\n");
        clearInputBuffer();
        return;
    }
    printf("Enter destination account number: ");
    if (scanf("%d", &to_account) != 1) {
        printf("Invalid account number.\n");
        clearInputBuffer();
        return;
    }
    if (from_account == to_account) {
        printf("Cannot transfer to the same account.\n");
        return;
    }
    double amount;
    printf("Enter amount to transfer: ");
    if (scanf("%lf", &amount) != 1 || amount <= 0) {
        printf("Invalid amount.\n");
        clearInputBuffer();
        return;
    }
    printf("\nTransfer Details:\n");
    printf("Amount: %.2f\n", amount);
    
    if (!confirmOperation("Are you sure you want to make this transfer?")) {
        printf("Transfer canceled.\n");
        return;
    }
	exchangeMoney(from_account, to_account, amount);
    
    printf("Transfer successful.\n");
}

void finishLoan(int account_number, double amount, double interest_rate) {
	FILE *file = fopen(FILENAME, "rb+");
    if (!file) {
        printf("No accounts found.\n");
        return;
    }
    
    BankAccount account;
    int found = 0;
    long pos = 0;
    
    while (fread(&account, sizeof(BankAccount), 1, file) == 1) {
        if (account.account_number == account_number) {
            found = 1;
            
            if (account.loan_balance > 0) {
                printf("This account already has a loan.\n");
                break;
            }
            
            account.loan_balance = amount * (1 + interest_rate/100);
            account.loan_interest = interest_rate;
            account.regular_balance += amount;
            
            fseek(file, pos, SEEK_SET);
            fwrite(&account, sizeof(BankAccount), 1, file);
            
            printf("Loan granted. Regular account balance: %.2f\n", account.regular_balance);
            printf("Total loan debt: %.2f\n", account.loan_balance);
            break;
        }
        pos = ftell(file);
    }
    if (!found) {
        printf("Account not found.\n");
    }
    fclose(file);
}

void takeLoan() {
    int account_number;
    printf("Enter account number: ");
    if (scanf("%d", &account_number) != 1) {
        printf("Invalid account number.\n");
        clearInputBuffer();
        return;
    }
    double amount;
    printf("Enter loan amount: ");
    if (scanf("%lf", &amount) != 1 || amount <= 0) {
        printf("Invalid amount.\n");
        clearInputBuffer();
        return;
    }
    double interest_rate;
    printf("Enter interest rate (e.g., 5 for 5%%): ");
    if (scanf("%lf", &interest_rate) != 1 || interest_rate <= 0) {
        printf("Invalid interest rate.\n");
        clearInputBuffer();
        return;
    }
    printf("\nLoan Details:\n");
    printf("Loan amount: %.2f\n", amount);
    printf("Interest rate: %.2f%%\n", interest_rate);
    printf("Total to repay: %.2f\n", amount * (1 + interest_rate/100));
    
    if (!confirmOperation("Are you sure you want to take this loan?")) {
        printf("Loan canceled.\n");
        return;
    }
    finishLoan(account_number, amount, interest_rate);
}

int enterAccountNum(){
	int account_number;
	printf("Enter account number: ");
    if (scanf("%d", &account_number) != 1) {
        printf("Invalid account number.\n");
        clearInputBuffer();
        return 0;
    }
    else return account_number;
}

double enterAmountToPay(){
	double amount;
    printf("Enter amount to pay: ");
    if (scanf("%lf", &amount) != 1 || amount <= 0) {
        printf("Invalid amount.\n");
        clearInputBuffer();
        return 0;
    }
    else return amount;
}

void printPaymentDetails(BankAccount* account, double** amount) {
	printf("\nPayment Details:\n");
    printf("Current debt: %.2f\n", account->loan_balance);
    printf("Payment amount: %.2f\n", **amount);
    printf("Remaining debt after payment: %.2f\n", account->loan_balance - **amount);
}

void confirmLoanPayment(int* found, int account_number, double* amount, FILE* file) {
	long pos = 0;
	BankAccount account;
	    while (fread(&account, sizeof(BankAccount), 1, file) == 1) {
        if (account.account_number == account_number) {
            *found = 1;
            if (account.loan_balance <= 0) {
                printf("This account has no loan.\n");
                break;
            }
            if (*amount > account.regular_balance) {
                printf("Insufficient funds in regular account.\n");
                break;
            }
            if (*amount > account.loan_balance) {
                printf("Amount exceeds current debt. Maximum payment: %.2f\n", account.loan_balance);
                break;
            }
            printPaymentDetails(&account, &amount);
            if (!confirmOperation("Are you sure you want to make this payment?")) {
                printf("Payment canceled.\n");
                break;
            }
            account.regular_balance -= *amount;
            account.loan_balance -= *amount;
            if (account.loan_balance < 0.01) {
                account.loan_balance = 0.0;
                account.loan_interest = 0.0;
            }
            fseek(file, pos, SEEK_SET);
            fwrite(&account, sizeof(BankAccount), 1, file);
            
            printf("Payment successful.\n");
            printf("Regular account balance: %.2f\n", account.regular_balance);
            printf("Remaining debt: %.2f\n", account.loan_balance);
            break;
        }
        pos = ftell(file);
    }
}

void payDebt() {
    int account_number = enterAccountNum();
    double amount = enterAmountToPay();
    
    FILE *file = fopen(FILENAME, "rb+");
    if (!file) {
        printf("No accounts found.\n");
        return;
    }
    int found = 0;
    
    confirmLoanPayment(&found, account_number, &amount, file);
    if (!found) {
        printf("Account not found.\n");
    }
    fclose(file);
}

void printAccDetails(BankAccount* account){
	printf("\nAccount to be deleted:\n");
    printf("Account #: %d\n", account->account_number);
    printf("Name: %s %s\n", account->name, account->surname);
    printf("Balance: %.2f\n", account->regular_balance);
    printf("Loan balance: %.2f\n", account->loan_balance);
}

int findAccountByNumber(int account_number, BankAccount *found_account) {
    FILE *file = fopen(FILENAME, "rb");
    if (!file) {
        return 0;
    }
	BankAccount account;
    int found = 0;
    
    while (fread(&account, sizeof(BankAccount), 1, file) == 1) {
        if (account.account_number == account_number) {
            *found_account = account;  // Copy the found account data
            found = 1;
            break;
        }
    }
    fclose(file);
    return found;
}

int copyAccountsExcluding(int account_number, const char *temp_filename) {
    FILE *source = fopen(FILENAME, "rb");
    FILE *dest = fopen(temp_filename, "wb");
    
    if (!source || !dest) {
        if (source) fclose(source);
        if (dest) fclose(dest);
        return -1;  // Error
    }
	BankAccount account;
    int accounts_written = 0;
    
    while (fread(&account, sizeof(BankAccount), 1, source) == 1) {
        if (account.account_number != account_number) {
            fwrite(&account, sizeof(BankAccount), 1, dest);
            accounts_written++;
        }
    }
    fclose(source);
    fclose(dest);
    return accounts_written;
}

void deleteAccount() {
    int account_number = enterAccountNum();
    BankAccount account_to_delete;
    
    // Find the account
    if (!findAccountByNumber(account_number, &account_to_delete)) {
        printf("Account not found.\n");
        return;
    }
    // Show account details and confirm
    printAccDetails(&account_to_delete);
    if (!confirmOperation("Are you sure you want to delete this account?")) {
        printf("Account deletion canceled.\n");
        return;
    }
    // Create temp file without deleted account
    const char *temp_filename = "temp.dat";
    int remaining_accounts = copyAccountsExcluding(account_number, temp_filename);
    
    if (remaining_accounts < 0) {
        printf("Error during account deletion.\n");
        return;
    }
   
    remove(FILENAME);
    rename(temp_filename, FILENAME);
    
    printf("Account deleted successfully. Total accounts remaining: %d\n", remaining_accounts);
}

void printChoicesMain() {
        printf("\nBank Account Management System\n");
        printf("1. Create account\n");
        printf("2. List all accounts\n");
        printf("3. Search for an account\n");
        printf("4. Make a deposit\n");
        printf("5. Make a withdrawal\n");
        printf("6. Make a transfer\n");
        printf("7. Take a loan\n");
        printf("8. Pay debt\n");
        printf("9. Delete an account\n");
        printf("0. Exit\n");
        printf("Enter your choice: ");	
}

int main() {
    int choice;
    
    do {
        printChoicesMain();
        
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Please enter a number.\n");
            clearInputBuffer();
            continue;
        }
        
        switch (choice) {
            case 1: createAccount(); break;
            case 2: listAccounts(); break;
            case 3: searchAccount(); break;
            case 4: deposit(); break;
            case 5: withdraw(); break;
            case 6: transfer(); break;
            case 7: takeLoan(); break;
            case 8: payDebt(); break;
            case 9: deleteAccount(); break;
            case 0: 
                printf("Exiting...\n"); 
                clearInputBuffer();
                break;
            default: printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 0);
    return 0;
}
