# Portfolio Management System

The **Portfolio Management System** is a simple C++ application that allows users to manage their financial portfolios, including assets and liabilities. Each user can register, log in, and manage their mutual funds, which contain various financial entities such as assets and liabilities. The system provides functionality to view and sort these financial entities.

## Features

- **User Registration and Login**: Users can register with a unique username and password. Each user is assigned a unique ID to avoid conflicts with identical usernames.
- **Mutual Fund Management**: Users can add, view, and manage their mutual funds, which consist of various financial entities (assets and liabilities).
- **Financial Entity Sorting**: Sort the portfolio by the value of assets or liabilities.
- **Portfolio Overview**: View the total value and details of all financial entities in a user's portfolio.

## How It Works

1. **Register a User**: 
   - Users can register with a unique username and password. Each registered user is given a unique user ID.

2. **Login to the System**:
   - Users must log in with their username and password. After login, users can manage their financial portfolios.

3. **Add Mutual Funds**:
   - Once logged in, users can add mutual funds to their portfolio, which can consist of assets and liabilities. Users can enter the name and value of each financial entity.

4. **View Portfolio**:
   - Users can view their entire portfolio, including all mutual funds and their associated assets and liabilities.

5. **Logout**:
   - Users can log out to end their session.

## Getting Started

### Prerequisites

To compile and run the project, you will need:

- A C++ compiler such as g++ or clang.
- Basic knowledge of C++ Standard Template Library (STL).

### Compilation

Use the following command to compile the program:

```bash
g++ -o portfolio_management main.cpp
