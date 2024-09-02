# Protfolio Management System
<h2>OOPS Project</h2>
<h2>Overview</h2>
The Portfolio Management System is a C++ application designed to manage and monitor the performance of various financial entities, including assets and liabilities, within a mutual fund. The system allows users to add, view, sort, and search for financial entities, making it easier to track the overall value and composition of a mutual fund.

<h3>Features</h3>
<strong>Asset and Liability Management:</strong> Add and manage different financial entities (Assets and Liabilities) within a mutual fund.<br>
<strong>Portfolio Display</strong>: View the details of all assets and liabilities in the mutual fund.<br>
<strong>Total Value Calculation</strong>: Calculate and display the total value of the mutual fund.<br>
<strong>Sorting</strong>: Sort the portfolio based on the current value of the financial entities.<br>
<strong>Entity Search</strong>: Search for specific financial entities by name within the portfolio.<br>

<h2>Code Structure</h2>
<h3>FinancialEntity (Base Class)</h3>

Represents a general financial entity with attributes like name and current value.
Contains pure virtual functions for displaying details (showDetails) and getting the current value.
<h3>Asset (Derived Class):</h3>

Represents an asset in the portfolio.
Overrides showDetails to display asset-specific information.
<h3>Liability (Derived Class):</h3>

Represents a liability in the portfolio.
Overrides showDetails to display liability-specific information.
<h3>MutualFund (Class):</h3>

Manages a collection of financial entities (assets and liabilities).
Provides functionalities to add entities, display the portfolio, calculate total value, sort entities by value, and search for entities by name.
