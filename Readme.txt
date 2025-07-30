Grocery Store Management System (C++ OOP Project)

Overview
This is a C++ Object-Oriented Programming (OOP) based Grocery Store Management System.
It allows customers to browse and purchase products while admins can manage inventory and view sales history.
The project uses file handling to store product inventory and sales bills.

Features
Customer Mode
View available products

Add items to cart

Remove or update items in cart

Search products by name

Apply discount coupons (SAVE10, SAVE20)

Checkout and generate a bill (saved to bill.txt)

Admin Mode
Secure admin login (password protected)

Add new products

Remove products

Update stock and price of products

View complete sales history

File Handling
The program automatically creates and manages the following files:

inventory.txt – Stores all product details and stock quantity

bill.txt – Stores the details of all purchases (sales history)

Both files are stored in the same directory where the program is executed.

How to Run
Compilation
bash
Copy
Edit
g++ store.cpp -o store
Execution
bash
Copy
Edit
./store
Usage
Main Menu
markdown
Copy
Edit
1. Customer Mode
2. Admin Mode
3. Exit
Customer Menu
markdown
Copy
Edit
1. View Products
2. Add Item
3. View Cart
4. Remove Item
5. Update Quantity
6. Search Product
7. Apply Coupon
8. Checkout
9. Back
Admin Menu
markdown
Copy
Edit
1. Add Product
2. Remove Product
3. Update Stock/Price
4. View Sales History
5. Back
Default Admin Password
nginx
Copy
Edit
admin123
Example Bill Output (bill.txt)
pgsql
Copy
Edit
=== New Purchase === Wed Jul 30 22:10:32 2025
Apple x 2 $5.98
Milk x 1 $2.49
TOTAL: $8.87
Technologies Used
Language: C++

Concepts: Classes, Objects, Encapsulation, File Handling, OOP Principles

