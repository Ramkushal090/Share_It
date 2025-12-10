#ifndef CLASSES_H
#define CLASSES_H

#include <bits/stdc++.h>

using namespace std;

const string separator = "------------------------------------------------------------";
enum conditions
{
    excellent,
    good,
    fair,
    poor,
    INVALID
};
enum categories
{
    electronics,
    furniture,
    clothing,
    books,
    sports,
    other
};

enum notificationTypes
{
    requestOwnerToBorrowItem,
    acceptBorrower,
    rejectBorrower
};

categories stringToCategory(string &category);
string categoryToString(categories category);
conditions stringToCondition(string &condition);
string conditionToString(conditions condition);
string typeToString(notificationTypes type);
notificationTypes stringToType(string &type);
void printAvailableConditions();
void printAvailableCategories();
bool isValidDate(const string& date, int &day, int &month, int &year);
int compareDates(const string& date1, const string& date2);
bool areValidateDates(const string& from_date, const string& to_date);

class User;

class Item {
protected:
    string name;
    User* owner;
    User* borrower;
    categories category;
    int quantity;
    string from_date; // date should be of the form "DD-MM-YYYY"
    string to_date;

public:
    Item(string name, categories category, int quantity, string from_date, string to_date,
         User *user, User *borrower = nullptr);
    Item(string &name, categories category, int &quantity, string &from_date, string &to_date);
    string getName() const;
    User* getOwner() const;
    User* getBorrower() const;
    categories getCategory() const;
    int getQuantity() const;
    string getFromDate() const;
    string getToDate() const;

    void updateQuantity(int new_quantity);
    void updateFromDate(string new_from_date);
    void updateToDate(string new_to_date);

    virtual void printItem();
};

class Listing {
private:
    Item* item_listed;
    bool item_available;
    int item_price;
    conditions item_condition;

public:
    Listing(Item* item_listed, int item_price, conditions item_condition);
    bool isAvailable() const;
    Item* getItem() const;
    void bookItem();
    void freeItem();
    void printListing();

    void printItem();
    string getName() const;

    categories getCategory() const;

    conditions getCondition() const;

    int getQuantity() const;

    int getPrice() const;

    User *getOwner() const;
};

class Notification {
private:
    string from_username;
    string to_username;
    Listing* listing_referred;
    notificationTypes type;

public:
    Notification(string from_username, string to_username, Listing *listing_referred, notificationTypes type);
    void printNotification();

    string getToUsername() const;

    string getFromUsername() const;

    notificationTypes getType() const;

    Listing *getListing() const;

    string getNotificationString() const;

//    void printNotification();

    bool printActions() const;
};

class User
{
private:
    string username;
    string password;
    int coins;
    vector<Item *> requested_items;
    vector<Listing *> listings;
    vector<Item *> borrowed_items;
    vector<Notification *> notifications;

public:
    User(string username, string password); // Create a user with 100 coins
    ~User(); // Destructor

    bool authenticate(string &entered_username, string &entered_password) const; // returns true if entered_username and entered_password are correct

    string getUsername() const;

    void listItem(Listing *listing);
    void removeListing(Listing *listing);

    void requestItem(Item *requested_item);
    void removeRequest(string &name);
    void borrowItem(Item *item);

    void printNotifications() const;

    void acceptCoins(int coins_given);
    bool spendCoins(int coins_to_spend);
    void addNotification(Notification *notification);
    void removeNotification(Notification *notification);
    void clearNotifications();
    int getCoinBalance() const;

    vector<Item*> getRequestedItems() const;

    Notification *getNotification(int notification_number) const;
    vector<string> getNotificationsString() const;
    void printBorrowedItems() const;

    int getNumberOfNotifications() const;

    void removeNotification(int notification_number);

    vector<Listing *> getListings() const;

    void removeRequest(int request_number);

    vector<Item *> getBorrowedItems() const;

    void returnItem(int borrowed_number);

    void returnItem(Item *borrowed_item);
};

class Manager
{
private:
    vector<User *> users;
    User *current_user{};
    vector<Listing *> listings;
    vector<Item *> requests;
    string currentDate;

public:
    Manager() = default;

    bool logged_in = false; // True if a user is logged in

    void registerUser(string username, string password); // Adds a new user to the users vector

    bool login(string &username, string &password); // updates current_user if valid username and password

    void logout(); // sets current_user to NULL

    string getListingDetails(int listing_number) const;
    string getRequestDetails(int request_number) const;
    vector<string> getListings() const;
    vector<string> getMyListings() const;
    vector<string> getRequests();
    vector<string> getNotificationString() const;
    bool editListing(int listing_number, string name, string category, int quantity, int price, string from_date, string to_date, string condition);
    vector<Listing*> searchListingsForRequest(Item* request);

    void printListings() const;
    void printNotifications() const;

    bool addListing(string name, string category, int quantity, int price,
                    string from_date, string to_date, string condition);
    void removeListing(string &name);
    bool addRequest(string name, string category, int quantity, string from_date, string to_date);
    // Adds a new request to the current_user's requested_items vector. It also searches through the listings vector of all users to find the item
    void removeRequest(string name);

//    vector<Listing *> searchListingsForRequest(Item *request); // Returns a vector of items that match the search criteria
    void borrowItem(Listing *item_to_borrow, User *borrower);     // Adds the item to the current_user's borrowed_items vector and removes it from the owner's listings vector

    void shareCoins(string &receiving_username, int coins_to_share); // Transfers coins_to_share from the current_user to the receiving_user
    void printRequests() const;

    vector<Item *> searchRequestsForListing(Listing *listing);

    void notifyRequestersAboutNewListing(Listing *new_listing);

    bool notifyUser(Notification *notificationToSend);

    void notifyRequesterOfNewRequest(Item *new_request);

    bool replyToNotification(int notification_number, string &action);

    bool printNotificationActions(int notification_number) const;

    void printBorrowedItems() const;

    int getBalance() const;

    int getNumberOfNotifications() const;

    void printMyListings() const;

    void printMyRequests() const;

    void removeRequest(int request_number);

    void requestOwnerToBorrow(int listing_number);

    bool userExists(string username) const;

    void returnItem(int borrowed_number);

    bool isNotCurrentUser(string username) const;

    bool removeListing(int listing_number);

    void setDate(string currentDate);

    void returnItemsIfOverdue();
};

#endif