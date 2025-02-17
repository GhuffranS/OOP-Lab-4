#include <iostream>
#include <string>

using namespace std;

class Book {
private:
    static int nextBookID; 
    string title;
    float basePrice;
    float discountedPrice;
    int stockLevel;
    int bookID; 

public:
    Book(string titleI, float priceI, int stockLevelI) : 
        title(titleI), basePrice(priceI), discountedPrice(priceI), 
        stockLevel(stockLevelI), bookID(nextBookID++) {} 

    Book() : 
        title("Unknown"), basePrice(0.0), discountedPrice(0.0), 
        stockLevel(30), bookID(nextBookID++) {} 

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

    void displayDetails() const {
        cout << "Book ID: " << bookID << endl;
        cout << "Title: " << title << endl;
        cout << "Price: " << discountedPrice << endl;
        cout << "Stock Level: " << stockLevel << endl;
    }

    string getTitle() const { return title; }
    float getPrice() const { return discountedPrice; }
    int getStockLevel() const { return stockLevel; }
};

int Book::nextBookID = 1; 

int main() {
    Book book1("Hemmingway", 55.4, 50);
    Book book2("The Lord of the Rings", 29.99, 100);

    book1.makePurchase(50);
    book2.makePurchase(15);

    cout << "\nBook 1 Details:\n";
    book1.displayDetails();

    cout << "\nBook 2 Details:\n";
    book2.displayDetails();

    return 0;
}