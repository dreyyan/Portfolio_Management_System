#include <iostream>   
#include <vector>      
#include <memory>      
#include <algorithm>   
#include <string>     
#define fast std::ios::sync_with_stdio(false), std::cin.tie(NULL), std::cout.tie(NULL)
#define ll long long
using namespace std;

class FinancialEntity {
protected:
    string name;
    double currentValue;

public:
    FinancialEntity(const string& name, double value)
        : name(name), currentValue(value) {}

    virtual ~FinancialEntity() = default;

    virtual void showDetails() const = 0; 
    virtual double getCurrentValue() const { return currentValue; }
    virtual string getName() const { return name; }
};

class Asset : public FinancialEntity {
public:
    Asset(const string& name, double value)
        : FinancialEntity(name, value) {}

    void showDetails() const override {
        cout << "Asset Name: " << name << "\n"
             << "Current Value: $" << currentValue << "\n";
    }
};

class Liability : public FinancialEntity {
public:
    Liability(const string& name, double value)
        : FinancialEntity(name, value) {}

    void showDetails() const override {
        cout << "Liability Name: " << name << "\n"
             << "Current Value: $" << currentValue << "\n";
    }
};

class MutualFund {
    string fundName;
    vector<unique_ptr<FinancialEntity>> entities;

public:
    MutualFund(const string& name) : fundName(name) {}

    void addEntity(unique_ptr<FinancialEntity> entity) {
        entities.push_back(move(entity));
    }

    void showPortfolio() const {
        cout << "Mutual Fund: " << fundName << "\n";
        for (const auto& entity : entities) {
            entity->showDetails();
            cout << "--------------------\n";
        }
    }

    double getTotalValue() const {
        double total = 0;
        for (const auto& entity : entities) {
            total += entity->getCurrentValue();
        }
        return total;
    }

    void sortPortfolioByValue() {
        sort(entities.begin(), entities.end(), [](const unique_ptr<FinancialEntity>& a, const unique_ptr<FinancialEntity>& b) {
            return a->getCurrentValue() > b->getCurrentValue();
        });
    }

    FinancialEntity* findEntityByName(const string& entityName) const {
        auto it = find_if(entities.begin(), entities.end(), [&entityName](const unique_ptr<FinancialEntity>& entity) {
            return entity->getName() == entityName;
        });
        return it != entities.end() ? it->get() : nullptr;
    }
};

class User {
    static int globalID;
    int userID;
    string username;
    string password;
    vector<MutualFund> userFunds;

public:
    User(const string& username, const string& password) 
        : username(username), password(password), userID(++globalID) {}

    int getUserID() const {
        return userID;
    }

    string getUsername() const {
        return username;
    }

    bool authenticate(const string& pass) const {
        return password == pass;
    }

    void addMutualFund(const MutualFund& fund) {
        userFunds.push_back(fund);
    }

    void showUserPortfolio() const {
        cout << "User ID: " << userID << " | Username: " << username << "\n";
        for (const auto& fund : userFunds) {
            fund.showPortfolio();
        }
    }
};

int User::globalID = 0;

class PortfolioManagementSystem {
    unordered_map<int, User> users;
    User* loggedInUser = nullptr;

public:
    void registerUser(const string& username, const string& password) {
        User newUser(username, password);
        users[newUser.getUserID()] = newUser;
        cout << "User registered with ID: " << newUser.getUserID() << "\n";
    }

    bool loginUser(const string& username, const string& password) {
        for (auto& [id, user] : users) {
            if (user.getUsername() == username && user.authenticate(password)) {
                loggedInUser = &user;
                cout << "Login successful! User ID: " << loggedInUser->getUserID() << "\n";
                return true;
            }
        }
        cout << "Login failed. Check your username and password.\n";
        return false;
    }

    void logoutUser() {
        loggedInUser = nullptr;
        cout << "Logged out successfully.\n";
    }

    User* getLoggedInUser() const {
        return loggedInUser;
    }

    void addUserMutualFund() {
        if (loggedInUser) {
            string fundName;
            cout << "Enter Mutual Fund name: ";
            cin >> fundName;

            MutualFund fund(fundName);

            char choice;
            do {
                cout << "Add (A)sset or (L)iability or (Q)uit: ";
                cin >> choice;

                if (choice == 'A' || choice == 'L') {
                    string entityName;
                    double value;
                    cout << "Enter name: ";
                    cin >> entityName;
                    cout << "Enter value: ";
                    cin >> value;

                    if (choice == 'A') {
                        fund.addEntity(make_unique<Asset>(entityName, value));
                    } else {
                        fund.addEntity(make_unique<Liability>(entityName, value));
                    }
                }
            } while (choice != 'Q');

            loggedInUser->addMutualFund(fund);
            cout << "Mutual Fund added successfully.\n";
        } else {
            cout << "No user is logged in.\n";
        }
    }
};

int main() {
    fast;
    PortfolioManagementSystem pms;
    int choice;

    do {
        cout << "\nPortfolio Management System Menu\n";
        cout << "1. Register User\n";
        cout << "2. Login User\n";
        cout << "3. Add Mutual Fund to Portfolio\n";
        cout << "4. View User Portfolio\n";
        cout << "5. Logout\n";
        cout << "6. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: {
                string username, password;
                cout << "Enter username: ";
                cin >> username;
                cout << "Enter password: ";
                cin >> password;
                pms.registerUser(username, password);
                break;
            }
            case 2: {
                string username, password;
                cout << "Enter username: ";
                cin >> username;
                cout << "Enter password: ";
                cin >> password;
                pms.loginUser(username, password);
                break;
            }
            case 3: {
                pms.addUserMutualFund();
                break;
            }
            case 4: {
                User* user = pms.getLoggedInUser();
                if (user) {
                    user->showUserPortfolio();
                } else {
                    cout << "No user is logged in.\n";
                }
                break;
            }
            case 5: {
                pms.logoutUser();
                break;
            }
            case 6: {
                cout << "Exiting...\n";
                break;
            }
            default:
                cout << "Invalid choice! Please try again.\n";
        }
    } while (choice != 6);

    return 0;
}
