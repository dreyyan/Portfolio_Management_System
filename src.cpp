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
int main() {
    fast;

    MutualFund myFund("Tech Growth Fund");

    int choice;
    do {
        cout << "\n--- Mutual Fund Management ---\n";
        cout << "1. Add Asset\n";
        cout << "2. Add Liability\n";
        cout << "3. Show Portfolio\n";
        cout << "4. Sort Portfolio by Value\n";
        cout << "5. Get Total Value of the Fund\n";
        cout << "6. Find Entity by Name\n";
        cout << "7. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: {
                string name;
                double value;
                cout << "Enter the Asset Name: ";
                cin.ignore();
                getline(cin, name);
                cout << "Enter the Asset Value: $";
                cin >> value;
                myFund.addEntity(make_unique<Asset>(name, value));
                cout << "Asset added successfully!\n";
                break;
            }
            case 2: {
                string name;
                double value;
                cout << "Enter the Liability Name: ";
                cin.ignore();
                getline(cin, name);
                cout << "Enter the Liability Value: $";
                cin >> value;
                myFund.addEntity(make_unique<Liability>(name, value));
                cout << "Liability added successfully!\n";
                break;
            }
            case 3: {
                cout << "Showing Portfolio:\n";
                myFund.showPortfolio();
                break;
            }
            case 4: {
                myFund.sortPortfolioByValue();
                cout << "Portfolio sorted by value successfully!\n";
                break;
            }
            case 5: {
                double totalValue = myFund.getTotalValue();
                cout << "Total Value of the Mutual Fund: $" << totalValue << "\n";
                break;
            }
            case 6: {
                string name;
                cout << "Enter the Entity Name to find: ";
                cin.ignore();
                getline(cin, name);
                FinancialEntity* foundEntity = myFund.findEntityByName(name);
                if (foundEntity) {
                    cout << "Entity found:\n";
                    foundEntity->showDetails();
                } else {
                    cout << "Entity '" << name << "' not found in the portfolio.\n";
                }
                break;
            }
            case 7: {
                cout << "Exiting...\n";
                break;
            }
            default: {
                cout << "Invalid choice! Please try again.\n";
            }
        }
    } while (choice != 7);

    return 0;
}
