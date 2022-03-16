#include <iostream>
#include <map>
#include "rapidcsv.h"

class Customer
        {
        public:
            int id;
            std::string name;
            std::string address;

            void print()
            {
                std::cout << "Customer: " << name << ", " << address << std::endl;
            }
};

class Product
        {
        public:
            int id;
            std::string name;
            int price;

            void print()
            {
                std::cout << "Product: " << name << ", " << price << std::endl;
            }
        };


int main() {
    rapidcsv::Document customersDoc("customers.csv");
    std::map<int, Customer> customers;
    std::map<int, int> customerAmounts;
    for (int i = 0; i < customersDoc.GetRowCount(); i++)
    {
        Customer customer;
        customer.id = customersDoc.GetCell<int>("id", i);
        customer.name = customersDoc.GetCell<std::string>("name", i);
        customer.address = customersDoc.GetCell<std::string>("address", i);
        customer.print();
        customers[customer.id] = customer;
        customerAmounts.insert(std::pair<int, int>(customer.id, 0));
    }

    rapidcsv::Document productsDoc("products.csv");
    std::map<int, Product> products;
    std::map<int, int> productAmounts;
    for (int i = 0; i < productsDoc.GetRowCount(); i++)
    {
        Product product;
        product.id = productsDoc.GetCell<int>("id", i);
        product.name = productsDoc.GetCell<std::string>("name", i);
        product.price = productsDoc.GetCell<int>("price", i);
        product.print();
        products[product.id] = product;
        productAmounts.insert(std::pair<int, int>(product.id, 0));
    }


    rapidcsv::Document ordersDoc("orders.csv");
    std::map<std::pair<int, int>, int> orders;
    for (int i = 0; i < ordersDoc.GetRowCount(); i++)
    {
        int customerId = ordersDoc.GetCell<int>("customerid", i);
        int productId = ordersDoc.GetCell<int>("productid", i);
        int amount = ordersDoc.GetCell<int>("amount", i);
        std::pair<int, int> CustomerProductPair(customerId, productId);
        orders.insert(std::pair<std::pair<int, int>, int>(CustomerProductPair, amount));
        productAmounts[productId] += amount;
        customerAmounts[customerId] += amount*products[productId].price;
    }

    for (auto& product : products)
    {
        std::cout << product.second.name << " amount: " << productAmounts[product.second.id] << std::endl;
        std::cout << product.second.name << " gross income: " << productAmounts[product.second.id]*product.second.price << std::endl;
    }

    for (auto& customer : customers)
    {
        std::cout << customer.second.name << " money spent: " << customerAmounts[customer.second.id] << std::endl;
    }

    return 0;
}
