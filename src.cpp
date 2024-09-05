#include <bits/stdc++.h>
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
    string username;
    string password;
    vector<MutualFund> userFunds;

public:
    User(const string& username, const string& password) 
        : username(username), password(password) {}

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
        cout << "User: " << username << "\n";
        for (const auto& fund : userFunds) {
            fund.showPortfolio();
        }
    }
};

class PortfolioManagementSystem {
    unordered_map<string, User> users;
    User* loggedInUser = nullptr;

public:
    void registerUser(const string& username, const string& password) {
        users[username] = User(username, password);
    }

    bool loginUser(const string& username, const string& password) {
        auto it = users.find(username);
        if (it != users.end() && it->second.authenticate(password)) {
            loggedInUser = &it->second;
            cout << "Login successful!\n";
            return true;
        } else {
            cout << "Login failed. Check your username and password.\n";
            return false;
        }
    }

    void logoutUser() {
        loggedInUser = nullptr;
        cout << "Logged out successfully.\n";
    }

    User* getLoggedInUser() const {
        return loggedInUser;
    }
};

int main() {
    PortfolioManagementSystem pms;
    pms.registerUser("john_doe", "password123");

    if (pms.loginUser("john_doe", "password123")) {
        User* user = pms.getLoggedInUser();
        
        MutualFund mf1("Retirement Fund");
        mf1.addEntity(make_unique<Asset>("Real Estate", 100000));
        mf1.addEntity(make_unique<Liability>("Loan", -50000));

        user->addMutualFund(mf1);

        user->showUserPortfolio();
        
        pms.logoutUser();
    }

    return 0;
}
