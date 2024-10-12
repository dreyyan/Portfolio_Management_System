#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <map>

using namespace std;

class FinancialEntity 
{
protected:
    string name;
    double currentValue;

public:
    FinancialEntity(const string &name, double value) : name(name), currentValue(value) {}
    virtual ~FinancialEntity() = default;

    virtual void showDetails() const = 0; 
    virtual double getCurrentValue() const { return currentValue; }
    virtual string getName() const { return name; }

    void setValue(double newValue) 
    {
        currentValue = newValue;
    }

    void addValue(double value) 
    {
        currentValue += value;
    }

    void subtractValue(double value) 
    {
        if (value <= currentValue)
        {
            currentValue -= value;
        }
        else{

            cout << "Insufficient value to complete the transaction.\n";
        }
    }

};

class Asset : public FinancialEntity 
{
public:
    Asset(const string &name, double value) : FinancialEntity(name, value) {}

    void showDetails() const override 
    {
        cout << "Asset Name: " << name << "\n"
             << "Current Value: $" << currentValue << "\n";
    }

};

class Liability : public FinancialEntity 
{
public:
    Liability(const string &name, double value) : FinancialEntity(name, value) {}

    void showDetails() const override 
    {
        cout << "Liability Name: " << name << "\n"
             << "Current Value: $" << currentValue << "\n";
    }

};

class Equity : public FinancialEntity 
{
public:
    Equity(const string &name, double value) : FinancialEntity(name, value) {}

    void showDetails() const override 
    {
        cout << "Equity Name: " << name << "\n"
             << "Current Value: $" << currentValue << "\n";
    }

};

class Transaction 
{
public:
    static void buy(FinancialEntity &entity, double amount) 
    {
        entity.addValue(amount);
        cout << "Bought $" << amount << " of " << entity.getName() << ".\n";
    }

    static void sell(FinancialEntity &entity, double amount) 
    {
        entity.subtractValue(amount);
        cout << "Sold $" << amount << " of " << entity.getName() << ".\n";
    }

};

class PortfolioManager 
{
private:
    map<string, unique_ptr<FinancialEntity>> entities;

public:
    void addEntity(const string &name, double value, const string &type) 
    {
        if (entities.find(name) != entities.end()) 
        {
            entities[name]->addValue(value);
        } 

        else 
        {
            if (type == "Asset") 
            {
                entities[name] = make_unique<Asset>(name, value);
            } 

            else if (type == "Liability") 
            {
                entities[name] = make_unique<Liability>(name, value);
            } 

            else if (type == "Equity") 
            {
                entities[name] = make_unique<Equity>(name, value);
            } 

            else 
            {
                cout << "Invalid financial entity type!\n";
            }

        }
    }

    void showPortfolio() const 
    {
        if (entities.empty()) 
        {
            cout << "No financial entities in the portfolio!\n";
        } 

        else 
        {
            for (const auto &pair : entities) 
            {
                pair.second->showDetails();
                cout << "------------------------\n";
            }
        }

    }

    FinancialEntity *searchEntity(const string &name) const 
    {
        auto it = entities.find(name);

        if (it != entities.end()) 
        {
            return it->second.get();
        } 

        else 
        {
            cout << "Entity not found.\n";
            return nullptr;
        }

    }

    double getTotalValue() const 
    {
        double totalValue = 0;

        for (const auto &pair : entities) 
        {
            totalValue += pair.second->getCurrentValue();
        }

        return totalValue;
    }

    void buyEntity(const string &name, double amount) 
    {
        FinancialEntity *entity = searchEntity(name);

        if (entity) 
        {
            Transaction::buy(*entity, amount);
        }

    }

    void sellEntity(const string &name, double amount) 
    {
        FinancialEntity *entity = searchEntity(name);

        if (entity) 
        {
            Transaction::sell(*entity, amount);
        }

    }
};

class User 
{
private:
    unordered_map<string, string> users; 

    string currentUsername;

    unordered_map<string, PortfolioManager> userPortfolios; 

public:
    void registerUser() 
    {
        string username, password;
        cout << "Enter a username: ";
        cin >> username;

        if (users.find(username) != users.end()) 
        {
            cout << "Username already exists. Please try again.\n";
            return;
        }

        cout << "Enter a password: ";
        cin >> password;

        users[username] = password;
        userPortfolios[username] = PortfolioManager(); 

        cout << "User registered successfully!\n";

    }

    bool loginUser() 
    {
        string username, password;
        cout << "Enter username: ";
        cin >> username;

        cout << "Enter password: ";
        cin >> password;

        if (users.find(username) != users.end() && users[username] == password) 
        {
            currentUsername = username;
            cout << "Login successful! Welcome, " << username << ".\n";
            return true;
        } 

        else 
        {
            cout << "Invalid username or password.\n";
            return false;
        }

    }

    PortfolioManager &getPortfolioManager() 
    {
        return userPortfolios[currentUsername];
    }

};

int main() 
{
    User userSystem;
    int choice;

    while (true) 
    {
        cout << "\n1. Register\n";
        cout << "2. Login\n";
        cout << "3. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        if (choice == 1) 
        {
            userSystem.registerUser();
        } 

        else if (choice == 2) 
        {
            if (userSystem.loginUser()) 
            {
                PortfolioManager &portfolio = userSystem.getPortfolioManager();
                int portfolioChoice;
                while (true) 
                {
                    cout << "\n----- Portfolio Management -----\n";
                    cout << "1. Add Asset\n";
                    cout << "2. Add Liability\n";
                    cout << "3. Add Equity\n";
                    cout << "4. Display All Entities\n";
                    cout << "5. Search for Entity\n";
                    cout << "6. Get Total Value of Portfolio\n";
                    cout << "7. Buy More of an Asset/Equity\n";
                    cout << "8. Sell Asset/Equity\n";
                    cout << "9. Logout\n";
                    cout << "Enter your choice: ";
                    cin >> portfolioChoice;

                    if (portfolioChoice == 9) 
                    {
                        cout << "Logging out...\n";
                        break;
                    }

                    // Portfolio actions similar to before
                    string name;
                    double value;
                    switch (portfolioChoice) 
                    {
                        case 1:

                            cout << "Enter Asset Name: ";
                            cin >> name;
                            cout << "Enter Asset Value: ";
                            cin >> value;
                            portfolio.addEntity(name, value, "Asset");
                            break;

                        case 2:

                            cout << "Enter Liability Name: ";
                            cin >> name;
                            cout << "Enter Liability Value: ";
                            cin >> value;
                            portfolio.addEntity(name, value, "Liability");
                            break;

                        case 3:

                            cout << "Enter Equity Name: ";
                            cin >> name;
                            cout << "Enter Equity Value: ";
                            cin >> value;
                            portfolio.addEntity(name, value, "Equity");
                            break;

                        case 4:

                            portfolio.showPortfolio();
                            break;

                        case 5:

                            cout << "Enter the name of the entity to search: ";
                            cin >> name;
                            portfolio.searchEntity(name);
                            break;

                        case 6:

                            cout << "Total Portfolio Value: $" << portfolio.getTotalValue() << "\n";
                            break;

                        case 7:

                            cout << "Enter the name of the Asset/Equity to buy more: ";
                            cin >> name;
                            cout << "Enter amount to buy: ";
                            cin >> value;
                            portfolio.buyEntity(name, value);
                            break;

                        case 8:

                            cout << "Enter the name of the Asset/Equity to sell: ";
                            cin >> name;
                            cout << "Enter amount to sell: ";
                            cin >> value;
                            portfolio.sellEntity(name, value);
                            break;

                        default:

                            cout << "Invalid choice. Please try again.\n";

                    }
                }
            }
        } 

        else if (choice == 3) 
        {
            cout << "Exiting system...\n";
            break;
        } 

        else 
        {
            cout << "Invalid choice! Please try again.\n";
        }
        
    }

    return 0;
}
