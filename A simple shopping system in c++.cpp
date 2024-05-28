#include <iostream>
#include <fstream>
#include <string>
#include <limits>
#include <cstdlib>

enum sizeType {
    S, M, L
};

struct moldType {
    std::string shape;
    sizeType size;
    double price;
    double weight;
};

struct customerType {
    std::string fname;
    std::string lname;
    std::string address;
    std::string cityStateZip;
};

struct orderType {
    customerType customer;
    moldType products[10];
    int numProdOrdered;
    double subTotal;
    double totalWeight;
    double shippingCost;
};

// Function to read the product catalog from a file
void readCatalog(moldType productCatalog[], int numProducts) {
    std::ifstream catalogFile("catalog.txt");
    if (!catalogFile.is_open()) {
        std::cerr << "Error opening catalog file." << std::endl;
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < numProducts; ++i) {
        std::string sizeStr;  // Read size as string
        catalogFile >> productCatalog[i].shape >> sizeStr >> productCatalog[i].price >> productCatalog[i].weight;

        // Convert the size to sizeType
        if (sizeStr == "S") {
            productCatalog[i].size = S;
        } else if (sizeStr == "M") {
            productCatalog[i].size = M;
        } else if (sizeStr == "L") {
            productCatalog[i].size = L;
        } else {
            std::cerr << "Error: Invalid size in catalog." << std::endl;
            exit(EXIT_FAILURE);
        }
    }

    catalogFile.close();
}


// Function to print product information
void printProduct(std::ostream& out, moldType mold) {
    out << mold.shape << " - ";
    switch (mold.size) {
        case S:
            out << "Small";
            break;
        case M:
            out << "Medium";
            break;
        case L:
            out << "Large";
            break;
    }
    out << " $" << mold.price << " " << mold.weight << "g" << std::endl;
}

// Function to enter order
void enterOrder(orderType& order, moldType catalog[], int numItemsInCatalog) {
     std::cout << "Enter the customer first name: ";
    std::cin >> order.customer.fname;
    std::cout << "Enter the customer last name: ";
    std::cin >> order.customer.lname;
    std::cout << "Enter the customer street address: ";
    std::cin.ignore();
    std::getline(std::cin, order.customer.address);
    std::cout << "Enter the customer city, state, and zip code: ";
    std::getline(std::cin, order.customer.cityStateZip);

    order.numProdOrdered = 0;
    order.subTotal = 0.0;
    order.totalWeight = 0.0;

    char addAnother;
    do {
        std::cout << "Please choose a product from the list:" << std::endl;
        for (int i = 0; i < numItemsInCatalog; ++i) {
            std::cout << i + 1 << ". ";
            printProduct(std::cout, catalog[i]);
        }

        int choice;
        do {
            std::cout << "Enter product number: ";
            std::cin >> choice;
            if (choice < 1 || choice > numItemsInCatalog) {
                std::cout << "Invalid product number. Please choose a product from the list above." << std::endl;
            }
        } while (choice < 1 || choice > numItemsInCatalog);

        moldType selectedProduct = catalog[choice - 1];
        order.products[order.numProdOrdered] = selectedProduct;

        std::cout << "Item: ";
        printProduct(std::cout, selectedProduct);
        std::cout << "added." << std::endl;

        order.subTotal += selectedProduct.price;
        order.totalWeight += selectedProduct.weight;
        order.numProdOrdered++;

        if (order.numProdOrdered < 10) {
            std::cout << "Do you want to add another product to the order? (y/n): ";
            std::cin >> addAnother;
        } else {
            std::cout << "Maximum number of products ordered." << std::endl;
            addAnother = 'n';
        }

    } while (addAnother == 'y');
}

// Function to print order information
void printOrder(std::ostream& out, orderType& order) {
    out << "\nCustomer: " << order.customer.fname << " " << order.customer.lname << "\n"
        << "Address: " << order.customer.address << "\n"
        << "Products Ordered: \n";

    for (int i = 0; i < order.numProdOrdered; ++i) {
        printProduct(out, order.products[i]);
    }

    out << "\nSubtotal: $" << order.subTotal << "\n"
        << "Total Weight: " << order.totalWeight << "g\n"
        << "Shipping Cost: $" << order.shippingCost << "\n"
        << "Total Cost: $" << order.subTotal + order.shippingCost << std::endl;
}

int main() {
    const int numProducts = 20;
    moldType catalog[numProducts];
    orderType order;

    readCatalog(catalog, numProducts);

    char anotherOrder;
    do {
        std::cout << "Welcome to Judy's Resin Molds!" << std::endl;
        enterOrder(order, catalog, numProducts);

        // Calculate shipping cost based on total weight
        order.shippingCost = 0.02 * order.totalWeight;

        // Print order to console
        printOrder(std::cout, order);

        // Print order to file with customer's last name
      std::ofstream outFile((order.customer.lname + ".txt").c_str());
if (!outFile.is_open()) {
    std::cerr << "Error creating output file." << std::endl;
    exit(EXIT_FAILURE);
}
printOrder(outFile, order);
outFile.close();

        std::cout << "Do you want to enter another order? (y/n): ";
        std::cin >> anotherOrder;

    } while (anotherOrder == 'y');

    std::cout << "Have a nice day! " << std::endl;
    std::cout <<"WELCOME BACK!" <<std::endl;
    return 0;
}