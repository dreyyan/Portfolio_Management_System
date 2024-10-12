#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <map>
#include <fstream>
#include <sstream>

using namespace std;

class FinancialEntity
{
protected:
    string name;
    double currentValue;

public:
    FinancialEntity(const string &name, double value) : name(name), currentValue(value) {}

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
        else
        {

            cout << "Insufficient value to complete the transaction.\n";
        }
    }
    // Helper function to determine the type of the entity (Asset, Liability, Equity)
    virtual std::string getType() const = 0;
    virtual ~FinancialEntity() = default;
};

// Asset Class

class Asset : public FinancialEntity
{
public:
    Asset(const string &name, double value) : FinancialEntity(name, value) {}

    void showDetails() const override
    {
        cout << "Asset Name: " << name << "\n"
             << "Current Value: $" << currentValue << "\n";
    }
    std::string getType() const override { return "Asset"; }
};

// Liability Class

class Liability : public FinancialEntity
{
public:
    Liability(const string &name, double value) : FinancialEntity(name, value) {}

    void showDetails() const override
    {
        cout << "Liability Name: " << name << "\n"
             << "Current Value: $" << currentValue << "\n";
    }
    std::string getType() const override { return "Liability"; }
};

// Equity Class

class Equity : public FinancialEntity
{
public:
    Equity(const string &name, double value) : FinancialEntity(name, value) {}

    void showDetails() const override
    {
        cout << "Equity Name: " << name << "\n"
             << "Current Value: $" << currentValue << "\n";
    }
    std::string getType() const override { return "Equity"; }
};

// This class just overrides the method getEntityType
class FinancialManager
{
public:
    std::string getEntityType(const FinancialEntity &entity) const
    {
        return entity.getType();
    }
};

// The Transaction class keeps track of all transactions.
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
    // Adding an Entity
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

    const map<string, unique_ptr<FinancialEntity>> &getEntities() const
    {
        return entities;
    }

    // Function to display Portfolio

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

    // Function to search an Entity

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

    // Total value of the Portfolio

    double getTotalValue() const
    {
        double totalValue = 0;

        for (const auto &pair : entities)
        {
            totalValue += pair.second->getCurrentValue();
        }

        return totalValue;
    }

    // Buying an Entity

    void buyEntity(const string &name, double amount)
    {
        FinancialEntity *entity = searchEntity(name);

        if (entity)
        {
            Transaction::buy(*entity, amount);
        }
    }

    // Function to Sell an Entity

    void sellEntity(const string &name, double amount)
    {
        FinancialEntity *entity = searchEntity(name);

        if (entity)
        {
            Transaction::sell(*entity, amount);
        }
    }
};

// User Class

class User
{
private:
    unordered_map<string, string> users;

    string currentUsername;

    unordered_map<string, PortfolioManager> userPortfolios;

public:
    // User Registration

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

    // User Login
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
    // Returns the Username of currently logged in user.
    string getCurrentUsername() const
    {
        return currentUsername;
    }

    PortfolioManager &getPortfolioManager()
    {
        return userPortfolios[currentUsername];
    }
};

class FileHandler
{
public:
    FinancialManager manager;

    // Function to save the portfolio to a file

    void savePortfolio(const PortfolioManager &portfolio, const std::string &username)
    {
        // Considering filename of format username_portfolio.txt

        string filename = username + "_portfolio.txt";
        ofstream fio(filename);

        if (!fio.is_open())
        {
            std::cout << "Error opening file for saving!\n";
            return;
        }

        for (const auto &entityPair : portfolio.getEntities())
        {
            const auto &entity = entityPair.second;
            fio << entity->getName() << "," << entity->getCurrentValue() << ","
                << manager.getEntityType(*entity) << "\n";
        }

        fio.close();
        std::cout << "Portfolio saved to " << filename << "\n";
    }

    // Function to load the portfolio from a file.

    void loadPortfolio(PortfolioManager &portfolio, const std::string &username)
    {

        string filename = username + "_portfolio.txt";
        ifstream file(filename);

        if (!file.is_open())
        {
            std::cout << "Error opening file for loading!\n";
            return;
        }

        std::string line;
        while (std::getline(file, line))
        {
            std::string name, type;
            double value;
            parseLine(line, name, value, type);
            portfolio.addEntity(name, value, type);
        }

        file.close();
        std::cout << "Portfolio loaded from " << filename << "\n";
    }

private:
    // Helper function to read a line from the file
    void parseLine(const std::string &line, std::string &name, double &value, std::string &type)
    {
        stringstream ss(line);
        string valueStr;

        getline(ss, name, ',');
        getline(ss, valueStr, ',');
        value = stod(valueStr); // Convert the value to a double
        std::getline(ss, type, ',');
    }
};

int main()
{
    User userSystem;
    FileHandler filehandler;
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
                string currentUser = userSystem.getCurrentUsername();
                PortfolioManager &portfolio = userSystem.getPortfolioManager();
                filehandler.loadPortfolio(portfolio, currentUser); // Load existing portfolio

                int userChoice;
                while (true)
                {
                    cout << "\nPortfolio Management Options:\n";
                    cout << "1. Add Entity\n";
                    cout << "2. Show Portfolio\n";
                    cout << "3. Buy Entity\n";
                    cout << "4. Sell Entity\n";
                    cout << "5. Save Portfolio\n";
                    cout << "6. Logout\n";
                    cout << "Enter your choice: ";
                    cin >> userChoice;

                    if (userChoice == 1)
                    {
                        string name, type;
                        double value;
                        cout << "Enter entity name: ";
                        cin >> name;
                        cout << "Enter entity type (Asset/Liability/Equity): ";
                        cin >> type;
                        cout << "Enter entity value: ";
                        cin >> value;
                        portfolio.addEntity(name, value, type);
                    }
                    else if (userChoice == 2)
                    {
                        portfolio.showPortfolio();
                    }
                    else if (userChoice == 3)
                    {
                        string name;
                        double amount;
                        cout << "Enter entity name to buy: ";
                        cin >> name;
                        cout << "Enter amount to buy: ";
                        cin >> amount;
                        portfolio.buyEntity(name, amount);
                    }
                    else if (userChoice == 4)
                    {
                        string name;
                        double amount;
                        cout << "Enter entity name to sell: ";
                        cin >> name;
                        cout << "Enter amount to sell: ";
                        cin >> amount;
                        portfolio.sellEntity(name, amount);
                    }
                    else if (userChoice == 5)
                    {
                        filehandler.savePortfolio(portfolio, currentUser); // Save the portfolio
                    }
                    else if (userChoice == 6)
                    {
                        break; // Logout
                    }
                    else
                    {
                        cout << "Invalid choice! Please try again.\n";
                    }
                }
            }
        }
        else if (choice == 3)
        {
            break; // Exit
        }
        else
        {
            cout << "Invalid choice! Please try again.\n";
        }
    }

    return 0;
}
