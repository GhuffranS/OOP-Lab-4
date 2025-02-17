#include <iostream>
using namespace std;

class Book{
    private:
        string title;
        float price;
        int stockLevel;
    public:
        Book(string titleI, float priceI, int stockLevelI){
            title = titleI;
            price = priceI;
            stockLevel = stockLevelI;
        }

        Book(){
            title = "Unkown";
            price = 0.0;
            stockLevel = 30;
        }

        int applyDiscount(int qty){
            if(qty > stockLevel){
                cout << "Quantity Greater Than Current Stock Level" << endl;
                return 0;
            }
            if (qty > 10)
            {
                price = price * 0.9;
                return 1;
            }
            else if (qty > 5)
            {
                price = price * 0.95;
                return 1;
            }
            
            
        }

        void updateDetails(string titleI, float priceI, int stockI){
            title = titleI;
            price = priceI;
            stockLevel = stockI;
        }

        void makePurchase(int quantity){
            applyDiscount(quantity);
            if (quantity > stockLevel)
            {
                cout << "Books Not Available"<< endl;
                return;
            }
            else{
                cout<<"Books Purchased : "<<quantity<<endl;
                stockLevel -= quantity;
                if (stockLevel <= 5)
                {
                    cout <<"Warning! Low Stock Level"<<endl;
                    return;
                }
                
            }
            
        }

};

int main(){
    Book book1;
    book1.updateDetails("Hemmingway",55.4,50);
    book1.makePurchase(50);
    return 0;
}
