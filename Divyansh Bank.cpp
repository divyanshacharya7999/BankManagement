#include <iostream>
#include <fstream>
#include <cstring>

using namespace std;

// Account structure
struct Account {
    int id;
    char name[50];
    double balance;
};

// Function prototypes
void createAccount();
void deposit();
void withdraw();
void showBalance();
void modifyAccount();
void deleteAccount();
void generateReport();
void displayMenu();

// Global variables
const char* ACCOUNTS_FILE = "accounts.dat";

int main() {
    displayMenu();
    return 0;
}

void createAccount() {
    ofstream fout(ACCOUNTS_FILE, ios::app | ios::binary);
    if (!fout) {
        cout << "Error opening file!" << endl;
        return;
    }

    Account account;
    cout << "Enter account id: ";
    cin >> account.id;
    cout << "Enter account name: ";
    cin.ignore();
    cin.getline(account.name, 50);
    cout << "Enter initial balance: ";
    cin >> account.balance;

    fout.write((char*)&account, sizeof(Account));
    fout.close();

    cout << "Account created successfully!" << endl;
}

void deposit() {
    int id;
    double amount;
    bool found = false;

    cout << "Enter account id: ";
    cin >> id;

    fstream file(ACCOUNTS_FILE, ios::in | ios::out | ios::binary);
    if (!file) {
        cout << "Error opening file!" << endl;
        return;
    }

    Account account;
    while (file.read((char*)&account, sizeof(Account))) {
        if (account.id == id) {
            found = true;
            cout << "Enter amount to deposit: ";
            cin >> amount;
            account.balance += amount;
            file.seekp(-sizeof(Account), ios::cur);
            file.write((char*)&account, sizeof(Account));
            break;
        }
    }

    file.close();

    if (found) {
        cout << "Deposit successful!" << endl;
        showBalance();
    } else {
        cout << "Account not found!" << endl;
    }
}

void withdraw() {
    int id;
    double amount;
    bool found = false;

    cout << "Enter account id: ";
    cin >> id;

    fstream file(ACCOUNTS_FILE, ios::in | ios::out | ios::binary);
    if (!file) {
        cout << "Error opening file!" << endl;
        return;
    }

    Account account;
    while (file.read((char*)&account, sizeof(Account))) {
        if (account.id == id) {
            found = true;
            cout << "Enter amount to withdraw: ";
            cin >> amount;
            if (account.balance >= amount) {
                account.balance -= amount;
                file.seekp(-sizeof(Account), ios::cur);
                file.write((char*)&account, sizeof(Account));
                cout << "Withdrawal successful!" << endl;
                showBalance();
            } else {
                cout << "Insufficient balance!" << endl;
            }
            break;
        }
    }

    file.close();

    if (!found) {
        cout << "Account not found!" << endl;
    }
}

void showBalance() {
    int id;
    bool found = false;

    cout << "Enter account id: ";
    cin >> id;

    ifstream file(ACCOUNTS_FILE, ios::binary);
    if (!file) {
        cout << "Error opening file!" << endl;
        return;
    }

    Account account;
    while (file.read((char*)&account, sizeof(Account))) {
        if (account.id == id) {
            found = true;
            cout << "Account balance: " << account.balance << endl;
            break;
        }
    }

    file.close();

    if (!found) {
        cout << "Account not found!" << endl;
    }
}

void modifyAccount() {
    int id;
    bool found = false;

    cout << "Enter account id: ";
    cin >> id;

    fstream file(ACCOUNTS_FILE, ios::in | ios::out | ios::binary);
    if (!file) {
        cout << "Error opening file!" << endl;
        return;
    }

    Account account;
    while (file.read((char*)&account, sizeof(Account))) {
        if (account.id == id) {
            found = true;
            cout << "Enter new account name: ";
            cin.ignore();
            cin.getline(account.name, 50);
            file.seekp(-sizeof(Account), ios::cur);
            file.write((char*)&account, sizeof(Account));
            cout << "Account modified successfully!" << endl;
            break;
        }
    }

    file.close();

    if (!found) {
        cout << "Account not found!" << endl;
    }
}

void deleteAccount() {
    int id;
    bool found = false;

    cout << "Enter account id: ";
    cin >> id;

    ifstream fileIn(ACCOUNTS_FILE, ios::binary);
    if (!fileIn) {
        cout << "Error opening file!" << endl;
        return;
    }

    ofstream fileOut("temp.dat", ios::binary);
    if (!fileOut) {
        cout << "Error opening file!" << endl;
        return;
    }

    Account account;
    while (fileIn.read((char*)&account, sizeof(Account))) {
        if (account.id != id) {
            fileOut.write((char*)&account, sizeof(Account));
        } else {
            found = true;
        }
    }

    fileIn.close();
    fileOut.close();

    if (found) {
        remove(ACCOUNTS_FILE);
        rename("temp.dat", ACCOUNTS_FILE);
        cout << "Account deleted successfully!" << endl;
    } else {
        remove("temp.dat");
        cout << "Account not found!" << endl;
    }
}

void generateReport() {
    ifstream file(ACCOUNTS_FILE, ios::binary);
    if (!file) {
        cout << "Error opening file!" << endl;
        return;
    }

    Account account;
    cout << "ID\tName\tBalance" << endl;
    while (file.read((char*)&account, sizeof(Account))) {
        cout << account.id << "\t" << account.name << "\t" << account.balance << endl;
    }

    file.close();
}

void displayMenu() {
    int choice;

    do {
        cout << endl;
        cout << "Bank Management System" << endl;
        cout << "1. Create account" << endl;
        cout << "2. Deposit" << endl;
        cout << "3. Withdraw" << endl;
        cout << "4. Show balance" << endl;
        cout << "5. Modify account" << endl;
        cout << "6. Delete account" << endl;
        cout << "7. Generate report" << endl;
        cout << "8. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                createAccount();
                break;
            case 2:
                deposit();
                break;
            case 3:
                withdraw();
                break;
            case 4:
                showBalance();
                break;
            case 5:
                modifyAccount();
                break;
            case 6:
                deleteAccount();
                break;
            case 7:
                generateReport();
                break;
            case 8:
                cout << "Exiting program..." << endl;
                break;
            default:
                cout << "Invalid choice!" << endl;
                break;
        }
    } while (choice != 8);
}