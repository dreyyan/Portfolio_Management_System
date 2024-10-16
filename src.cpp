#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <map>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <stdexcept>
#include <iomanip>
#include <ctime>
#include <cmath>

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
    string asset;
    double amount;
    string type;
    time_t date;

    Transaction(const string &assetName, double transactionAmount, const string &transactionType)
        : asset(assetName), amount(transactionAmount), type(transactionType), date(time(nullptr)) {}

    string getDateString() const
    {
        char buffer[20];
        struct tm *tm_info = localtime(&date);
        strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", tm_info);
        return buffer;
    }
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
                if (value < 0)
                {
                    cout << "Asset value cannot be negative!\n";
                    cout << "Shall I add in Liability instead? (y/n): ";

                    char choice;
                    cin >> choice;

                    if (choice == 'y' || choice == 'Y')
                    {
                        entities[name] = make_unique<Liability>(name, value);
                    }

                    else
                    {
                        entities[name] = make_unique<Asset>(name, -1 * value);
                    }
                }

                else
                {
                    entities[name] = make_unique<Asset>(name, value);
                }
            }

            else if (type == "Liability")
            {
                if (value > 0)
                {
                    cout << "Liability value cannot be positive!\n";
                    cout << "Shall I add in Asset instead? (y/n): ";
                    char choice;
                    cin >> choice;

                    if (choice == 'y' || choice == 'Y')
                    {
                        entities[name] = make_unique<Asset>(name, value);
                    }

                    else
                    {
                        entities[name] = make_unique<Liability>(name, -1 * value);
                    }
                }

                else
                {
                    entities[name] = make_unique<Liability>(name, value);
                }
            }

            else if (type == "Equity")
            {
                if (value < 0)
                {
                    cout << "Equity value cannot be negative!\n";
                    cout << "Shall I add in Liability instead? (y/n): ";
                    char choice;
                    cin >> choice;

                    if (choice == 'y' || choice == 'Y')
                    {
                        entities[name] = make_unique<Liability>(name, value);
                    }

                    else
                    {
                        entities[name] = make_unique<Equity>(name, -1 * value);
                    }
                }

                else
                {
                    entities[name] = make_unique<Equity>(name, value);
                }
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
    string currentUsername;
    unordered_map<string, PortfolioManager> userPortfolios;

public:
    // Check if a user exists in the file
    bool userExists(const string &username)
    {
        ifstream file("users.txt");
        string line, savedUsername;
        while (getline(file, line))
        {
            stringstream ss(line);
            getline(ss, savedUsername, ',');

            if (savedUsername == username)
            {
                return true;
            }
        }
        return false;
    }

    // User Registration
    void registerUser()
    {
        string username, password;
        cout << "Enter a username: ";
        cin >> username;

        if (userExists(username))
        {
            cout << "Username already exists. Please try again.\n";
            return;
        }

        cout << "Enter a password: ";
        cin >> password;

        // Save new user to the file
        ofstream file("users.txt", ios::app);
        if (file.is_open())
        {
            file << username << "," << password << "\n";
            file.close();
            cout << "User registered successfully!\n";
        }

        else
        {
            cout << "Error: Could not open file for saving.\n";
        }

        userPortfolios[username] = PortfolioManager();
    }

    // User Login
    bool loginUser()
    {
        string username, password;

        cout << "Enter username: ";
        cin >> username;

        cout << "Enter password: ";
        cin >> password;

        // Check if the username and password match any entry in users.txt
        ifstream file("users.txt");
        string line, savedUsername, savedPassword;

        while (getline(file, line))
        {
            stringstream ss(line);
            getline(ss, savedUsername, ','); // Username
            getline(ss, savedPassword, ','); // Password

            if (savedUsername == username && savedPassword == password)
            {
                currentUsername = username;
                cout << "Login successful! Welcome, " << username << ".\n";
                return true;
            }
        }

        // If no match found
        cout << "Invalid username or password.\n";
        return false;
    }

    // Returns the username of the currently logged in user
    string getCurrentUsername() const
    {
        return currentUsername;
    }

    // Access the portfolio manager of the logged in user
    PortfolioManager &getPortfolioManager()
    {
        return userPortfolios[currentUsername];
    }
};

// FileHandler class handles the portfolio files.
class FileHandler
{
public:
    FinancialManager manager;

    // Function to save the portfolio to a file

    void savePortfolio(const PortfolioManager &portfolio, const string &username)
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
        cout << "Portfolio loaded from " << filename << "\n";
    }

private:
    // Helper function to read a line from the file
    void parseLine(const std::string &line, std::string &name, double &value, std::string &type)
    {
        stringstream ss(line);
        string valueStr;

        getline(ss, name, ',');
        getline(ss, valueStr, ',');

        value = stod(valueStr);
        getline(ss, type, ',');
    }
};


class PortfolioAnalytics
{
public:
    // Function to calculate total assets value
    double totalAssets(const PortfolioManager &portfolio) const
    {
        double total = 0;
        for (const auto &entityPair : portfolio.getEntities())
        {
            if (entityPair.second->getType() == "Asset")
            {
                total += entityPair.second->getCurrentValue();
            }
        }
        return total;
    }

    // Function to calculate total liabilities value
    double totalLiabilities(const PortfolioManager &portfolio) const
    {
        double total = 0;
        for (const auto &entityPair : portfolio.getEntities())
        {
            if (entityPair.second->getType() == "Liability")
            {
                total += entityPair.second->getCurrentValue();
            }
        }
        return total;
    }

    // Function to calculate total equities value
    double totalEquities(const PortfolioManager &portfolio) const
    {
        double total = 0;
        for (const auto &entityPair : portfolio.getEntities())
        {
            if (entityPair.second->getType() == "Equity")
            {
                total += entityPair.second->getCurrentValue();
            }
        }
        return total;
    }

    // Function to generate a summary report
    void showReport(const PortfolioManager &portfolio) const
    {
        double assets = totalAssets(portfolio);
        double liabilities = totalLiabilities(portfolio);
        double equities = totalEquities(portfolio);
        double netValue = assets + equities - liabilities;

        cout << "\n--- Portfolio Summary Report ---\n";
        cout << "Total Assets: $" << assets << "\n";
        cout << "Total Liabilities: $" << liabilities << "\n";
        cout << "Total Equities: $" << equities << "\n";
        cout << "Net Portfolio Value: $" << netValue << "\n";
        cout << "---------------------------------\n";
    }

    // Function to show distribution of entities by type
    void entityDistribution(const PortfolioManager &portfolio) const
    {
        map<string, int> distribution;

        for (const auto &entityPair : portfolio.getEntities())
        {
            string type = entityPair.second->getType();
            distribution[type]++;
        }

        cout << "\n--- Portfolio Entity Distribution ---\n";
        for (const auto &entry : distribution)
        {
            cout << entry.first << ": " << entry.second << "\n";
        }
        cout << "------------------------------------\n";
    }
};

class TransactionHistory
{
private:
    vector<Transaction> transactions; // Stores all transactions

public:
    // Add a transaction
    void add(const string &asset, double amount, const string &type)
    {
        transactions.emplace_back(asset, amount, type);
    }

    // Display all transactions
    void show() const
    {
        cout << "\n--- Transaction History ---\n";
        for (const auto &tran : transactions)
        {
            cout << "Asset: " << tran.asset << ", Amount: $" << tran.amount
                 << ", Type: " << tran.type << ", Date: " << tran.getDateString() << endl;
        }
        cout << "----------------------------\n";
    }

    // Search transactions by asset
    void search(const string &asset) const
    {
        cout << "\n--- Transactions for Asset: " << asset << " ---\n";
        bool found = false;
        for (const auto &tran : transactions)
        {
            if (tran.asset == asset)
            {
                cout << "Asset: " << tran.asset << ", Amount: $" << tran.amount
                     << ", Type: " << tran.type << ", Date: " << tran.getDateString() << endl;
                found = true;
            }
        }
        if (!found)
            cout << "No transactions found for asset: " << asset << endl;
        cout << "----------------------------\n";
    }

    // Filter transactions by type
    void filter(const string &type) const
    {
        cout << "\n--- Transactions of Type: " << type << " ---\n";
        bool found = false;
        for (const auto &tran : transactions)
        {
            if (tran.type == type)
            {
                cout << "Asset: " << tran.asset << ", Amount: $" << tran.amount
                     << ", Date: " << tran.getDateString() << endl;
                found = true;
            }
        }
        if (!found)
            cout << "No transactions found of type: " << type << endl;
        cout << "----------------------------\n";
    }
};

class Watchlist {
public:

    void add_asset(const string &username, const string &asset_name, double initial_price) {
        ofstream file(username + "_watchlist.txt", ios::app);
        if (!file.is_open()) {
            cout << "Error opening watchlist file!\n";
            return;
        }

        file << asset_name << "," << initial_price << "," << initial_price << "\n";
        file.close();
        cout << "Added " << asset_name << " to the watchlist.\n";
    }

    void remove_asset(const string &username, const string &asset_name) {
        ifstream infile(username + "_watchlist.txt");
        ofstream tempFile("temp.txt");

        if (!infile.is_open() || !tempFile.is_open()) {
            cout << "Error opening files for removing asset!\n";
            return;
        }

        string line;
        bool asset_found = false;
        while (getline(infile, line)) {
            stringstream ss(line);
            string name;
            getline(ss, name, ',');
            if (name != asset_name) {
                tempFile << line << "\n";
            } else {
                asset_found = true;
            }
        }

        infile.close();
        tempFile.close();
        remove((username + "_watchlist.txt").c_str());
        rename("temp.txt", (username + "_watchlist.txt").c_str());

        if (asset_found) {
            cout << "Removed " << asset_name << " from the watchlist.\n";
        } else {
            cout << "Asset " << asset_name << " not found in the watchlist.\n";
        }
    }

    void track_performance(const string &username) const {
        ifstream file(username + "_watchlist.txt");
        if (!file.is_open()) {
            cout << "Error opening watchlist file for tracking performance!\n";
            return;
        }

        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            string name;
            double initial_price, current_price;
            getline(ss, name, ',');
            ss >> initial_price;
            ss.ignore(1, ',');
            ss >> current_price;

            double change = ((current_price - initial_price) / initial_price) * 100;
            cout << "Asset: " << name << ", Price Change: " << change << "%\n";
        }
        file.close();
    }

    void notify_significant_changes(const string &username, double threshold) const {
        ifstream file(username + "_watchlist.txt");
        if (!file.is_open()) {
            cout << "Error opening watchlist file for notifications!\n";
            return;
        }

        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            string name;
            double initial_price, current_price;
            getline(ss, name, ',');
            ss >> initial_price;
            ss.ignore(1, ',');
            ss >> current_price;

            double change = ((current_price - initial_price) / initial_price) * 100;
            if (abs(change) >= threshold) {
                cout << "Significant change in " << name << ": " << change << "%\n";
            }
        }
        file.close();
    }

    void update_price(const string &username, const string &asset_name, double new_price) {
        ifstream infile(username + "_watchlist.txt");
        ofstream tempFile("temp.txt");

        if (!infile.is_open() || !tempFile.is_open()) {
            cout << "Error opening files for updating price!\n";
            return;
        }

        string line;
        bool asset_found = false;
        while (getline(infile, line)) {
            stringstream ss(line);
            string name;
            double initial_price, current_price;
            getline(ss, name, ',');
            ss >> initial_price;
            ss.ignore(1, ',');
            ss >> current_price;

            if (name == asset_name) {
                current_price = new_price;
                asset_found = true;
            }
            tempFile << name << "," << initial_price << "," << current_price << "\n";
        }

        infile.close();
        tempFile.close();
        remove((username + "_watchlist.txt").c_str());
        rename("temp.txt", (username + "_watchlist.txt").c_str());

        if (asset_found) {
            cout << "Updated price of " << asset_name << " to " << new_price << ".\n";
        } else {
            cout << "Asset " << asset_name << " not found in the watchlist.\n";
        }
    }
};

int main()
{
    try
    {
        User userSystem;
        Watchlist watchlist;
        FileHandler filehandler;
        PortfolioAnalytics portfolioAnalytics;
        Watchlist myWatchlist;
        int choice;

        while (true)
        {
            cout << "\n|1. Register\n";
            cout << "|2. Login\n";
            cout << "|3. Exit\n";
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
                    filehandler.loadPortfolio(portfolio, currentUser);

                    int userChoice;
                    while (true)
                    {
                        cout << "\nPortfolio Management Options:\n";
                        cout << "|1. Add Entity\n";
                        cout << "|2. Show Portfolio\n";
                        cout << "|3. Buy Entity\n";
                        cout << "|4. Sell Entity\n";
                        cout << "|5. Save Portfolio\n";
                        cout << "|6. Get Total Value of Portfolio\n";
                        cout << "|7. Search for Entity\n";
                        cout << "|8. Generate Portfolio Report\n";
                        cout << "|9. Show Entity Distribution\n";
                        cout << "|10. Manage Watchlist\n";
                        cout << "|11. Logout\n";
                        cout << "Enter your choice: ";
                        cin >> userChoice;

                        try
                        {
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
                                filehandler.savePortfolio(portfolio, currentUser);
                            }

                            else if (userChoice == 6)
                            {
                                cout << "Total Portfolio Value: $" << portfolio.getTotalValue() << "\n";
                            }

                            else if (userChoice == 7)
                            {
                                string name;

                                cout << "Enter the name of the entity to search: ";
                                cin >> name;

                                FinancialEntity *entity = portfolio.searchEntity(name);

                                if (entity)
                                {
                                    entity->showDetails();
                                }
                            }

                            else if (userChoice == 8)
                            {
                                portfolioAnalytics.showReport(portfolio);
                            }

                            else if (userChoice == 9)
                            {
                                portfolioAnalytics.entityDistribution(portfolio); // Show distribution
                            }

                            else if (userChoice == 10) // Watchlist management
                            {
                                int watchlistChoice;
                                cout << "\nWatchlist Options:\n";
                                cout << "|1. Add Asset to Watchlist\n";
                                cout << "|2. Remove Asset from Watchlist\n";
                                cout << "|3. Update Asset Price in Watchlist\n";
                                cout << "|4. View Watchlist Performance\n";
                                cout << "|5. Notify Significant Changes in Watchlist\n";
                                cout << "Enter your choice: ";
                                cin >> watchlistChoice;

                                if (watchlistChoice == 1)
                                {
                                    string assetName;
                                    double initialPrice;
                                    cout << "Enter asset name: ";
                                    cin >> assetName;
                                    cout << "Enter initial price: ";
                                    cin >> initialPrice;
                                    myWatchlist.add_asset(currentUser,assetName,initialPrice);
                                }
                                else if (watchlistChoice == 2)
                                {
                                    string assetName;
                                    cout << "Enter asset name to remove: ";
                                    cin >> assetName;
                                    myWatchlist.remove_asset(currentUser,assetName);
                                }
                                else if (watchlistChoice == 3)
                                {
                                    string assetName;
                                    double newPrice;
                                    cout << "Enter asset name: ";
                                    cin >> assetName;
                                    cout << "Enter new price: ";
                                    cin >> newPrice;
                                    myWatchlist.update_price(currentUser,assetName,newPrice);
                                }
                                else if (watchlistChoice == 4)
                                {
                                    myWatchlist.track_performance(currentUser);
                                }
                                else if (watchlistChoice == 5)
                                {
                                    double threshold;
                                    cout << "Enter price change threshold (percentage): ";
                                    cin >> threshold;
                                    myWatchlist.notify_significant_changes(currentUser,threshold);
                                }
                                else{
                                    cout << "Invalid Input";
                                }
                            }

                            else if(userChoice == 11) // Logout
                            {
                                break; // Logout
                            }
                            else
                            {
                                cout << "Invalid choice! Please try again.\n";
                            }
                        }

                        catch (const exception &e)
                        {
                            cout << "An error occurred: " << e.what() << "\n";
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
    }

    catch (const exception &e)
    {
        cout << "An unexpected error occurred: " << e.what() << "\n";
    }

    return 0;
}
