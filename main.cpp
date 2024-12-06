#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>
#include <limits>
#include <algorithm>
#include <map>
#include <sstream>

using namespace std;

// Product structure
struct Product {
    int id;
    string name;
    string category;
    double price;
    int quantity;

    // Display product details
    void display() const {
        cout << setw(10) << left << id
             << setw(20) << left << name
             << setw(15) << left << category
             << setw(10) << right << fixed << setprecision(2) << price
             << setw(10) << right << quantity << "\n";
    }
};

// Function prototypes
void createProduct();
void readProducts();
void updateProduct();
void deleteProduct();
void searchProduct();
void sortProducts();
void viewLowStock();
void generateReports();
void exportToCSV();
void importFromCSV();
void authenticateUser();
vector<Product> loadProducts();
void saveProducts(const vector<Product>& products);

const string filename = "products.txt";
const string usersFile = "users.txt";
string loggedInUser = "";
bool isAdmin = false;

int main() {
    authenticateUser();  // Log in before accessing the system

    int choice;

    do {
        cout << "\n--- Product Management System ---\n";
        cout << "1. Create Product\n";
        cout << "2. View All Products\n";
        cout << "3. Update Product\n";
        cout << "4. Delete Product\n";
        cout << "5. Search Product by ID\n";
        cout << "6. Sort Products\n";
        cout << "7. View Low Stock Products\n";
        cout << "8. Generate Report\n";
        cout << "9. Export Products to CSV\n";
        cout << "10. Import Products from CSV\n";
        cout << "11. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: createProduct(); break;
            case 2: readProducts(); break;
            case 3: updateProduct(); break;
            case 4: deleteProduct(); break;
            case 5: searchProduct(); break;
            case 6: sortProducts(); break;
            case 7: viewLowStock(); break;
            case 8: generateReports(); break;
            case 9: exportToCSV(); break;
            case 10: importFromCSV(); break;
            case 11: cout << "Exiting... Thank you!\n"; break;
            default: cout << "Invalid choice! Please enter a number between 1 and 11.\n";
        }
    } while (choice != 11);

    return 0;
}

// Simple authentication system
void authenticateUser() {
    string username, password;
    cout << "Enter username: ";
    cin >> username;
    cout << "Enter password: ";
    cin >> password;

    ifstream file(usersFile);
    string storedUsername, storedPassword;
    bool authenticated = false;

    while (file >> storedUsername >> storedPassword) {
        if (storedUsername == username && storedPassword == password) {
            loggedInUser = username;
            isAdmin = (username == "admin"); // Admin check based on username
            authenticated = true;
            break;
        }
    }

    if (!authenticated) {
        cout << "Invalid credentials. Exiting...\n";
        exit(0);  // Exit if authentication fails
    }

    cout << "Welcome, " << (isAdmin ? "Admin" : "User") << "!\n";
}

// Create a new product
void createProduct() {
    vector<Product> products = loadProducts();
    Product p;

    cout << "Enter Product ID: ";
    cin >> p.id;

    // Check for duplicate ID
    for (const auto& prod : products) {
        if (prod.id == p.id) {
            cout << "Error: A product with this ID already exists.\n";
            return;
        }
    }

    cin.ignore(); // Clear the input buffer
    cout << "Enter Product Name: ";
    getline(cin, p.name);
    cout << "Enter Product Category: ";
    getline(cin, p.category);
    cout << "Enter Product Price: ";
    while (!(cin >> p.price) || p.price < 0) {
        cin.clear(); // Clear error flag
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignore invalid input
        cout << "Invalid price. Please enter a positive number: ";
    }
    cout << "Enter Product Quantity: ";
    while (!(cin >> p.quantity) || p.quantity < 0) {
        cin.clear(); // Clear error flag
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignore invalid input
        cout << "Invalid quantity. Please enter a positive number: ";
    }

    products.push_back(p);
    saveProducts(products);
    cout << "Product added successfully!\n";
}

// Read all products
void readProducts() {
    vector<Product> products = loadProducts();

    if (products.empty()) {
        cout << "No products found.\n";
    } else {
        cout << "\n--- Product List ---\n";
        cout << setw(10) << left << "ID"
             << setw(20) << left << "Name"
             << setw(15) << left << "Category"
             << setw(10) << right << "Price"
             << setw(10) << right << "Quantity\n";
        cout << string(65, '-') << "\n";
        for (const auto& p : products) {
            p.display();
        }
    }
}

// Update an existing product
void updateProduct() {
    vector<Product> products = loadProducts();
    int id;
    bool found = false;

    cout << "Enter Product ID to update: ";
    cin >> id;

    for (auto& p : products) {
        if (p.id == id) {
            found = true;
            cin.ignore(); // Clear the input buffer
            cout << "Enter new Product Name: ";
            getline(cin, p.name);
            cout << "Enter new Product Category: ";
            getline(cin, p.category);
            cout << "Enter new Product Price: ";
            while (!(cin >> p.price) || p.price < 0) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid price. Please enter a positive number: ";
            }
            cout << "Enter new Product Quantity: ";
            while (!(cin >> p.quantity) || p.quantity < 0) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid quantity. Please enter a positive number: ";
            }

            saveProducts(products);
            cout << "Product updated successfully!\n";
            break;
        }
    }

    if (!found) {
        cout << "Product with ID " << id << " not found.\n";
    }
}

// Delete a product
void deleteProduct() {
    vector<Product> products = loadProducts();
    int id;
    bool found = false;

    cout << "Enter Product ID to delete: ";
    cin >> id;

    for (auto it = products.begin(); it != products.end(); ++it) {
        if (it->id == id) {
            found = true;
            products.erase(it);
            saveProducts(products);
            cout << "Product deleted successfully!\n";
            break;
        }
    }

    if (!found) {
        cout << "Product with ID " << id << " not found.\n";
    }
}

// Search for a product by ID
void searchProduct() {
    vector<Product> products = loadProducts();
    int id;
    bool found = false;

    cout << "Enter Product ID to search: ";
    cin >> id;

    for (const auto& p : products) {
        if (p.id == id) {
            found = true;
            cout << "\n--- Product Details ---\n";
            p.display();
            break;
        }
    }

    if (!found) {
        cout << "Product with ID " << id << " not found.\n";
    }
}

// Sort products by ID, Name, or Price
void sortProducts() {
    vector<Product> products = loadProducts();
    int choice;

    cout << "Sort by:\n1. ID\n2. Name\n3. Price\nEnter your choice: ";
    cin >> choice;

    switch (choice) {
        case 1: sort(products.begin(), products.end(), [](const Product& a, const Product& b) { return a.id < b.id; }); break;
        case 2: sort(products.begin(), products.end(), [](const Product& a, const Product& b) { return a.name < b.name; }); break;
        case 3: sort(products.begin(), products.end(), [](const Product& a, const Product& b) { return a.price < b.price; }); break;
        default: cout << "Invalid choice!\n"; return;
    }

    saveProducts(products);
    cout << "Products sorted successfully!\n";
}

// View low-stock products
void viewLowStock() {
    vector<Product> products = loadProducts();
    int threshold;
    cout << "Enter low-stock threshold: ";
    cin >> threshold;

    cout << "\n--- Low Stock Products ---\n";
    bool found = false;
    for (const auto& p : products) {
        if (p.quantity <= threshold) {
            found = true;
            p.display();
        }
    }

    if (!found) {
        cout << "No products with low stock.\n";
    }
}

// Generate inventory report
void generateReports() {
    vector<Product> products = loadProducts();
    double totalValue = 0;
    map<string, int> categoryCounts;

    for (const auto& p : products) {
        totalValue += p.price * p.quantity;
        categoryCounts[p.category]++;
    }

    cout << "\n--- Inventory Report ---\n";
    cout << "Total Inventory Value: $" << fixed << setprecision(2) << totalValue << "\n";
    cout << "Product Count by Category:\n";
    for (const auto& [category, count] : categoryCounts) {
        cout << category << ": " << count << "\n";
    }
}

// Export products to CSV
void exportToCSV() {
    vector<Product> products = loadProducts();
    ofstream file("products.csv");

    if (file.is_open()) {
        file << "ID,Name,Category,Price,Quantity\n";
        for (const auto& p : products) {
            file << p.id << "," << p.name << "," << p.category << "," << p.price << "," << p.quantity << "\n";
        }
        file.close();
        cout << "Products exported to 'products.csv' successfully!\n";
    } else {
        cout << "Error: Could not export to CSV.\n";
    }
}

// Import products from CSV
void importFromCSV() {
    vector<Product> products = loadProducts();
    ifstream file("products.csv");

    if (file.is_open()) {
        string line, word;
        getline(file, line); // Skip header line

        while (getline(file, line)) {
            stringstream ss(line);
            Product p;

            getline(ss, word, ','); p.id = stoi(word);
            getline(ss, p.name, ',');
            getline(ss, p.category, ',');
            getline(ss, word, ','); p.price = stod(word);
            getline(ss, word, ','); p.quantity = stoi(word);

            products.push_back(p);
        }

        file.close();
        saveProducts(products);
        cout << "Products imported successfully!\n";
    } else {
        cout << "Error: Could not open 'products.csv'.\n";
    }
}

// Load products from file
vector<Product> loadProducts() {
    vector<Product> products;
    ifstream file(filename);
    if (file.is_open()) {
        Product p;
        while (file >> p.id) {
            file.ignore();
            getline(file, p.name);
            getline(file, p.category);
            file >> p.price >> p.quantity;
            file.ignore();
            products.push_back(p);
        }
        file.close();
    }
    return products;
}

// // Save products to file
// void saveProducts(const vector<Product>& products) {
//     ofstream file(filename);
//     if (file.is_open()) {
//         for (const auto& p : products) {
//             file << p.id << "\n" << p.name << "\n" << p.category << "\n" << p.price << "\n" << p.quantity << "\n";
//         }
//         file.close();
//     } else {
//         cout << "Error: Could not save products.\n";
//     }
// }
