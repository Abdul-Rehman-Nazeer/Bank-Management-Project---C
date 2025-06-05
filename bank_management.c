#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_ACCOUNTS 100
#define MAX_NAME_LENGTH 50
#define MINIMUM_BALANCE 500
#define OVERDRAFT_LIMIT 1000
#define FILENAME "bank_data.dat"

typedef struct {
    int accountNumber;
    char name[MAX_NAME_LENGTH];
    double balance;
    char accountType; // 'S' for Savings, 'C' for Checking
} Account;

typedef struct {
    Account accounts[MAX_ACCOUNTS];
    int accountCount;
    int nextAccountNumber;
} Bank;

// Function prototypes
void initializeBank(Bank* bank);
void loadAccountsFromFile(Bank* bank);
void saveAccountsToFile(const Bank* bank);
int createAccount(Bank* bank, const char* name, double initialDeposit, char accountType);
bool deleteAccount(Bank* bank, int accountNumber);
Account* findAccount(Bank* bank, int accountNumber);
bool deposit(Bank* bank, int accountNumber, double amount);
bool withdraw(Bank* bank, int accountNumber, double amount);
void displayAccount(const Account* account);
void displayAllAccounts(const Bank* bank);
void addInterestToSavings(Bank* bank);
void displayMenu();
void handleCreateAccount(Bank* bank);
void handleDeposit(Bank* bank);
void handleWithdraw(Bank* bank);
void handleDeleteAccount(Bank* bank);
void handleViewAccount(Bank* bank);
bool saveBankData(const Bank* bank);
bool loadBankData(Bank* bank);

void initializeBank(Bank* bank) {
    bank->accountCount = 0;
    bank->nextAccountNumber = 1001;
    loadAccountsFromFile(bank);
}

void loadAccountsFromFile(Bank* bank) {
    FILE* file = fopen(FILENAME, "rb");
    if (file != NULL) {
        fread(bank, sizeof(Bank), 1, file);
        fclose(file);
    }
}

void saveAccountsToFile(const Bank* bank) {
    FILE* file = fopen(FILENAME, "wb");
    if (file != NULL) {
        fwrite(bank, sizeof(Bank), 1, file);
        fclose(file);
    }
}

int createAccount(Bank* bank, const char* name, double initialDeposit, char accountType) {
    if (bank->accountCount >= MAX_ACCOUNTS) {
        printf("Maximum number of accounts reached.\n");
        return -1;
    }

    Account* newAccount = &bank->accounts[bank->accountCount];
    newAccount->accountNumber = bank->nextAccountNumber++;
    strncpy(newAccount->name, name, MAX_NAME_LENGTH - 1);
    newAccount->name[MAX_NAME_LENGTH - 1] = '\0';
    newAccount->balance = initialDeposit;
    newAccount->accountType = accountType;

    bank->accountCount++;
    saveAccountsToFile(bank);
    return newAccount->accountNumber;
}

bool deleteAccount(Bank* bank, int accountNumber) {
    for (int i = 0; i < bank->accountCount; i++) {
        if (bank->accounts[i].accountNumber == accountNumber) {
            bank->accounts[i] = bank->accounts[bank->accountCount - 1];
            bank->accountCount--;
            saveAccountsToFile(bank);
            return true;
        }
    }
    return false;
}

Account* findAccount(Bank* bank, int accountNumber) {
    for (int i = 0; i < bank->accountCount; i++) {
        if (bank->accounts[i].accountNumber == accountNumber) {
            return &bank->accounts[i];
        }
    }
    return NULL;
}

bool deposit(Bank* bank, int accountNumber, double amount) {
    if (amount <= 0) {
        printf("Invalid deposit amount.\n");
        return false;
    }

    Account* account = findAccount(bank, accountNumber);
    if (account == NULL) {
        printf("Account not found.\n");
        return false;
    }

    account->balance += amount;
    printf("Deposit successful. New balance: $%.2f\n", account->balance);
    saveAccountsToFile(bank);
    return true;
}

bool withdraw(Bank* bank, int accountNumber, double amount) {
    if (amount <= 0) {
        printf("Invalid withdrawal amount.\n");
        return false;
    }

    Account* account = findAccount(bank, accountNumber);
    if (account == NULL) {
        printf("Account not found.\n");
        return false;
    }

    if (account->accountType == 'S') {
        if (account->balance - amount < MINIMUM_BALANCE) {
            printf("Withdrawal failed. Minimum balance requirement not met.\n");
            return false;
        }
    } else {
        if (account->balance - amount < -OVERDRAFT_LIMIT) {
            printf("Withdrawal failed. Overdraft limit exceeded.\n");
            return false;
        }
    }

    account->balance -= amount;
    printf("Withdrawal successful. New balance: $%.2f\n", account->balance);
    saveAccountsToFile(bank);
    return true;
}

void displayAccount(const Account* account) {
    if (account == NULL) {
        printf("Account not found.\n");
        return;
    }

    printf("\nAccount Details:\n");
    printf("Account Type: %s\n", account->accountType == 'S' ? "Savings" : "Checking");
    printf("Account Number: %d\n", account->accountNumber);
    printf("Name: %s\n", account->name);
    printf("Balance: $%.2f\n", account->balance);
}

void displayAllAccounts(const Bank* bank) {
    if (bank->accountCount == 0) {
        printf("No accounts to display.\n");
        return;
    }

    for (int i = 0; i < bank->accountCount; i++) {
        displayAccount(&bank->accounts[i]);
        printf("----------------------\n");
    }
}

void displayMenu() {
    printf("\n--- Bank Management System ---\n");
    printf("1. Create Savings Account\n");
    printf("2. Create Checking Account\n");
    printf("3. Deposit\n");
    printf("4. Withdraw\n");
    printf("5. Delete Account\n");
    printf("6. View Account Details\n");
    printf("7. View All Accounts\n");
    printf("8. Add Interest to Savings Accounts\n");
    printf("9. Save Bank Data\n");
    printf("10. Load Bank Data\n");
    printf("11. Exit\n");
    printf("Enter your choice: ");
}

void handleCreateAccount(Bank* bank) {
    char name[MAX_NAME_LENGTH];
    double initialDeposit;

    printf("Enter name: ");
    scanf(" %[^\n]s", name);
    printf("Enter initial deposit: ");
    scanf("%lf", &initialDeposit);

    int accountNumber = createAccount(bank, name, initialDeposit, 'S');
    if (accountNumber != -1) {
        printf("Account created successfully. Account Number: %d\n", accountNumber);
    }
}

void handleDeposit(Bank* bank) {
    int accountNumber;
    double amount;

    printf("Enter account number: ");
    scanf("%d", &accountNumber);
    printf("Enter amount to deposit: ");
    scanf("%lf", &amount);

    deposit(bank, accountNumber, amount);
}

void handleWithdraw(Bank* bank) {
    int accountNumber;
    double amount;

    printf("Enter account number: ");
    scanf("%d", &accountNumber);
    printf("Enter amount to withdraw: ");
    scanf("%lf", &amount);

    withdraw(bank, accountNumber, amount);
}

void handleDeleteAccount(Bank* bank) {
    int accountNumber;

    printf("Enter account number to delete: ");
    scanf("%d", &accountNumber);

    if (deleteAccount(bank, accountNumber)) {
        printf("Account deleted successfully.\n");
    } else {
        printf("Account not found.\n");
    }
}

void handleViewAccount(Bank* bank) {
    int accountNumber;

    printf("Enter account number: ");
    scanf("%d", &accountNumber);

    Account* account = findAccount(bank, accountNumber);
    displayAccount(account);
}

bool saveBankData(const Bank* bank) {
    FILE* file = fopen(FILENAME, "wb");
    if (file == NULL) {
        printf("Error opening file for writing.\n");
        return false;
    }

    // Write bank data
    fwrite(&bank->accountCount, sizeof(int), 1, file);
    fwrite(&bank->nextAccountNumber, sizeof(int), 1, file);
    
    // Write all accounts
    for (int i = 0; i < bank->accountCount; i++) {
        fwrite(&bank->accounts[i], sizeof(Account), 1, file);
    }

    fclose(file);
    printf("Bank data saved successfully.\n");
    return true;
}

bool loadBankData(Bank* bank) {
    FILE* file = fopen(FILENAME, "rb");
    if (file == NULL) {
        printf("No existing bank data found.\n");
        return false;
    }

    // Read bank data
    fread(&bank->accountCount, sizeof(int), 1, file);
    fread(&bank->nextAccountNumber, sizeof(int), 1, file);
    
    // Read all accounts
    for (int i = 0; i < bank->accountCount; i++) {
        fread(&bank->accounts[i], sizeof(Account), 1, file);
    }

    fclose(file);
    printf("Bank data loaded successfully.\n");
    return true;
}

// Add interest to savings accounts
void addInterestToSavings(Bank* bank) {
    const double INTEREST_RATE = 0.05;
    int i;
    for (i = 0; i < bank->accountCount; i++) {
        if (bank->accounts[i].accountType == 'S') {
            double interest = bank->accounts[i].balance * INTEREST_RATE;
            bank->accounts[i].balance += interest;
            printf("Interest added to account %d: $%.2f\n", 
                   bank->accounts[i].accountNumber, interest);
        }
    }
}

int main() {
    Bank bank;
    initializeBank(&bank);
    int choice;

    // Try to load existing data
    loadBankData(&bank);

    do {
        displayMenu();
        scanf("%d", &choice);
        getchar(); // Clear input buffer

        switch (choice) {
            case 1:
                handleCreateAccount(&bank);
                break;
            case 2:
                handleCreateAccount(&bank);
                break;
            case 3:
                handleDeposit(&bank);
                break;
            case 4:
                handleWithdraw(&bank);
                break;
            case 5:
                handleDeleteAccount(&bank);
                break;
            case 6:
                handleViewAccount(&bank);
                break;
            case 7:
                displayAllAccounts(&bank);
                break;
            case 8:
                addInterestToSavings(&bank);
                break;
            case 9:
                saveBankData(&bank);
                break;
            case 10:
                loadBankData(&bank);
                break;
            case 11:
                printf("Saving data before exit...\n");
                saveBankData(&bank);
                printf("Exiting. Thank you!\n");
                break;
            default:
                printf("Invalid choice. Try again.\n");
        }
    } while (choice != 11);

    return 0;
}
