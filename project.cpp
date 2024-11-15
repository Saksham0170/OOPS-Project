#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
using namespace std;

// Class to represent a bank account
class BankAccount {
private:
    int accountNumber;
    string accountHolderName;
    double balance;

public:
    // Default constructor
    BankAccount() : accountNumber(0), accountHolderName(""), balance(0.0) {}

    // Parameterized constructor
    BankAccount(int accNum, string accHolderName, double initialBalance) {
        accountNumber = accNum;
        accountHolderName = accHolderName;
        balance = initialBalance;
    }

    // Function to display account details
    void displayAccountDetails() const {
        cout << "Account Number: " << accountNumber << endl;
        cout << "Account Holder: " << accountHolderName << endl;
        cout << "Balance: $" << fixed << setprecision(2) << balance << endl;
    }

    // Function to deposit money
    void deposit(double amount) {
        if (amount > 0) {
            balance += amount;
            cout << "Deposited: $" << amount << endl;
        } else {
            cout << "Invalid deposit amount!" << endl;
        }
    }
  
    // Function to withdraw money
    void withdraw(double amount) {      
        balance -= amount;
        cout << "Withdrawn: $" << amount << endl;
    }
 
    // Function to save account details to the file
    void saveAccountToFile(fstream& file) {
        file << accountNumber << "\n";
        file << accountHolderName << "\n";
        file << balance << "\n";
    }

    // Function to load account details from the file
    void loadAccountFromFile(fstream& file) {
        file >> accountNumber;
        file.ignore(); // to skip newline character after account number
        getline(file, accountHolderName);
        file >> balance;
    }

    // Getter for account number
    int getAccountNumber() const {
        return accountNumber;
    }
    double getBal() const{
        return balance;
    }
};

// Function to withdraw money from an account
void withdrawMoney(fstream& file, const string& filename, int accNum, double amount) {
    // Open file in read-write mode to modify the content
    file.clear();  // Clear any EOF or error flags
    file.seekg(0, ios::beg);  // Move the file pointer to the beginning

    bool accountFound = false;
    fstream tempFile("temp_accounts.txt", ios::out);  // Temporary file to store updated data

    while (!file.eof()) {
        BankAccount account;
        account.loadAccountFromFile(file);

        if (file) {
            // Check if this is the correct account number
            if (account.getAccountNumber() == accNum) {
                // Try to withdraw the amount
                if (amount > 0 && amount <= account.getBal()) {
                    account.withdraw(amount);  // Withdraw the money
                    accountFound = true;
                } else {
                    cout << "Insufficient funds or invalid amount!" << endl;
                }
            }
            // Save the updated account to the temporary file
            account.saveAccountToFile(tempFile);
        }
    }

    if (!accountFound) {
        cout << "Account not found!" << endl;
    }

    // Close files
    file.close();
    tempFile.close();

    // Now, open the original file in write mode, and copy content from temp file back
    file.open(filename, ios::out | ios::trunc);  // Open file for writing (clear existing content using trunc mode)

    // Now copy content from temp file to the original file
    fstream tempFileRead("temp_accounts.txt", ios::in);
    while (!tempFileRead.eof()) {
        string line;
        getline(tempFileRead, line);
        file << line << endl;
    }

    tempFileRead.close();
    file.close();
    file.open(filename, ios::in | ios::out | ios::app); //Open file again in original state
    remove("temp_accounts.txt"); // Remove temp file
}

// Function to deposit money into an account
void depositMoney(fstream& file, const string& filename, int accNum, double amount) {
    // Open file in read-write mode to modify the content
    file.clear();  // Clear any EOF or error flags
    file.seekg(0, ios::beg);  // Move the file pointer to the beginning

    bool accountFound = false;
    fstream tempFile("temp_accounts.txt", ios::out);  // Temporary file to store updated data

    while (!file.eof()) {
        BankAccount account;
        account.loadAccountFromFile(file);

        if (file) {
            // Check if this is the correct account number
            if (account.getAccountNumber() == accNum) {
                account.deposit(amount);  // Deposit the money into the account
                accountFound = true;
            }
            // Save the updated account to the temporary file
            account.saveAccountToFile(tempFile);
        }
    }

    if (!accountFound) {
        cout << "Account not found!" << endl;
    }

    // Close files
    file.close();
    tempFile.close();

    // Now, open the original file in write mode, and copy content from temp file back
    file.open(filename, ios::out | ios::trunc);  // Open file for writing (clear existing content using trunc mode)

    // Now copy content from temp file to the original file
    fstream tempFileRead("temp_accounts.txt", ios::in);
    while (!tempFileRead.eof()) {
        string line;
        getline(tempFileRead, line);
        file << line << endl;
    }

    tempFileRead.close();
    file.close();
    file.open(filename, ios::in | ios::out | ios::app);
    remove("temp_accounts.txt"); // Optionally remove temp file
}


// Function to get the next account number from the last used number
int getNextAccountNumber(const string& filename) {
    ifstream file(filename);
    int lastAccountNumber = 0; // Default starting account number
    if (file) {
        file >> lastAccountNumber;
    }
    file.close();

    // Increment the account number for the new account
    lastAccountNumber++;
    // Update the last account number in the file
    ofstream outFile(filename);
    outFile << lastAccountNumber;
    outFile.close();

    return lastAccountNumber;
}

// Function to create a new bank account
void createAccount(fstream& file, const string& accountNumFile) {
    string accHolderName;
    double initialBalance;

    // Get the next available account number
    int accNum = getNextAccountNumber(accountNumFile);

    cin.ignore(); // Clear input buffer before taking string input
    cout << "Enter Account Holder Name: ";
    getline(cin, accHolderName);
    cout << "Enter Initial Balance: $";
    cin >> initialBalance;

    BankAccount newAccount(accNum, accHolderName, initialBalance);
    newAccount.saveAccountToFile(file);
    cout << "Account with account number " <<accNum<<" created successfully!" << endl;
}

// Function to display all accounts from the file
void displayAllAccounts(fstream& file) {
    file.clear(); // Clear any EOF or error flags
    file.seekg(0, ios::beg); // Move file pointer to the beginning

    cout << "All Bank Accounts:" << endl;
    while (!file.eof()) {
        BankAccount account;
        account.loadAccountFromFile(file);
        if (file) {
            account.displayAccountDetails();
            cout << "--------------------------------" << endl;
        }
    }
}

// Function to search an account by account number
void searchAccount(fstream& file, int accNum) {
    file.clear(); // Clear any EOF or error flags
    file.seekg(0, ios::beg); // Move file pointer to the beginning

    bool found = false;
    while (!file.eof()) {
        BankAccount account;
        account.loadAccountFromFile(file);
        if (file) {
            if (account.getAccountNumber() == accNum) {
                account.displayAccountDetails();
                found = true;
                break;
            }
        }
    }

    if (!found) {
        cout << "Account not found!" << endl;
    }
}


int main() {
    string filename = "accounts.txt"; //Account info is stored here
    string accountNumFile = "last_account_number.txt"; //Last account number which was alloted is stored here
    fstream file;
    file.open(filename, ios::in | ios::out | ios::app); // Open file in append mode

    if (!file) {
        cout << "File could not be opened! Exiting program..." << endl;
        return -1;
    }

    // Check if last_account_number.txt exists, create if not
    ifstream accountNumFileStream(accountNumFile);
    if (!accountNumFileStream) {
        // If the file doesn't exist, create it and initialize with 1000 as the first account number
        ofstream outFile(accountNumFile);
        outFile << 0;
        outFile.close();
    }
    
    int choice;
    do {
        cout << "\n--- Bank Management System ---" << endl;
        cout << "1. Create Account" << endl;
        cout << "2. Display All Accounts" << endl;
        cout << "3. Search Account" << endl;
        cout << "4. Deposit Money" << endl;
        cout << "5. Withdraw Money" << endl;
        cout << "6. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        if (choice == 1) {
            createAccount(file, accountNumFile);
        } else if (choice == 2) {
            displayAllAccounts(file);
        } else if (choice == 3) {
            int accNum;
            cout << "Enter Account Number to search: ";
            cin >> accNum;
            searchAccount(file, accNum);
        } else if (choice == 4) {
            int accNum;
            double amount;
            cout << "Enter Account Number to deposit: ";
            cin >> accNum;
            cout << "Enter deposit amount: $";
            cin >> amount;
            depositMoney(file,"accounts.txt",accNum,amount);
        } else if (choice == 5) {
            int accNum;
            double amount;
            cout << "Enter Account Number to withdraw: ";
            cin >> accNum;
            cout << "Enter withdrawal amount: $";
            cin >> amount;
            withdrawMoney(file,"accounts.txt",accNum,amount);

        } else if (choice == 6) {
            cout << "Exiting... Thank you!" << endl;
        } else {
            cout << "Invalid choice. Please try again." << endl;
        }
    } while (choice != 6);

    file.close(); // Close the file before exiting
    return 0;
}