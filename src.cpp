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
