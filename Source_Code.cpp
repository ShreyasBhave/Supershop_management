#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <iomanip>
#include <ctime>
using namespace std;

class Product {
public:
    int id;
    string name;
    double price;
    int stock;

    Product(int i, string n, double p, int s) {
        id = i; name = n; price = p; stock = s;
    }
};

class CartItem {
public:
    int id;
    string name;
    double price;
    int qty;

    CartItem(int i, string n, double p, int q) {
        id = i; name = n; price = p; qty = q;
    }
};

class Store {
private:
    vector<Product> products;
    vector<CartItem> cart;
    double taxRate = 0.05;
    double discount = 0.0;

public:
    Store() { loadData(); }

    void saveData() {
        ofstream inv("inventory.txt");
        for (auto &p : products)
            inv << p.id << " " << p.name << " " << p.price << " " << p.stock << "\n";
        inv.close();
    }

    void loadData() {
        ifstream inv("inventory.txt");
        products.clear();
        int id, stock; string name; double price;
        while (inv >> id >> name >> price >> stock)
            products.push_back(Product(id, name, price, stock));
        inv.close();

        if (products.empty()) {
            // Initial products if file is empty
            products.push_back(Product(101, "Apple", 2.99, 20));
            products.push_back(Product(102, "Orange", 3.99, 15));
            products.push_back(Product(103, "Banana", 1.99, 25));
            products.push_back(Product(104, "Grapes", 5.99, 10));
            products.push_back(Product(105, "Milk", 2.49, 30));
            products.push_back(Product(106, "Eggs", 1.99, 40));
            products.push_back(Product(107, "Bread", 3.49, 25));
        }
    }

    void displayProducts() {
        cout << "\nAvailable Products:\n---------------------------\n";
        for (auto &p : products) {
            cout << "ID: " << p.id << " | " << p.name
                 << " | $" << fixed << setprecision(2) << p.price
                 << " | Stock: " << p.stock << "\n";
        }
    }

    void addItem(int pid, int q) {
        for (auto &p : products) {
            if (p.id == pid) {
                if (q <= 0) { cout << "Invalid quantity.\n"; return; }
                if (q > p.stock) { cout << "Not enough stock.\n"; return; }
                cart.push_back(CartItem(p.id, p.name, p.price, q));
                p.stock -= q;
                cout << q << " " << p.name << " added to cart.\n";
                saveData();
                return;
            }
        }
        cout << "Invalid product ID.\n";
    }

    void viewCart() {
        if (cart.empty()) { cout << "Cart is empty.\n"; return; }
        cout << "\nYour Cart:\n---------------------------\n";
        for (auto &c : cart)
            cout << c.name << " | $" << c.price << " x " << c.qty << "\n";
        cout << "Total (with tax/discount): $" << calcTotal() << "\n";
    }

    void removeItem(int pid) {
        for (int i = 0; i < cart.size(); i++) {
            if (cart[i].id == pid) {
                for (auto &p : products)
                    if (p.id == pid) p.stock += cart[i].qty;
                cart.erase(cart.begin() + i);
                cout << "Item removed.\n";
                saveData();
                return;
            }
        }
        cout << "Item not in cart.\n";
    }

    void updateQuantity(int pid, int q) {
        for (int i = 0; i < cart.size(); i++) {
            if (cart[i].id == pid) {
                for (auto &p : products) {
                    if (p.id == pid) {
                        int available = p.stock + cart[i].qty;
                        if (q <= 0 || q > available) {
                            cout << "Invalid quantity.\n"; return;
                        }
                        p.stock = available - q;
                        cart[i].qty = q;
                        cout << "Quantity updated.\n";
                        saveData();
                        return;
                    }
                }
            }
        }
        cout << "Item not found in cart.\n";
    }

    void searchProduct(string nm) {
        bool found = false;
        for (auto &p : products) {
            if (p.name == nm) {
                cout << p.name << " | $" << p.price << " | Stock: " << p.stock << "\n";
                found = true;
            }
        }
        if (!found) cout << "Product not found.\n";
    }

    void applyCoupon(string code) {
        if (code == "SAVE10") discount = 0.10;
        else if (code == "SAVE20") discount = 0.20;
        else { cout << "Invalid coupon.\n"; return; }
        cout << "Coupon applied successfully.\n";
    }

    void checkout() {
        if (cart.empty()) { cout << "Cart is empty.\n"; return; }
        double total = calcTotal();
        cout << "Final Amount to Pay: $" << total << "\n";

        ofstream bill("bill.txt", ios::app);
        time_t now = time(0);
        bill << "=== New Purchase === " << ctime(&now);
        for (auto &c : cart)
            bill << c.name << " x " << c.qty << " $" << c.price * c.qty << "\n";
        bill << "TOTAL: $" << total << "\n\n";
        bill.close();

        cout << "Payment Done. Bill saved to bill.txt.\n";
        cart.clear();
        saveData();
    }

    // ---------- ADMIN FUNCTIONS ----------
    void addNewProduct(int id, string nm, double pr, int st) {
        products.push_back(Product(id, nm, pr, st));
        cout << "Product added.\n";
        saveData();
    }

    void removeProduct(int id) {
        for (int i = 0; i < products.size(); i++) {
            if (products[i].id == id) {
                products.erase(products.begin() + i);
                cout << "Product removed.\n";
                saveData();
                return;
            }
        }
        cout << "Product not found.\n";
    }

    void updateStock(int id, int st, double pr) {
        for (auto &p : products) {
            if (p.id == id) {
                p.stock = st;
                p.price = pr;
                cout << "Stock/Price updated.\n";
                saveData();
                return;
            }
        }
        cout << "Product not found.\n";
    }

    void viewSalesHistory() {
        ifstream bill("bill.txt");
        if (!bill.is_open()) { cout << "No sales history yet.\n"; return; }
        cout << "\n=== Sales History ===\n";
        string line;
        while (getline(bill, line)) cout << line << "\n";
        bill.close();
    }

private:
    double calcTotal() {
        double total = 0;
        for (auto &c : cart) total += c.price * c.qty;
        total -= total * discount;
        total += total * taxRate;
        return total;
    }
};

int main() {
    Store shop;

    while (true) {
        cout << "\n1. Customer Mode\n2. Admin Mode\n3. Exit\nChoice: ";
        int mode; cin >> mode;

        if (mode == 1) {
            while (true) {
                cout << "\n--- Customer Menu ---\n";
                cout << "1. View Products\n2. Add Item\n3. View Cart\n4. Remove Item\n5. Update Quantity\n";
                cout << "6. Search Product\n7. Apply Coupon\n8. Checkout\n9. Back\nChoice: ";
                int ch; cin >> ch;
                if (ch == 1) shop.displayProducts();
                else if (ch == 2) { int id, q; cout << "Enter ID & Qty: "; cin >> id >> q; shop.addItem(id,q); }
                else if (ch == 3) shop.viewCart();
                else if (ch == 4) { int id; cout << "Enter ID: "; cin >> id; shop.removeItem(id); }
                else if (ch == 5) { int id,q; cout << "Enter ID & New Qty: "; cin >> id >> q; shop.updateQuantity(id,q); }
                else if (ch == 6) { string nm; cout << "Enter Name: "; cin >> nm; shop.searchProduct(nm); }
                else if (ch == 7) { string c; cout << "Enter Coupon: "; cin >> c; shop.applyCoupon(c); }
                else if (ch == 8) shop.checkout();
                else if (ch == 9) break;
            }
        }
        else if (mode == 2) {
            string pass; cout << "Enter Admin Password: "; cin >> pass;
            if (pass != "admin123") { cout << "Wrong password!\n"; continue; }

            while (true) {
                cout << "\n--- Admin Menu ---\n";
                cout << "1. Add Product\n2. Remove Product\n3. Update Stock/Price\n4. View Sales History\n5. Back\nChoice: ";
                int a; cin >> a;
                if (a == 1) { int id,s; string n; double p; cout << "Enter ID Name Price Stock: "; cin >> id >> n >> p >> s; shop.addNewProduct(id,n,p,s); }
                else if (a == 2) { int id; cout << "Enter ID: "; cin >> id; shop.removeProduct(id); }
                else if (a == 3) { int id,s; double p; cout << "Enter ID NewStock NewPrice: "; cin >> id >> s >> p; shop.updateStock(id,s,p); }
                else if (a == 4) shop.viewSalesHistory();
                else if (a == 5) break;
            }
        }
        else if (mode == 3) break;
    }
}
