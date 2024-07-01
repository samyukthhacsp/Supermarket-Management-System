#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip>
#include <ctime>
#include <sstream>

using namespace std;

class Product {
public:
    int product_id;
    string name;
    float price;
    int quantity;
    string category;

    Product(int id, string n, float p, int q, string c) : product_id(id), name(n), price(p), quantity(q), category(c) {}

    void display() const {
        cout << left << setw(10) << product_id << setw(20) << name << setw(10) << price << setw(10) << quantity << setw(15) << category << endl;
    }
};

class Customer {
public:
    int customer_id;
    string name;
    string address;
    string phone_number;
    bool has_membership;
    int credit_points;

    Customer(int id, string n, string addr, string phone, bool member, int points) : customer_id(id), name(n), address(addr), phone_number(phone), has_membership(member), credit_points(points) {}

    void display() const {
        cout << left << setw(10) << customer_id << setw(20) << name << setw(20) << address << setw(15) << phone_number << setw(10) << has_membership << setw(10) << credit_points << endl;
    }
};

class Sale {
public:
    int sale_id;
    int product_id;
    int customer_id;
    int quantity;
    string datetime;

    Sale(int sid, int pid, int cid, int q, string dt) : sale_id(sid), product_id(pid), customer_id(cid), quantity(q), datetime(dt) {}

    void display() const {
        cout << left << setw(10) << sale_id << setw(10) << product_id << setw(10) << customer_id << setw(10) << quantity << setw(20) << datetime << endl;
    }
};

class Supermarket {
private:
    vector<Product> products;
    vector<Customer> customers;
    vector<Sale> sales;
    int next_product_id = 1;
    int next_customer_id = 1;
    int next_sale_id = 1;
    vector<string> categories = {"Dairy", "Toiletry", "Vegetable", "Fruit", "Meat", "Beverage", "Snack", "Household"};

    void loadProducts() {
        ifstream file("products.txt");
        if (file.is_open()) {
            int id, quantity;
            float price;
            string name, category;
            while (file >> id >> ws) {
                getline(file, name, ',');
                file >> price >> quantity;
                getline(file, category);
                products.emplace_back(id, name, price, quantity, category);
                next_product_id = max(next_product_id, id + 1);
            }
            file.close();
        }
    }

    void loadCustomers() {
        ifstream file("customers.txt");
        if (file.is_open()) {
            int id;
            string name, address, phone;
            bool member;
            int points;
            while (file >> id >> ws) {
                getline(file, name, ',');
                getline(file, address, ',');
                getline(file, phone, ',');
                file >> member >> points;
                customers.emplace_back(id, name, address, phone, member, points);
                next_customer_id = max(next_customer_id, id + 1);
            }
            file.close();
        }
    }

    void loadSales() {
        ifstream file("sales.txt");
        if (file.is_open()) {
            int sid, pid, cid, quantity;
            string datetime;
            while (file >> sid >> pid >> cid >> quantity >> ws) {
                getline(file, datetime);
                sales.emplace_back(sid, pid, cid, quantity, datetime);
                next_sale_id = max(next_sale_id, sid + 1);
            }
            file.close();
        }
    }

    void saveProducts() const {
        ofstream file("products.txt");
        if (file.is_open()) {
            file << "ID\tName\tPrice\tQuantity\tCategory\n";
            for (const auto& p : products) {
                file << p.product_id << "\t" << p.name << "\t" << p.price << "\t" << p.quantity << "\t" << p.category << "\n";
            }
            file.close();
        } else {
            cout << "Unable to open products.txt for saving." << endl;
        }
    }

    void saveCustomers() const {
        ofstream file("customers.txt");
        if (file.is_open()) {
            file << "ID\tName\tAddress\tPhone Number\tHas Membership\tCredit Points\n";
            for (const auto& c : customers) {
                file << c.customer_id << "\t" << c.name << "\t" << c.address << "\t" << c.phone_number << "\t" << c.has_membership << "\t" << c.credit_points << "\n";
            }
            file.close();
        } else {
            cout << "Unable to open customers.txt for saving." << endl;
        }
    }


     void saveSales() const {
        ofstream file("sales.txt");
        if (file.is_open()) {
            file << "ID\tProduct ID\tCustomer ID\tQuantity\tDate Time\n";
            for (const auto& s : sales) {
                file << s.sale_id << "\t" << s.product_id << "\t" << s.customer_id << "\t" << s.quantity << "\t" << s.datetime << "\n";
            }
            file.close();
        } else {
            cout << "Unable to open sales.txt for saving." << endl;
        }
    }

public:
    Supermarket() {
        loadProducts();
        loadCustomers();
        loadSales();
    }

    ~Supermarket() {
        saveProducts();
        saveCustomers();
        saveSales();
    }

    void addProduct() {
        string name, category;
        float price;
        int quantity;
        cout << "Enter product name: ";
        cin.ignore();
        getline(cin, name);
        cout << "Enter product price: ";
        cin >> price;
        cout << "Enter product quantity: ";
        cin >> quantity;
        cout << "Enter product category (Choose a number):" << endl;
        for (size_t i = 0; i < categories.size(); ++i) {
            cout << i + 1 << ": " << categories[i] << endl;
        }
        int category_choice;
        cin >> category_choice;
        if (category_choice < 1 || category_choice > categories.size()) {
            cout << "Invalid category choice." << endl;
            return;
        }
        category = categories[category_choice - 1];
        products.emplace_back(next_product_id++, name, price, quantity, category);
        saveProducts();
    }



    void displayProducts() const {
        cout << left << setw(10) << "ID" << setw(20) << "Name" << setw(10) << "Price" << setw(10) << "Quantity" << setw(15) << "Category" << endl;
        for (const auto& p : products) {
            p.display();
        }
    }

    void deleteProduct() {
    int id;
    cout << "Enter product ID to delete: ";
    cin >> id;

    auto it = find_if(products.begin(), products.end(), [id](const Product& p) { return p.product_id == id; });
    if (it != products.end()) {
        products.erase(it);
        cout << "Product with ID " << id << " deleted successfully." << endl;
    } else {
        cout << "Product not found." << endl;
    }
    saveProducts();
}

    void modifyProduct() {
    int id;
    cout << "Enter product ID to modify: ";
    cin >> id;

    auto it = find_if(products.begin(), products.end(), [id](const Product& p) { return p.product_id == id; });
    if (it != products.end()) {
        cout << "Enter new product name: ";
        cin.ignore();
        getline(cin, it->name);
        cout << "Enter new product price: ";
        cin >> it->price;
        cout << "Enter new product quantity: ";
        cin >> it->quantity;
        cout << "Enter new product category (Choose a number):" << endl;
        for (size_t i = 0; i < categories.size(); ++i) {
            cout << i + 1 << ": " << categories[i] << endl;
        }
        int category_choice;
        cin >> category_choice;
        if (category_choice < 1 || category_choice > categories.size()) {
            cout << "Invalid category choice." << endl;
            return;
        }
        it->category = categories[category_choice - 1];
        cout << "Product details updated successfully." << endl;
    } else {
        cout << "Product not found." << endl;
    }

    saveProducts();
}
    void searchProductByName(const vector<Product>& products) {
    string name;
    cout << "Enter product name to search: ";
    cin.ignore();
    getline(cin, name);

    auto it = find_if(products.begin(), products.end(), [&name](const Product& p) {
        return p.name == name;
    });

    if (it != products.end()) {
        cout << left << setw(10) << "ID" << setw(20) << "Name" << setw(10) << "Price" << setw(10) << "Quantity" << setw(15) << "Category" << endl;
        it->display();
    } else {
        cout << "Product not found." << endl;
    }
}



    void addCustomer() {
        string name, address, phone;
        bool has_membership;
        int credit_points = 0; // Initialize credit points to 0 for new customers
        cout << "Enter customer name: ";
        cin.ignore();
        getline(cin, name);
        cout << "Enter customer address: ";
        getline(cin, address);
        cout << "Enter customer phone number: ";
        while (true) {
            getline(cin, phone);
            if (phone.length() == 10 && all_of(phone.begin(), phone.end(), ::isdigit)) {
                break;
            } else {
                cout << "Invalid phone number. Please enter a 10-digit phone number: ";
            }
        }
       
        char membership_choice='y';
        if (membership_choice == 'y' || membership_choice == 'Y') {
            cout << "Membership card created. Credit points initialized to 0." << endl;
            has_membership = true;
        } else {
            cout << "No membership card created." << endl;
            has_membership = false;
        }
        customers.emplace_back(next_customer_id++, name, address, phone, has_membership, credit_points);
        saveCustomers();
    }

    void recordSale(int pid, int cid, int quantity) {
        // Get current date and time
        string datetime = getCurrentDateTime();
        sales.emplace_back(next_sale_id++, pid, cid, quantity, datetime);
        saveSales();

    }

    void deleteCustomer() {
    int id;
    cout << "Enter customer ID to delete: ";
    cin >> id;

    auto it = find_if(customers.begin(), customers.end(), [id](const Customer& c) { return c.customer_id == id; });
    if (it != customers.end()) {
        customers.erase(it);
        cout << "Customer with ID " << id << " deleted successfully." << endl;
    } else {
        cout << "Customer not found." << endl;
    }

    saveCustomers();
}
    void modifyCustomer() {
    int id;
    cout << "Enter customer ID to modify: ";
    cin >> id;

    auto it = find_if(customers.begin(), customers.end(), [id](const Customer& c) { return c.customer_id == id; });
    if (it != customers.end()) {
        cout << "Enter new customer name: ";
        cin.ignore();
        getline(cin, it->name);
        cout << "Enter new customer address: ";
        getline(cin, it->address);
        cout << "Enter new customer phone number: ";
        getline(cin, it->phone_number);
        cout << "Customer details updated successfully." << endl;
    } else {
        cout << "Customer not found." << endl;
    }

    saveCustomers();
}
    void searchCustomerByName(const vector<Customer>& customers) {
    string name;
    cout << "Enter customer name to search: ";
    cin.ignore();
    getline(cin, name);

    auto it = find_if(customers.begin(), customers.end(), [&name](const Customer& c) {
        return c.name == name;
    });

    if (it != customers.end()) {
        cout << left << setw(10) << "ID" << setw(20) << "Name" << setw(20) << "Address" << setw(15) << "Phone" << setw(10) << "Member" << setw(10) << "Points" << endl;
        it->display();
    } else {
        cout << "Customer not found." << endl;
    }
}





    void createBill() {

        string name, address;
    cout << "Enter customer name: ";
    cin.ignore();
    getline(cin, name);
    cout << "Enter customer address: ";
    getline(cin, address);

    // Check if the customer exists
    auto customer_it = find_if(customers.begin(), customers.end(), [name](const Customer& c) { return c.name == name; });
    if (customer_it == customers.end() || customer_it->address != address) {
        cout << "Customer not found or address does not match. Adding new customer." << endl;
        addCustomer();
        customer_it = customers.end() - 1;
    }

    float subtotal = 0.0;
    char add_more_products;
    vector<pair<Product, int>> selected_products; // Stores selected products along with their quantities
    do {
        // Print product categories
        cout << "\nProduct Categories:" << endl;
        for (size_t i = 0; i < categories.size(); ++i) {
            cout << i + 1 << ": " << categories[i] << endl;
        }
        int category_choice;
        cout << "Choose a category: ";
        cin >> category_choice;
        if (category_choice < 1 || category_choice > categories.size()) {
            cout << "Invalid category choice." << endl;
            return;
        }

        // Display products by selected category
        cout << "\nProducts in category '" << categories[category_choice - 1] << "':" << endl;
        cout << left << setw(10) << "ID" << setw(20) << "Name" << setw(10) << "Price" << setw(10) << "Quantity" << setw(15) << "Category" << endl;
        for (const auto& product : products) {
            if (product.category == categories[category_choice - 1]) {
                product.display();
            }
        }

        // Ask for product selection
        int pid, quantity;
        cout << "Enter product ID: ";
        cin >> pid;
        auto product_it = find_if(products.begin(), products.end(), [pid](const Product& p) { return p.product_id == pid; });
        if (product_it == products.end()) {
            cout << "Product not found." << endl;
            return;
        }
        cout << "Enter quantity: ";
        cin >> quantity;
        if (quantity > product_it->quantity) {
            cout << "Insufficient quantity of product." << endl;
            return;
        }

        // Update subtotal
        subtotal += product_it->price * quantity;

        // Deduct quantity from total stock
        product_it->quantity -= quantity;
       

        // Add selected product to the list
        selected_products.emplace_back(*product_it, quantity);

        // Ask if the user wants to add more products
        cout << "Do you want to add more products? (y/n): ";
        cin >> add_more_products;

        saveProducts();

    } while (add_more_products == 'y' || add_more_products == 'Y');

    // Apply delivery charge if applicable
    float delivery_charge = 0.0;
    char delivery_option;
    cout << "\nDelivery option (h for home delivery, any other key for self-pickup): ";
    cin >> delivery_option;
    if (delivery_option == 'h' || delivery_option == 'H') {
        delivery_charge = 50.0; // Home delivery charge
    }


     // Apply membership discount if applicable
    float membership_discount = 0.0;
    if (customer_it->has_membership) {
        int credit_points = customer_it->credit_points;
        membership_discount = (credit_points / 50) * 10;
    }

    // Calculate final bill amount
    float final_bill_amount = subtotal + delivery_charge - membership_discount;

    // Update customer credit points
    customer_it->credit_points += 10; // Add 10 credit points for each sale

    // Save updated customer data
    saveCustomers();
    saveProducts();

   

    char use_credit_points;
    cout << "Do you want to use credit points for discount? (yes/no): ";
    cin >> use_credit_points;

    if (use_credit_points == 'y' || use_credit_points == 'Y') {
        // Apply credit points discount if applicable
        float credit_points_discount = 0.0;
        if (customer_it->has_membership && customer_it->credit_points > 0) {
            // Calculate discount based on every 5 credit points
            int points_used = customer_it->credit_points / 5;
            credit_points_discount = points_used * 2.0;
            // Deduct used credit points
            customer_it->credit_points -= points_used * 5;
            // Save updated customer data
            saveCustomers();
        }
        // Adjust final bill amount
        final_bill_amount -= credit_points_discount;

        cout << "Credit points discount applied: " << fixed << setprecision(2) << credit_points_discount << " INR\n";
    }

   

    // Display bill
    cout << "\n\n******************* Supermarket Bill *******************\n";
    cout << "Customer Name: " << name << endl;
    cout << "Customer Address: " << address << endl;
    cout << "--------------------------------------------------------\n";
    cout << left << setw(20) << "Product" << setw(15) << "Price/Unit" << setw(10) << "Quantity" << setw(15) << "Total" << endl;
    for (const auto& selected_product : selected_products) {
        const Product& product = selected_product.first;
        int quantity = selected_product.second;
        float total_price = product.price * quantity;
        cout << left << setw(20) << product.name << setw(15) << product.price << setw(10) << quantity << setw(15) << total_price << endl;
    }
    cout << "--------------------------------------------------------\n";
    cout << "Subtotal: " << fixed << setprecision(2) << subtotal << " INR\n";
    if (delivery_charge > 0.0) {
        cout << "Delivery Charge: " << fixed << setprecision(2) << delivery_charge << " INR\n";
    }
   
    cout << "--------------------------------------------------------\n";
    cout << "Final Bill Amount: " << fixed << setprecision(2) << final_bill_amount << " INR\n";
    cout << "--------------------------------------------------------\n";
    cout << "Thank you for shopping with us!\n";
    cout << "********************************************************\n";

}

   




   
    void customerMenu() {
    int choice;
    do {
        cout << "\nCustomer Management Menu\n";
        cout << "1. Add Customer\n";
        cout << "2. Display Customers\n";
        cout << "3. Delete Customer\n";
        cout << "4. Modify Customer\n";
        cout << "5. Search Customer by Name\n"; // New option for searching customer
        cout << "0. Back to Main Menu\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                addCustomer();
                break;
            case 2:
                displayCustomers();
                break;
            case 3:
                deleteCustomer();
                break;
            case 4:
                modifyCustomer();
                break;
            case 5:
                searchCustomerByName(customers); // Call the new method for searching customer
                break;
            case 0:
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
        }
    } while (choice != 0);
}

    void productMenu() {
    int choice;
    do {
        cout << "\nProduct Management Menu\n";
        cout << "1. Add Product\n";
        cout << "2. Display Products\n";
        cout << "3. Display Products by Category\n";
        cout << "4. Delete Product\n";
        cout << "5. Modify Product\n";
        cout << "6. Search Product by Name\n"; // New option for searching product
        cout << "0. Back to Main Menu\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                addProduct();
                break;
            case 2:
                displayProducts();
                break;
            case 3:
                displayProductsByCategory();
                break;
            case 4:
                deleteProduct();
                break;
            case 5:
                modifyProduct();
                break;
            case 6:
                searchProductByName(products); // Call the new method for searching product
                break;
            case 0:
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
        }
    } while (choice != 0);
}
   
   

    void salesMenu() {
        int choice;
        do {
            cout << "\nSales Management Menu\n";
            cout << "1. Create Bill\n";
            cout << "2. Display Sales\n";
            cout << "0. Back to Main Menu\n";
            cout << "Enter your choice: ";
            cin >> choice;

            switch (choice) {
                case 1:
                    createBill();
                    break;
                case 2:
                    displaySales();
                    break;
                case 0:
                    break;
                default:
                    cout << "Invalid choice. Please try again." << endl;
            }
        } while (choice != 0);
    }

    void displayProductsByCategory() const {
        cout << "\nProduct Categories:" << endl;
        for (size_t i = 0; i < categories.size(); ++i) {
            cout << i + 1 << ": " << categories[i] << endl;
        }
        int category_choice;
        cout << "Choose a category: ";
        cin >> category_choice;
        if (category_choice < 1 || category_choice > categories.size()) {
            cout << "Invalid category choice." << endl;
            return;
        }

        cout << "\nProducts in category '" << categories[category_choice - 1] << "':" << endl;
        cout << left << setw(10) << "ID" << setw(20) << "Name" << setw(10) << "Price" << setw(10) << "Quantity" << setw(15) << "Category" << endl;
        for (const auto& product : products) {
            if (product.category == categories[category_choice - 1]) {
                product.display();
            }
        }
    }

    void displayCustomers() const {
        cout << left << setw(10) << "ID" << setw(20) << "Name" << setw(20) << "Address" << setw(15) << "Phone" << setw(10) << "Member" << setw(10) << "Points" << endl;
        for (const auto& c : customers) {
            c.display();
        }
    }

    void displaySales() const {
        cout << left << setw(10) << "ID" << setw(10) << "Product" << setw(10) << "Customer" << setw(10) << "Quantity" << setw(20) << "DateTime" << endl;
        for (const auto& s : sales) {
            s.display();
        }
    }

    string getCurrentDateTime() const {
        time_t now = time(0);
        tm* ltm = localtime(&now);
        stringstream ss;
        ss << 1900 + ltm->tm_year << "-" << 1 + ltm->tm_mon << "-" << ltm->tm_mday << " ";
        ss << ltm->tm_hour << ":" << ltm->tm_min << ":" << ltm->tm_sec;
        return ss.str();
    }
};

int main() {
    Supermarket supermarket;
    int choice;
    do {
        cout << "\nMain Menu\n";
        cout << "1. Customer Management\n";
        cout << "2. Product Management\n";
        cout << "3. Sales Management\n";
        cout << "0. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                supermarket.customerMenu();
                break;
            case 2:
                supermarket.productMenu();
                break;
            case 3:
                supermarket.salesMenu();
                break;
            case 0:
                cout << "Exiting program..." << endl;
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
        }
} while (choice != 0);

    return 0;
}
