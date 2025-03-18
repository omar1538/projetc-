#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <random>
#include <iomanip>
#include <functional>
#include <limits>

using namespace std;

class User {
public:
    User(const string& username, const string& hashedPassword, const string& salt)
        : username(username), hashedPassword(hashedPassword), salt(salt) {}

    const string& getUsername() const { return username; }
    const string& getHashedPassword() const { return hashedPassword; }
    const string& getSalt() const { return salt; }

private:
    string username;
    string hashedPassword;
    string salt;
};

class Utils {
public:
    static string generateSalt() {
        const int SALT_LENGTH = 16;
        static random_device rd;
        static mt19937 gen(rd());
        static uniform_int_distribution<> dis(0, 255);

        stringstream ss;
        for (int i = 0; i < SALT_LENGTH; i++) {
            ss << hex << setw(2) << setfill('0') << dis(gen);
        }
        return ss.str();
    }

    static string hashPassword(const string& password, const string& salt) {
        string combined = password + salt;
        hash<string> hasher;
        size_t hashValue = hasher(combined);

        stringstream ss;
        ss << hex << setw(16) << setfill('0') << hashValue;
        return ss.str();
    }
};

class AuthSystem {
public:
    AuthSystem() {
        cout << "Initializing AuthSystem...\n";
        loadUsers();
    }

    bool registerUser(const string& username, const string& password) {
        cout << "Attempting to register user: " << username << "\n";

        if (username.empty() || password.empty()) {
            cout << "Error: Username and password cannot be empty.\n";
            return false;
        }

        if (isUsernameTaken(username)) {
            cout << "Error: Username already exists.\n";
            return false;
        }

        if (!validatePassword(password)) {
            cout << "Error: Password must be at least 8 characters long and contain letters and numbers.\n";
            return false;
        }

        try {
            string salt = Utils::generateSalt();
            string hashedPassword = Utils::hashPassword(password, salt);

            users.emplace_back(username, hashedPassword, salt);
            saveUsers();
            return true;
        } catch (const exception& e) {
            cout << "Error during registration: " << e.what() << "\n";
            return false;
        }
    }

    bool login(const string& username, const string& password) {
        cout << "Attempting login for user: " << username << "\n";

        User* user = findUser(username);
        if (!user) {
            cout << "User not found\n";
            return false;
        }

        try {
            string hashedPassword = Utils::hashPassword(password, user->getSalt());
            return (hashedPassword == user->getHashedPassword());
        } catch (const exception& e) {
            cout << "Error during login: " << e.what() << "\n";
            return false;
        }
    }

private:
    static const string USER_FILE;
    vector<User> users;

    void loadUsers() {
        ifstream file(USER_FILE);
        if (!file.is_open()) return;

        users.clear();
        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            string username, hashedPassword, salt;

            getline(ss, username, ',');
            getline(ss, hashedPassword, ',');
            getline(ss, salt, ',');

            users.emplace_back(username, hashedPassword, salt);
        }
        file.close();
    }

    void saveUsers() {
        ofstream file(USER_FILE);
        for (const auto& user : users) {
            file << user.getUsername() << "," << user.getHashedPassword() << "," << user.getSalt() << "\n";
        }
        file.close();
    }

    bool isUsernameTaken(const string& username) {
        return findUser(username) != nullptr;
    }

    bool validatePassword(const string& password) {
        if (password.length() < 8) return false;
        bool hasLetter = false, hasNumber = false;
        for (char c : password) {
            if (isalpha(c)) hasLetter = true;
            if (isdigit(c)) hasNumber = true;
        }
        return hasLetter && hasNumber;
    }

    User* findUser(const string& username) {
        for (auto& user : users) {
            if (user.getUsername() == username) return &user;
        }
        return nullptr;
    }
};

const string AuthSystem::USER_FILE = "users.csv";

void displayMenu() {
    cout << "\n=== Authentication System ===\n";
    cout << "1. Register\n";
    cout << "2. Login\n";
    cout << "3. Exit\n";
    cout << "Choose an option: ";
}

int main() {
    AuthSystem authSystem;
    string input;
    int choice;

    while (true) {
        displayMenu();
        getline(cin, input);

        try { choice = stoi(input); } catch (...) { continue; }

        switch (choice) {
            case 1: {
                string username, password;
                cout << "Enter username: ";
                getline(cin, username);
                cout << "Enter password: ";
                getline(cin, password);
                if (authSystem.registerUser(username, password)) {
                    cout << "Registration successful!\n";
                }
                break;
            }
            case 2: {
                string username, password;
                cout << "Enter username: ";
                getline(cin, username);
                cout << "Enter password: ";
                getline(cin, password);
                if (authSystem.login(username, password)) {
                    cout << "Login successful!\n";
                } else {
                    cout << "Login failed. Invalid credentials.\n";
                }
                break;
            }
            case 3:
                cout << "Goodbye!\n";
                return 0;
            default:
                cout << "Invalid option.\n";
        }
    }
}
