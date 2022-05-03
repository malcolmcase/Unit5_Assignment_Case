#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <algorithm>
#include <fstream>
#include <unistd.h>
using namespace std;

class MenuItem
{
  private:
    string name;
    double cost;
    string desc;
    int count;
    char addLetter;
    char removeLetter;
  public:
    MenuItem(string name, double cost, string desc, int count, char addLetter, char removeLetter)
    {
      this->name = name;
      this->cost = cost;
      this->desc = desc;
      this->count = count;
      this->addLetter = addLetter;
      this->removeLetter = removeLetter;
    }
    void setName(string n) {name = n;}
    void setCost(double c) {cost = c;}
    void setDesc(string d) {desc = d;}
    void setCount(int q) {count = q;}
    void setAddLetter (char a) {addLetter = a;}
    void setRemoveLetter (char r) {removeLetter = r;}
    string getName() const {return name;}
    double getCost() const {return cost;}
    string getDesc() const {return desc;}
    int getCount() const {return count;}
    char getAddLetter() const {return addLetter;}
    char getRemoveLetter() const {return removeLetter;}
};

void populateMenu(vector<MenuItem> &m1)
{
  m1.push_back(MenuItem("Taco", 3.0, "yum", 0, 'A', 'a'));
  m1.push_back(MenuItem("Burrito", 7.0, "yum", 0, 'B', 'b'));
  m1.push_back(MenuItem("Torta", 7.0, "yum", 0, 'C', 'c'));
  m1.push_back(MenuItem("Quesadilla", 6.0, "yum", 0, 'D', 'd'));
  m1.push_back(MenuItem("Chips", 1.5, "yum", 0, 'E', 'e'));
  m1.push_back(MenuItem("Soda", 2.0, "yum", 0, 'F', 'f'));
  m1.push_back(MenuItem("Green Tea", 2.5, "yum", 0, 'G', 'g'));
}

void showMenu(vector<MenuItem> &m)
{  
  cout << fixed << setprecision(2);//set doubles to 2 decimal places
  cout << "Malcolm's Slightly More Effcient Menu!" << endl; 
  cout << "ADD  \tNAME \t COST \tREMOVE\tCOUNT\tDESC"<<endl; 
  for(int i = 0; i < m.size(); i++)
  {
    cout << m[i].getAddLetter() << ")" << setw(10) << m[i].getName() 
    << setw(5) << "$" << m[i].getCost() << setw(5) << "(" << m[i].getRemoveLetter()
    << ")" << setw(7) << m[i].getCount() << setw(13) << m[i].getDesc() 
    << endl; 
  }

}

void printReceipt(vector<MenuItem> &m, double subtotal, double tip)
{
  ofstream receipt { "receipt" };
  receipt << fixed << setprecision(2);
  receipt << "Thank you for shopping with us!\n";
  receipt << "\tSubtotal: " << subtotal << endl;
  receipt << "\tTax:      " << subtotal*0.0625 << endl;
  receipt << "\tTip:      " << tip << endl;
  receipt << "\tTotal     " << subtotal+(subtotal*0.0625)+tip << endl;
  receipt << "\t\tPAID";
}

void acceptOrder(vector<MenuItem> &m)
{
  char option = '\0';// the user-selected menu item
  double change = 0.0;
  double subtotal = 0.0; 
  double tip = 0.0;
  double total = 0.0;
  
  do
  {
    cout << "\nPlease choose an item (x to Proceed to Checkout): ";
    cin >> option; 

    for(int i=0; i < m.size(); i++)
    {
      if(option == m[i].getAddLetter())
      {
        cout << "\nMenu Item " << m[i].getAddLetter() << " selected."; 
        m[i].setCount( m[i].getCount() + 1 ); //increment the count by 1
        cout << "\033[2J\033[1;1H"; //clear screen 
        cout << "1 UP on " << m[i].getName() << endl;
        subtotal += m[i].getCost();
        showMenu(m); //show the updated menu
        cout << "\nSubtotal: $" << subtotal << endl;  
      }
      else if(option == m[i].getRemoveLetter() )
      {
        cout << "\nRemove Item " << m[i].getRemoveLetter() << " selected."; 
        if(m[i].getCount() > 0) //subtract if and only if the count is > 0
        {
          m[i].setCount( m[i].getCount() - 1 ); //decrement the count by 1
          cout << "\033[2J\033[1;1H"; //clear screen 
          cout << "1 DOWN on " << m[i].getName() << endl;
          subtotal -= m[i].getCost();
          showMenu(m);
          cout << "\nSubtotal: $" << subtotal << endl;  
        }
        else //the the user why you blocked item removal 
        {
            
            cout << "\nItem count must be > 0 to remove: " << m[i].getName() << endl;
        }
      }
      else if(
                option != m[i].getAddLetter() && 
                option != m[i].getRemoveLetter() &&
                option != 'x' &&
                option != 'X' 
            )
            {
              if(i == 0)
              {
                cout << "\nInvalid input (" << option << "). Please try again." << endl; 
              }  
            }
    }
  }while(option != 'x' && option != 'X'); 
  
  cout << "\nThank you for placing your order." << endl; 
  cout << "\nYour subtotal is " << subtotal << ".\n";
  cout << "Please enter tip amount:\n(Recommended Tip (20%): $" << subtotal*0.2 << ")" << endl;
  cin >> tip;
  total = (subtotal*0.0625)+tip+subtotal;
  
  system("clear");
  cout << "Your total is (with tax) $" << total << endl;
  cout << "Would you like to pay cash or with a credit card?\n";
  cout << "A) Cash\nB) Card\n";
  cin >> option;
  if (option == 'A' || option == 'a') //cash process
    {
      double tender;
      double due;
      due = total;
      
      while(due > 0.0)
        {
          system("clear");
          cout << "Amount Due: $" << due << endl;
          cout << "Enter amount paid:\n";
          cin >> tender;
          if (tender >= due)
          {
            change = (tender-due);
            cout << "Thank you for your purchase, your change is $" << change << endl;
            due = (due-tender);
            printReceipt(m, subtotal, tip);
            sleep(2);
            system("clear");
          }
          else if (tender < due)
          {
            due = (due-tender);
          }
        }
    }
  else if (option == 'B' || option == 'b') //credit process
    {
      string card;
      cout << "Please enter card number:\n";
      cin >> card;
      cout << "\nThank you for your purchase.\n";
      printReceipt(m, subtotal, tip);
      sleep(2);
      system("clear");
    }
  else if
      (option != 'A' &&
      option != 'a' && 
      option != 'B' && 
      option != 'b' &&
      option != 'x' &&
      option != 'X' )
      {
        cout << "\nInvalid input (" << option << "). Please try again." << endl; 
      }
}

int main() 
{ 
  char admin;
  
  do{
  cout << "Would you like to create a new order?\nA) Create New Order\nB) Quit\n";
  cin >> admin;
    if (admin == 'A' || admin == 'a')
      {
        system("clear");
        vector<MenuItem> wholeMenu; 
        populateMenu(wholeMenu);
        showMenu(wholeMenu);
        acceptOrder(wholeMenu); 
      }
    else if (admin == 'B' || admin == 'b')
      {
        cout << "\nLogging off...";
      }
    else
      {
        cout << "Invalid input, try again.\n\n";
      }
  }while (admin != 'B' && admin != 'b'); //DeMorgan's Law
    
  return 0; 
}