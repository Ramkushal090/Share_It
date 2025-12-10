#include "classes.h"

#include <string>
#include <regex>
#include <stdexcept>

using namespace std;

// This file contains both the item_class and the listing_class.

// Item class

Item::Item(string name, categories category, int quantity, string from_date, string to_date, User *user, User *borrower) :
        name(name), category(category), quantity(quantity), from_date(from_date), to_date(to_date),
        owner(user), borrower(borrower) {}

Item::Item(string &name, categories category, int &quantity, string &from_date, string &to_date) :
        name(name), category(category), quantity(quantity), from_date(from_date), to_date(to_date),
        owner(nullptr), borrower(nullptr) {}

string Item::getName() const {
    return this->name;
}

User* Item::getOwner() const {
    return this->owner;
}

User* Item::getBorrower() const {
    return this->borrower;
}

categories Item::getCategory() const {
    return this->category;
}

int Item::getQuantity() const {
    return this->quantity;
}

string Item::getFromDate() const {
    return this->from_date;
}

string Item::getToDate() const {
    return this->to_date;
}

void Item::updateQuantity(int new_quantity) {
    this->quantity = new_quantity;
}

// Dates should be of the form "DD-MM-YYYY"

void Item::updateFromDate(string new_from_date) {
    int day, month, year;
    
    if (!isValidDate(new_from_date, day, month, year)) {
        throw invalid_argument("Invalid from_date format. Please use 'DD-MM-YYYY'.");
    }
    
    // Check if the new_from_date is before the current to_date
    if (!to_date.empty() && compareDates(new_from_date, to_date) > 0) {
        throw invalid_argument("from_date cannot be after to_date.");
    }

    this->from_date = new_from_date;
}

void Item::updateToDate(string new_to_date) {
    int day, month, year;
    
    if (!isValidDate(new_to_date, day, month, year)) {
        throw invalid_argument("Invalid to_date format. Please use 'DD-MM-YYYY'.");
    }
    
    // Check if the new_to_date is after the current from_date
    if (!from_date.empty() && compareDates(new_to_date, from_date) < 0) {
        throw invalid_argument("to_date cannot be before from_date.");
    }

    this->to_date = new_to_date;
}

// Print details of Item
void Item::printItem() {
    cout << "Item Name: " << name << endl;
    cout << "Category: " << categoryToString(category) << endl;
    if (owner != nullptr)
        cout << "Owner: " << owner->getUsername() << endl;
    if (borrower != nullptr)
        cout << "Borrower: " << borrower->getUsername() << endl;
    cout << "Quantity: " << quantity << endl;
    cout << "Availability: " << from_date << " to " << to_date << endl;
}

// Listing class

Listing::Listing(Item* item_listed, int item_price, conditions item_condition)
        : item_listed(item_listed), item_price(item_price), item_condition(item_condition), item_available(true) {}

bool Listing::isAvailable() const {
    return this->item_available;
}

Item* Listing::getItem() const {
    return this->item_listed;
}

void Listing::bookItem() {
    // Might need some more work
    this->item_available = false;
}

void Listing::freeItem() {
    // Might need some more work
    this->item_available = true;
}

void Listing::printListing() {
    this->item_listed->printItem();
    cout << "Price: " << this->item_price << endl;
    cout << "Condition: " << conditionToString(this->item_condition) << endl;
}

void Listing::printItem() {
    this->item_listed->printItem();
    cout << "Price: " << this->item_price << endl;
    cout << "Condition: " << conditionToString(this->item_condition) << endl;
    cout << "Availability: " << (this->item_available ? "Available" : "Not Available") << endl;
}

string Listing::getName() const {
    return this->item_listed->getName();
}

categories Listing::getCategory() const {
    return this->item_listed->getCategory();
}

conditions Listing::getCondition() const {
    return this->item_condition;
}

int Listing::getPrice() const {
    return this->item_price;
}

int Listing::getQuantity() const {
    return this->item_listed->getQuantity();
}

User* Listing::getOwner() const {
    return this->item_listed->getOwner();
}
