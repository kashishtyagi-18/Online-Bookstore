#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <iomanip>
#include <fstream>
#include <sstream>

class Book {
public:
    std::string title;
    std::string author;
    double price;
    int stock;

    Book(std::string t, std::string a, double p, int s)
        : title(t), author(a), price(p), stock(s) {}
};

class ShoppingCart {
public:
    std::vector<Book> cart;

    void addBook(const Book& book, int quantity) {
        for (int i = 0; i < quantity; ++i) {
            cart.push_back(book);
        }
    }

    double totalCost() const {
        double total = 0.0;
        for (const auto& book : cart) {
            total += book.price;
        }
        return total;
    }

    void displayCart() const {
        std::cout << "Shopping Cart:\n";
        for (const auto& book : cart) {
            std::cout << "Title: " << book.title << ", Author: " << book.author << ", Price: $" << book.price << "\n";
        }
    }
};

class Customer {
public:
    std::string id;
    std::string name;
    ShoppingCart cart;

    Customer() {}
    Customer(std::string i, std::string n)
        : id(i), name(n) {}
};

class Store {
private:
    std::vector<Book> inventory;
    std::vector<Book> initialInventory; // Store initial inventory
    std::unordered_map<std::string, Customer> customers;

public:
    Store() {
        // Add initial inventory
        addInitialBooks();
        // Store initial inventory state
        initialInventory = inventory;
    }

    void addInitialBooks() {
        addBookToInventory(Book("Engineering Mechanics", "R.S Khurmi", 345, 5));
        addBookToInventory(Book("1984", "George Orwell", 520, 3));
        addBookToInventory(Book("Applied Mathematics", "ML Aggarwal", 627, 4));
        addBookToInventory(Book("Applied Physics", "Rajni Bahuguna", 850, 4));
        addBookToInventory(Book("Signals and System", "Shaila Dinkar", 390, 4));
        addBookToInventory(Book("Analog Communication", "Ashish Bagwari", 720, 4));
        addBookToInventory(Book("Digital Communication", "John R. Barry", 640, 4));
        addBookToInventory(Book("Probability Statistics", "S.Chand", 945, 4));
        addBookToInventory(Book("Computational Methods", "Debashish Lohar", 710, 4));
        addBookToInventory(Book("Digital Logic & Design", "M. Morris Mano", 440, 4));
    }

    void addBookToInventory(const Book& book) {
        inventory.push_back(book);
    }

    void displayInventory() const {
        std::cout << "+------------------------------------------------------------------+\n";
        std::cout << "| Title                    | Author              | Price     | Stock  |\n";
        std::cout << "+------------------------------------------------------------------+\n";
        for (const auto& book : inventory) {
            std::cout << "| " << std::setw(25) << std::left << book.title
                      << "| " << std::setw(20) << std::left << book.author
                      << "| Rs." << std::setw(7) << std::left << book.price
                      << "| " << std::setw(5) << std::left << book.stock << "|\n";
        }
        std::cout << "+------------------------------------------------------------------+\n";
    }

    void addCustomer(const Customer& customer) {
        customers[customer.id] = customer;
    }
    Customer* findCustomer(const std::string& id) {
        auto it = customers.find(id);
        if (it != customers.end()) {
            return &it->second;
        } else {
            return nullptr;
        }
    }

    Book* recursiveSearch(int index, const std::string& title) {
        if (index >= inventory.size()) return nullptr;
        if (inventory[index].title == title) {
            return &inventory[index];
        }
        return recursiveSearch(index + 1, title);
    }
    std::vector<Book*> searchBooksByFirstChar(char firstChar) {
        std::vector<Book*> result;
        for (auto& book : inventory) {
            if (book.title[0] == firstChar) {
                result.push_back(&book);
            }
        }
        return result;
    }
    void purchaseBooks(Customer& customer) const {
        std::cout << "\nFinal Bill for " << customer.name << " (ID: " << customer.id << "):\n";
        std::cout << "+--------------------------------------------+\n";
        std::cout << "| Title                    | Price            |\n";
        std::cout << "+--------------------------------------------+\n";
        for (const auto& book : customer.cart.cart) {
            std::cout << "| " << std::setw(25) << std::left << book.title
                      << "| Rs." << std::setw(13) << std::left << book.price << "|\n";
        }
        std::cout << "+--------------------------------------------+\n";
        std::cout << "| Total                  | Rs." << std::setw(13) << std::left << customer.cart.totalCost() << "|\n";
        std::cout << "+--------------------------------------------+\n";

        std::ofstream outFile("customers_data.txt", std::ios::app);
        outFile << customer.id << "," << customer.name << "," << customer.cart.totalCost() << "\n";
        outFile.close();
    }

    void loadCustomerData() {
        std::ifstream inFile("customers_data.txt");
        if (inFile.is_open()) {
            std::string line;
            while (std::getline(inFile, line)) {
                std::string id, name;
                double totalCost;
                std::istringstream iss(line);
                std::getline(iss, id, ',');
                std::getline(iss, name, ',');
                iss >> totalCost;
                customers[id] = Customer(id, name);
                // Load other data as needed
            }
            inFile.close();
        }
    }

    void clearCustomerData() {
        customers.clear();
        std::ofstream outFile("customers_data.txt", std::ios::trunc);
        outFile.close();
        std::cout << "All customer data cleared.\n";
        // Reset inventory to initial state
        inventory = initialInventory;
        std::cout << "Inventory reset to initial state.\n";
    }

    void viewPreviousPurchases(const std::string& customerId) const {
        std::ifstream inFile("customers_data.txt");
        if (inFile.is_open()) {
            std::string line;
            while (std::getline(inFile, line)) {
                std::string id, name;
                double totalCost;
                std::istringstream iss(line);
                std::getline(iss, id, ',');
                std::getline(iss, name, ',');
                iss >> totalCost;
                if (id == customerId) {
                    std::cout << "Previous purchases for " << name << " (ID: " << id << "): Total Cost: Rs." << totalCost << "\n";
                    break;
                }
            }
            inFile.close();
        }
    }
};

int main() {
    Store store;
    store.loadCustomerData();

    std::string customerId, customerName;
    char customerType;

    std::cout << "Enter Customer ID: ";
    std::cin >> customerId;
    std::cin.ignore();  // Ignore leftover newline character

    Customer* existingCustomer = store.findCustomer(customerId);
    if (existingCustomer) {
        std::cout << "Welcome back, " << existingCustomer->name << "!\n";
        existingCustomer->cart.displayCart();
        store.viewPreviousPurchases(customerId);
    } else {
        std::cout << "New customer. Enter your name: ";
        std::getline(std::cin, customerName);
        Customer newCustomer(customerId, customerName);
        store.addCustomer(newCustomer);
        existingCustomer = store.findCustomer(customerId);
    }
    int choice;
    do {
        std::cout << "\n1. View Inventory\n2. Add Book to Cart\n3. Purchase\n4. Search Book\n5. Clear Customer Data\n6. Add New Book to Inventory\n7. Exit\nEnter your choice: ";
        std::cin >> choice;
        std::cin.ignore();  // Ignore leftover newline character

        switch (choice) {
            case 1:
                store.displayInventory();
                break;
            case 2: {
                std::string title;
                std::cout << "Enter book title to add to cart: ";
                std::getline(std::cin, title);
                Book* foundBook = store.recursiveSearch(0, title);
                if (foundBook) {
                    if (foundBook->stock > 0) {
                        int quantity;
                        std::cout << "Enter quantity to purchase: ";
                        std::cin >> quantity;
                        std::cin.ignore();  // Ignore leftover newline character
                        if (quantity <= foundBook->stock) {
                            existingCustomer->cart.addBook(*foundBook, quantity);
                            foundBook->stock -= quantity;
                            std::cout << "Book(s) added to cart.\n";
                        } else {
                            std::cout << "Not enough stock available.\n";
                        }
                    } else {
                        std::cout << "Book is out of stock.\n";
                    }
                } else {
                    std::cout << "Book not found.\n";
                }
                break;
            }
            case 3:
                store.purchaseBooks(*existingCustomer);
                break;
            case 4: {
                char firstChar;
                std::cout << "Enter the first letter or number of the book title to search: ";
                std::cin >> firstChar;
                std::cin.ignore();  // Ignore leftover newline character
                std::vector<Book*> foundBooks = store.searchBooksByFirstChar(firstChar);
                if (!foundBooks.empty()) {
                    for (auto* book : foundBooks) {
                        std::cout << "Found: " << book->title << " by " << book->author << " - Rs." << book->price << "\n";
                    }
                } else {
                    std::cout << "No books found starting with " << firstChar << ".\n";
                }
                break;
            }
            case 5:
                store.clearCustomerData();
                break;
            //case 6:
                //store.viewPreviousPurchases(customerId);
                //break;
            case 6: {
                std::string title, author;
                double price;
                int stock;
                std::cout << "Enter book title: ";
                std::getline(std::cin, title);
                std::cout << "Enter book author: ";
                std::getline(std::cin, author);
                std::cout << "Enter book price: ";
                std::cin >> price;
                std::cout << "Enter book stock: ";
                std::cin >> stock;
                std::cin.ignore();  // Ignore leftover newline character
                store.addBookToInventory(Book(title, author, price, stock));
                std::cout << "Book added to inventory.\n";
                break;
            }
        }
    } while (choice != 7);

    return 0;
}
