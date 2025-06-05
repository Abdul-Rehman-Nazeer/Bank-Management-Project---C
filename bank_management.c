#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_ACCOUNTS 100
#define MAX_NAME_LENGTH 50
#define MINIMUM_BALANCE 500
#define OVERDRAFT_LIMIT 1000

// Structure for account
typedef struct {
    int accountNumber;
    char name[MAX_NAME_LENGTH];
    double balance;
    char accountType; // 'S' for Savings, 'C' for Checking
} Account;

// Structure for bank
typedef struct {
    Account accounts[MAX_ACCOUNTS];
    int accountCount;
    int nextAccountNumber;
} Bank;

// Function prototypes
void initializeBank(Bank* bank);
int createAccount(Bank* bank, const char* name, double initialDeposit, char accountType);
bool deleteAccount(Bank* bank, int accountNumber);
Account* findAccount(Bank* bank, int accountNumber);
bool deposit(Bank* bank, int accountNumber, double amount);
bool withdraw(Bank* bank, int accountNumber, double amount);
void displayAccount(const Account* account);
void displayAllAccounts(const Bank* bank);
void displayMenu();
void handleCreateAccount(Bank* bank);
void handleDeposit(Bank* bank);
void handleWithdraw(Bank* bank);
void handleDeleteAccount(Bank* bank);
void handleViewAccount(Bank* bank);

// Initialize bank
void initializeBank(Bank* bank) {
    bank->accountCount = 0;
    bank->nextAccountNumber = 1001;
}

// Create a new account
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
    return newAccount->accountNumber;
}

// Delete an account
bool deleteAccount(Bank* bank, int accountNumber) {
    int i;
    for (i = 0; i < bank->accountCount; i++) {
        if (bank->accounts[i].accountNumber == accountNumber) {
            // Move the last account to this position
            bank->accounts[i] = bank->accounts[bank->accountCount - 1];
            bank->accountCount--;
            return true;
        }
    }
    return false;
}

// Find an account
Account* findAccount(Bank* bank, int accountNumber) {
    int i;
    for (i = 0; i < bank->accountCount; i++) {
        if (bank->accounts[i].accountNumber == accountNumber) {
            return &bank->accounts[i];
        }
    }
    return NULL;
}

// Deposit money
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
    return true;
}

// Withdraw money
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
        // Savings account - check minimum balance
        if (account->balance - amount < MINIMUM_BALANCE) {
            printf("Withdrawal failed. Minimum balance requirement not met.\n");
            return false;
        }
    } else {
        // Checking account - check overdraft limit
        if (account->balance - amount < -OVERDRAFT_LIMIT) {
            printf("Withdrawal failed. Overdraft limit exceeded.\n");
            return false;
        }
    }

    account->balance -= amount;
    printf("Withdrawal successful. New balance: $%.2f\n", account->balance);
    return true;
}

// Display account details
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

// Display all accounts
void displayAllAccounts(const Bank* bank) {
    int i;
    if (bank->accountCount == 0) {
        printf("No accounts to display.\n");
        return;
    }

    for (i = 0; i < bank->accountCount; i++) {
        displayAccount(&bank->accounts[i]);
        printf("----------------------\n");
    }
}


// Display menu
void displayMenu() {
    printf("\n--- Bank Management System ---\n");
    printf("1. Create Savings Account\n");
    printf("2. Create Checking Account\n");
    printf("3. Deposit\n");
    printf("4. Withdraw\n");
    printf("5. Delete Account\n");
    printf("6. View Account Details\n");
    printf("7. View All Accounts\n");
    printf("8. Exit\n");
    printf("Enter your choice: ");
}

// Handle account creation
void handleCreateAccount(Bank* bank) {
    char name[MAX_NAME_LENGTH];
    double initialDeposit;
    char accountType;

    printf("Enter name: ");
    scanf(" %[^\n]s", name);
    printf("Enter initial deposit: ");
    scanf("%lf", &initialDeposit);

    int accountNumber = createAccount(bank, name, initialDeposit, accountType);
    if (accountNumber != -1) {
        printf("Account created successfully. Account Number: %d\n", accountNumber);
    }
}

// Handle deposit
void handleDeposit(Bank* bank) {
    int accountNumber;
    double amount;

    printf("Enter account number: ");
    scanf("%d", &accountNumber);
    printf("Enter amount to deposit: ");
    scanf("%lf", &amount);

    deposit(bank, accountNumber, amount);
}

// Handle withdrawal
void handleWithdraw(Bank* bank) {
    int accountNumber;
    double amount;

    printf("Enter account number: ");
    scanf("%d", &accountNumber);
    printf("Enter amount to withdraw: ");
    scanf("%lf", &amount);

    withdraw(bank, accountNumber, amount);
}

// Handle account deletion
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

// Handle viewing account
void handleViewAccount(Bank* bank) {
    int accountNumber;

    printf("Enter account number: ");
    scanf("%d", &accountNumber);

    Account* account = findAccount(bank, accountNumber);
    displayAccount(account);
}

int main() {
    Bank bank;
    initializeBank(&bank);
    int choice;

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
                printf("Exiting. Thank you!\n");
                break;
            default:
                printf("Invalid choice. Try again.\n");
        }
    } while (choice != 8);

    return 0;
}
