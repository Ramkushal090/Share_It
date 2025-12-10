#include "classes.h"
#include <iostream>
#include <vector>
#include <string>

using namespace std;

// Constructor
User::User(string username, string password)
    : username(username), password(password), coins(100) {}

// Authentication
bool User::authenticate(string &entered_username, string &entered_password) const {
    return this->username == entered_username && this->password == entered_password;
}

// Getters
string User::getUsername() const {
    return this->username;
}

// Listings Management
void User::listItem(Listing *listing) {
    this->listings.push_back(listing);
}

void User::removeListing(Listing *listing) {
    Listing *listing_to_del = nullptr;
    for (size_t i = 0; i < this->listings.size(); i++) {
        if (this->listings[i] == listing) {
            listing_to_del = this->listings[i];
            this->listings.erase(this->listings.begin() + i);
            break;
        }
    }
    delete listing_to_del;
    listing_to_del = nullptr;
}

vector<Listing*> User::getListings() const {
    return this->listings;
}

vector<Item*> User::getRequestedItems() const {
    return this->requested_items;
}

// Request Management
void User::requestItem(Item *requested_item) {
    this->requested_items.push_back(requested_item);
}

void User::removeRequest(string &name) {
    Item *item_to_del = nullptr;
    for (size_t i = 0; i < this->requested_items.size(); i++) {
        if (this->requested_items[i]->getName() == name) {
            item_to_del = this->requested_items[i];
            this->requested_items.erase(this->requested_items.begin() + i);
            break;
        }
    }
    delete item_to_del;
    item_to_del = nullptr;
}

void User::removeRequest(int request_number) {
    Item *item_to_del = this->requested_items[request_number];
    this->requested_items.erase(this->requested_items.begin() + request_number);
    delete item_to_del;
    item_to_del = nullptr;
}

// Borrowing Management
void User::borrowItem(Item *item) {
    this->borrowed_items.push_back(item);
}

vector<Item*> User::getBorrowedItems() const {
    return this->borrowed_items;
}

void User::returnItem(int borrowed_number) {
    this->borrowed_items.erase(this->borrowed_items.begin() + borrowed_number - 1);
}

void User::returnItem(Item* borrowed_item) {
    Item *item_to_del = nullptr;
    for (size_t i = 0; i < this->borrowed_items.size(); i++) {
        if (this->borrowed_items[i] == borrowed_item) {
            item_to_del = this->borrowed_items[i];
            this->borrowed_items.erase(this->borrowed_items.begin() + i);
            break;
        }
    }
    delete item_to_del;
    item_to_del = nullptr;
}

// Notifications
vector<string> User::getNotificationsString() const {
    vector<string> notifications_string;
    for (Notification *notification : this->notifications) {
        notifications_string.push_back(notification->getNotificationString());
    }
    return notifications_string;
}

void User::printNotifications() const {
    cout << "Notifications (Total: " << this->notifications.size() << ")" << endl;
    cout << separator << endl;
    for (int i = 0; i < this->notifications.size(); i++) {
        cout << "----" << i + 1 << ":";
        notifications[i]->printNotification();
        cout << separator << endl;
    }
}

int User::getNumberOfNotifications() const {
    return this->notifications.size();
}

Notification *User::getNotification(int notification_number) const {
    return this->notifications[notification_number];
}

void User::addNotification(Notification *notification) {
    this->notifications.push_back(notification);
}

void User::removeNotification(Notification *notification) {
    Notification *notification_to_del = nullptr;
    for (size_t i = 0; i < this->notifications.size(); i++) {
        if (this->notifications[i] == notification) {
            notification_to_del = this->notifications[i];
            this->notifications.erase(this->notifications.begin() + i);
            break;
        }
    }
    delete notification_to_del;
    notification_to_del = nullptr;
}

void User::removeNotification(int notification_number) {
    Notification *notification_to_del = this->notifications[notification_number];
    this->notifications.erase(this->notifications.begin() + notification_number);
    delete notification_to_del;
    notification_to_del = nullptr;
}

void User::printBorrowedItems() const {
    cout << "Borrowed Items (Total: " << this->borrowed_items.size() << ")" << endl;
    cout << separator << endl;
    int borrowed_number = 0;
    for (Item *item : this->borrowed_items) {
        cout << "Borrowed Item " << ++borrowed_number << ":" << endl;
        item->printItem();
        cout << separator << endl;
    }
}

void User::clearNotifications() {
    for (Notification *notification : this->notifications) {
        delete notification;
    }
    this->notifications.clear();
}

// Coins Management
void User::acceptCoins(int coins_given) {
    this->coins += coins_given;
}

bool User::spendCoins(int coins_to_spend) {
    if (coins_to_spend > this->coins)
        return false;
    this->coins -= coins_to_spend;
    return true;
}

int User::getCoinBalance() const {
    return this->coins;
}

// Destructor
User::~User() {
    for (Listing *listing : this->listings) {
        delete listing;
    }
    for (Item *item : this->requested_items) {
        delete item;
    }
    for (Item *item : this->borrowed_items) {
        delete item;
    }
    clearNotifications();
}
