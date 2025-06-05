# 💼 Bank Management System in C

This is a **console-based Bank Management System** written in **C**. It was my **first project after learning programming fundamentals**, and it simulates basic banking operations like account creation, deposits, withdrawals, and account management. All account data is stored persistently using **file handling** (`accounts.dat`), allowing data to be retained between program runs.

## 📌 Features
- ✅ Create Savings and Checking Accounts  
- 💰 Deposit and Withdraw Funds  
- 🔍 View Single or All Account Details  
- ❌ Delete Accounts  
- 💾 Persistent Data Storage via Binary Files  
- 📋 Menu-Driven User Interface

## 🧠 Account Rules
| Account Type | Constraint                          |
|--------------|-------------------------------------|
| Savings      | Minimum balance of 500             |
| Checking     | Overdraft limit up to 1000 allowed |

## 📂 File Handling
- All account data is stored in a binary file named `accounts.dat`.
- Data is automatically loaded at program start and saved after every operation.

## 🏗️ Data Structures

### `Account`
```c
typedef struct {
    int accountNumber;
    char name[50];
    double balance;
    char accountType; // 'S' for Savings, 'C' for Checking
} Account;
