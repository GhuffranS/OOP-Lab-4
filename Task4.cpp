#include <iostream>
#include <string>

using namespace std;

class Book {
private:
    string title;
    float basePrice;
    float discountedPrice;
    int stockLevel;

public:
    Book(string titleI, float priceI, int stockLevelI) {
        title = titleI;
        basePrice = priceI;
        discountedPrice = priceI;
        stockLevel = stockLevelI;
    }

    Book() {
        title = "Unknown";
        basePrice = 0.0;
        discountedPrice = 0.0; 
        stockLevel = 30;
    }

    void updateDetails(string titleI, float priceI, int stockI) {
        title = titleI;
        basePrice = priceI;
        discountedPrice = priceI;
        stockLevel = stockI;
    }

    void makePurchase(int quantity) {
        if (quantity > stockLevel) {
            cout << "Books Not Available" << endl;
            return;
        }

        discountedPrice = basePrice; 
        if (quantity > 10) {
            discountedPrice *= 0.9; 
        } else if (quantity > 5) {
            discountedPrice *= 0.95; 
        }

        stockLevel -= quantity;
        cout << "Books Purchased : " << quantity << endl;
        cout << "Price per Book: " << discountedPrice << endl; 

        if (stockLevel <= 5) {
            cout << "Warning! Low Stock Level" << endl;
        }
    }

    string getTitle() const { return title; }
    float getPrice() const { return discountedPrice; }
    int getStockLevel() const { return stockLevel; }
};

int main() {
    Book book1("Hemmingway", 55.4, 50);
    book1.makePurchase(50); 
    cout << "Title: " << book1.getTitle() << endl;
    cout << "Price: " << book1.getPrice() << endl;
    cout << "Stock Level: " << book1.getStockLevel() << endl; 

    return 0;
}