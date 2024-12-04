#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip> // For better formatting

using namespace std;

// Product structure
struct Product {
    int id;
    string name;
    double price;

    // Display product details
    void display() const {
        cout << setw(10) << left << id
             << setw(30) << left << name
             << setw(10) << right << fixed << setprecision(2) << price << "\n";
    }
};

// Function prototypes
void createProduct();
void readProducts();
void updateProduct();
void deleteProduct();
void searchProduct();
vector<Product> loadProducts();
void saveProducts(const vector<Product>& products);

const string filename = "products.txt";

int main() {
    int choice;

    do {
        cout << "\n--- Product Management System ---\n";
        cout << "1. Create Product\n";
        cout << "2. View All Products\n";
        cout << "3. Update Product\n";
        cout << "4. Delete Product\n";
        cout << "5. Search Product by ID\n";
        cout << "6. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: createProduct(); break;
            case 2: readProducts(); break;
            case 3: updateProduct(); break;
            case 4: deleteProduct(); break;
            case 5: searchProduct(); break;
            case 6: cout << "Exiting... Thank you!\n"; break;
            default: cout << "Invalid choice! Please enter a number between 1 and 6.\n";
        }
    } while (choice != 6);

    return 0;
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
    cout << "Enter Product Price: ";
    while (!(cin >> p.price) || p.price < 0) {
        cin.clear(); // Clear error flag
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignore invalid input
        cout << "Invalid price. Please enter a positive number: ";
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
             << setw(30) << left << "Name"
             << setw(10) << right << "Price\n";
        cout << string(50, '-') << "\n";
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
            cout << "Enter new Product Price: ";
            while (!(cin >> p.price) || p.price < 0) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid price. Please enter a positive number: ";
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

// Load products from the file
vector<Product> loadProducts() {
    vector<Product> products;
    ifstream file(filename);

    if (file.is_open()) {
        Product p;
        while (file >> p.id) {
            file.ignore(); // Ignore newline after ID
            getline(file, p.name);
            file >> p.price;
            file.ignore(); // Ignore newline after price
            products.push_back(p);
        }
        file.close();
    } else {
        cout << "Error: Could not open file '" << filename << "'.\n";
    }

    return products;
}

// Save products to the file
void saveProducts(const vector<Product>& products) {
    ofstream file(filename, ios::trunc); // Overwrite the file

    if (file.is_open()) {
        for (const auto& p : products) {
            file << p.id << "\n" << p.name << "\n" << p.price << "\n";
        }
        file.close();
    } else {
        cout << "Error: Could not save to file '" << filename << "'.\n";
    }
}
