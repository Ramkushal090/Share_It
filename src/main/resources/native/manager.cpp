#include "classes.h"
#include "nlohmann/json.hpp"
#include <fstream>

using namespace std;
using json = nlohmann::json;

json data;
json users_data;

void Manager::setDate(string currentDate) {
    int day, month, year;
    if (isValidDate(currentDate, day, month, year)) {
        this->currentDate = currentDate;
    } else {
        cout << "Invalid date format. Please use 'DD-MM-YYYY'." << endl;
    }
}

void Manager::registerUser(string username, string password) {
    // Load existing users from the file
    json users_data;
    ifstream user_file("../demo1/src/main/resources/native/user.json");
    if (user_file.is_open()) {
        user_file >> users_data;  // Load JSON data from file
        user_file.close();
    }

    // Check if user already exists (both in memory and JSON file)
    bool user_exists = false;
    for (User* existing_user : users) {
        if (existing_user->getUsername() == username) {
            user_exists = true;
            cout << "User with username '" << username << "' already exists. Try another username." << endl;
            return;
        }
    }
    for (auto& existing_user : users_data) {
        if (existing_user["name"] == username) {
            user_exists = true;
            cout << "User with username '" << username << "' already exists. Try another username." << endl;
            return;
        }
    }

    // If user does not exist, create a new user and save it
    if (!user_exists) {
        User* new_user = new User(username, password);
        this->users.push_back(new_user);

        // Add new user to JSON
        json new_user_json = {
            {"name", username},
            {"password", password}
        };
        users_data.push_back(new_user_json);

        // Save updated JSON data back to the file
        ofstream out_file("../demo1/src/main/resources/native/user.json");
        if (out_file.is_open()) {
            out_file << users_data.dump(4);  // Pretty-print JSON with 4-space indentation
            out_file.close();
        } else {
            cout << "Error: Unable to save user data to users.json." << endl;
        }

        cout << "New user '" << username << "' is created! Login to access your account." << endl;
    }
}



bool Manager::login(string &username, string &password) {
    // Load existing users from the JSON file
    json users_data;
    ifstream user_file("../demo1/src/main/resources/native/user.json");
    if (user_file.is_open()) {
        user_file >> users_data;  // Load JSON data
        user_file.close();
    } else {
        cout << "Error: Unable to open users.json for reading." << endl;
        return false;
    }

    // Authenticate against users in memory
    for (User* user : this->users) {
        if (user->authenticate(username, password)) {
            this->current_user = user;
            this->logged_in = true;
            cout << "Successfully logged in as " << username << "." << endl;
            return true;
        }
    }

    // Authenticate against users in the JSON file
    for (auto& existing_user : users_data) {
        if (existing_user["name"] == username && existing_user["password"] == password) {
            // Create a new User object for the logged-in user and store it in memory
            User* logged_in_user = new User(username, password);
            this->users.push_back(logged_in_user);
            this->current_user = logged_in_user;
            this->logged_in = true;

            cout << "Successfully logged in as " << username << "." << endl;
            return true;
        }
    }

    cout << "Invalid login credentials." << endl;
    return false;
}

void Manager::logout() {
    this->current_user = nullptr;
    this->logged_in = false;
    cout << "Logged out." << endl;
}

string Manager::getListingDetails(int listing_number) const {
    if (listing_number < 1 || listing_number > this->listings.size()) {
    cout << listing_number << endl;
        return "Invalid listing number.";
    }
    Listing* listing = this->listings[listing_number - 1];
    string itemDetails = listing->getItem()->getName() + "|" + conditionToString(listing->getCondition()) + "|" +
                   to_string(listing->getPrice()) + "|" + to_string(listing->getItem()->getQuantity()) + "|" +
                   listing->getOwner()->getUsername() + "|" + listing->getItem()->getFromDate() + "|" +
                   listing->getItem()->getToDate();
    return itemDetails;
}

string Manager::getRequestDetails(int request_number) const {
    if (request_number < 1 || request_number > this->requests.size()) {
        return "Invalid request number.";
    }
    Item* request = this->requests[request_number - 1];
    string itemDetails = request->getName() + "|" + categoryToString(request->getCategory()) + "|" +
                   to_string(request->getQuantity()) + "|" + request->getOwner()->getUsername() + "|" +
                   request->getFromDate() + "|" + request->getToDate();
    return itemDetails;
}

vector<string> Manager::getListings() const {
    vector<string> listings;
    for (Listing* listing: this->listings) {
        string toAdd = listing->getItem()->getName() + "|" + conditionToString(listing->getCondition()) + "|" +
                        to_string(listing->getPrice()) + "|" + to_string(listing->getItem()->getQuantity()) + "|" +
                        listing->getOwner()->getUsername() + "|" + listing->getItem()->getFromDate() + "|" +
                        listing->getItem()->getToDate();
        listings.push_back(toAdd);
    }
    return listings;
}

void Manager::printListings() const {
    cout << "Listings (Total: " << this->listings.size() << ")" << endl;
    cout << separator << endl;
    int listing_number = 0;
    for (Listing* listing: this->listings) {
        cout << "Listing " << ++listing_number << ":" << endl;
        listing->printItem();
        cout << separator << endl;
    }
}

vector<string> Manager::getMyListings() const {
    vector<string> my_listings;
    for (Listing* listing: this->current_user->getListings()) {
        // string format is "Item1|New|100|5|John Doe|2024-01-01|2024-12-31"
        string toAdd = listing->getItem()->getName() + "|" + conditionToString(listing->getCondition()) + "|" +
                        to_string(listing->getPrice()) + "|" + to_string(listing->getItem()->getQuantity()) + "|" +
                        listing->getOwner()->getUsername() + "|" + listing->getItem()->getFromDate() + "|" +
                        listing->getItem()->getToDate();
        my_listings.push_back(toAdd);
    }
    return my_listings;
}

void Manager::printMyListings() const {
    // Load listings from the JSON file
    json listing_data;
    ifstream data_file("../data.json");
    if (data_file.is_open()) {
        try {
            data_file >> listing_data;
        } catch (const std::exception& e) {
            cout << "Error: Failed to parse JSON data. " << e.what() << endl;
            return;
        }
        data_file.close();
    } else {
        cout << "Error: Unable to open data.json for reading." << endl;
        return;
    }

    // Check if the current user has listings
    string username = current_user->getUsername();
    if (!listing_data.contains(username) || listing_data[username]["listings"].empty()) {
        cout << "No listings found for user: " << username << endl;
        return;
    }

    // Print listings
    const auto& user_listings = listing_data[username]["listings"];
    cout << "Listings for user: " << username << " (Total: " << user_listings.size() << ")" << endl;
    cout << separator << endl;

    int listing_number = 0;
    for (const auto& listing : user_listings) {
        cout << "Listing " << ++listing_number << ":" << endl;
        cout << "Name: " << listing["name"] << endl;
        cout << "Category: " << listing["category"] << endl;
        cout << "Quantity: " << listing["quantity"] << endl;
        cout << "Price: " << listing["price"] << endl;
        cout << "From Date: " << listing["from_date"] << endl;
        cout << "To Date: " << listing["to_date"] << endl;
        cout << "Condition: " << listing["condition"] << endl;
        cout << separator << endl;
    }
}

vector<string> Manager::getRequests() {
    // string format is "Request1|Urgent|200|1|John Doe|2024-03-01|2024-03-15"
    vector<string> requests;
    for (Item* request: this->requests) {
        string toAdd = request->getName() + "|" + categoryToString(request->getCategory()) + "|" +
                        to_string(request->getQuantity()) + "|" + request->getOwner()->getUsername() + "|" +
                        request->getFromDate() + "|" + request->getToDate();
        requests.push_back(toAdd);
    }
    return requests;
}

void Manager::printRequests() const {
    cout << "Requests (Total: " << this->requests.size() << ")" << endl;
    cout << separator << endl;
    int request_number = 0;
    for (Item *item: this->requests) {
        cout << "Request " << ++request_number << ":" << endl;
        item->printItem();
        cout << separator << endl;
    }
}

void Manager::printMyRequests() const {
    cout << "Requests (Total: " << this->current_user->getRequestedItems().size() << ")" << endl;
    cout << separator << endl;
    int request_number = 0;
    for (Item* item: this->current_user->getRequestedItems()) {
        cout << "Request " << ++request_number << ":" << endl;
        item->printItem();
        cout << separator << endl;
    }
}

vector<string> Manager::getNotificationString() const {
    return this->current_user->getNotificationsString();
}

void Manager::printNotifications() const {
    current_user->printNotifications();
}

int Manager::getNumberOfNotifications() const {
    return current_user->getNumberOfNotifications();
}

bool Manager::printNotificationActions(int notification_number) const {
    return current_user->getNotification(notification_number - 1)->printActions();
}

bool Manager::addListing(string name, string category, int quantity, int price,
                         string from_date, string to_date, string condition) {
    conditions condition_enum = stringToCondition(condition);
    categories category_enum = stringToCategory(category);

    if (condition_enum == INVALID) {
        cout << "Invalid condition. Please use 'excellent', 'good', 'fair', or 'poor'." << endl;
        return false;
    }
    if (!areValidateDates(from_date, to_date)) {
        return false;
    }
    if (quantity <= 0) {
        cout << "Quantity must be positive." << endl;
        return false;
    }
    if (price <= 0) {
        cout << "Price must be positive." << endl;
        return false;
    }

    json listing_data;
    ifstream data_file("../demo1/src/main/resources/native/data.json");
    if (data_file.is_open()) {
        try {
            data_file >> listing_data;
        } catch (const std::exception& e) {
            listing_data = json::object();
        }
        data_file.close();
    } else {
        // Initialize empty JSON if file does not exist
        listing_data = json::object();
    }

    Item* new_item = new Item(name, category_enum, quantity, from_date, to_date, current_user);
    Listing* new_listing = new Listing(new_item, price, condition_enum);
    this->listings.push_back(new_listing);
    this->current_user->listItem(new_listing);

    // Add new listing to JSON
    json listing = {
        {"name", name},
        {"category", category},
        {"quantity", quantity},
        {"price", price},
        {"from_date", from_date},
        {"to_date", to_date},
        {"condition", condition}
    };

    listing_data[current_user->getUsername()]["listings"].push_back(listing);

    ofstream out_file("../demo1/src/main/resources/native/data.json");
    if (out_file.is_open()) {
        out_file << listing_data.dump(4);  // Pretty-print JSON
        out_file.close();
        cout << "Item " << name << " is now listed! " << endl;
    }

    notifyRequestersAboutNewListing(new_listing);
    return true;
}

bool Manager::editListing(int listing_number, string name, string category, int quantity, int price,
                          string from_date, string to_date, string condition) {
    if (listing_number < 1 || listing_number > this->current_user->getListings().size()) {
        cout << "Invalid listing number." << endl;
        return false;
    }
    // delete the old listing and add the new listing at the same index
    Listing *listing_to_edit = this->current_user->getListings()[listing_number - 1];
    if (!listing_to_edit->isAvailable()) {
        cout << "Item " << listing_to_edit->getItem()->getName() << " is currently borrowed. Cannot edit listing." << endl;
        return false;
    }
    this->current_user->removeListing(listing_to_edit);
    for (int i = 0; i < this->listings.size(); i++) {
        if (this->listings[i] == listing_to_edit) {
            this->listings.erase(this->listings.begin() + i);
            break;
        }
    }
    Listing* new_listing = new Listing(new Item(name, stringToCategory(category), quantity, from_date, to_date, current_user),
                                       price, stringToCondition(condition));
    this->current_user->listItem(new_listing);
    this->listings.insert(this->listings.begin() + listing_number - 1, new_listing);
    return true;
}

void Manager::removeListing(string &name) {
    Listing* listing_to_del = nullptr;
    for (int i = 0; i < this->listings.size(); i++) {
        if (this->listings[i]->getName() == name) {
            listing_to_del = this->listings[i];
            if (!listing_to_del->isAvailable()) {
                cout << "Item " << name << " is currently borrowed. Cannot remove listing." << endl;
                return;
            }
            this->listings.erase(this->listings.begin() + i);
            break;
        }
    }
    if (listing_to_del == nullptr) {
        cout << "Item " << name << " not listed." << endl;
    } else {
        this->current_user->removeListing(listing_to_del);  // Takes care of deleting the listing object as well
        listing_to_del = nullptr;
        cout << "Item " << name << " removed listing." << endl;
    }
}

bool Manager::removeListing(int listing_number) {
    if (listing_number < 1 || listing_number > this->current_user->getListings().size()) {
        cout << "Invalid listing number." << endl;
        return false;
    }
    Listing *listing_to_del = this->current_user->getListings()[listing_number - 1];
    if (!listing_to_del->isAvailable()) {
        cout << "Item " << listing_to_del->getName() << " is currently borrowed. Cannot remove listing." << endl;
        return false;
    }
    this->current_user->removeListing(listing_to_del);
    for (int i = 0; i < this->listings.size(); i++) {
        if (this->listings[i] == listing_to_del) {
            this->listings.erase(this->listings.begin() + i);
            break;
        }
    }
    cout << "Listing removed." << endl;
    return true;
}

bool Manager::addRequest(string name, string category, int quantity, string from_date, string to_date) {
    categories category_enum = stringToCategory(category);
    if (!areValidateDates(from_date, to_date)) {
        return false;
    }
    if (quantity <= 0) {
        cout << "Quantity must be positive." << endl;
        return false;
    }

    Item* new_request = new Item(name, category_enum, quantity, from_date, to_date,
                                 nullptr, current_user);
    this->current_user->requestItem(new_request);
    requests.push_back(new_request);

    json request = {
        {"name", name},
        {"category", category},
        {"quantity", quantity},
        {"from_date", from_date},
        {"to_date", to_date}
    };

    data[current_user->getUsername()]["requests"].push_back(request);

    notifyRequesterOfNewRequest(new_request);
    return true;
}

void Manager::removeRequest(string name) {
    this->current_user->removeRequest(name);
}

void Manager::removeRequest(int request_number) {
    if (request_number < 1 || request_number > this->current_user->getRequestedItems().size()) {
        cout << "Invalid request number." << endl;
        return;
    }
    // we need to remove this request from manager's requests as well
    Item *request_to_del = this->current_user->getRequestedItems()[request_number - 1];
    for (int i = 0; i < this->requests.size(); i++) {
        if (this->requests[i] == request_to_del) {
            this->requests.erase(this->requests.begin() + i);
            break;
        }
    }
    this->current_user->removeRequest(request_number - 1);
    cout << "Request removed." << endl;
}

void Manager::printBorrowedItems() const {
    current_user->printBorrowedItems();
}

vector<Listing*> Manager::searchListingsForRequest(Item* request) {
    vector<Listing*> search_results;
    for (Listing* listing: this->listings) {
        if (listing->isAvailable() &&     // Item is available
            listing->getName() == request->getName() &&     // Item name matches
            listing->getCategory() == request->getCategory() &&     // Item category matches
            listing->getQuantity() >= request->getQuantity() &&     // Item quantity is enough
            compareDates(listing->getItem()->getFromDate(), request->getFromDate()) <= 0 &&    // From date is before or equal
            compareDates(listing->getItem()->getToDate(), request->getToDate()) >= 0    // To date is after or equal
            )
        {
            search_results.push_back(listing);
        }
    }
    return search_results;
}

vector<Item*> Manager::searchRequestsForListing(Listing *listing) {
    vector<Item*> search_results;
    for (Item *request: this->requests) {
        if (listing->getName() == request->getName() &&     // Item name matches
            listing->getCategory() == request->getCategory() &&     // Item category matches
            listing->getQuantity() >= request->getQuantity() &&     // Item quantity is enough
            compareDates(listing->getItem()->getFromDate(), request->getFromDate()) <= 0 &&    // From date is before or equal
            compareDates(listing->getItem()->getToDate(), request->getToDate()) >= 0    // To date is after or equal
            )
        {
            search_results.push_back(request);
        }
    }
    return search_results;
}

// when a new listing is made, we get a list of requests that match the listing and notify the requesters
// when a new request is made, we make a list of listings that match the request and notify the requesters
// once requester accepts the request, we notify the owner of the listing
// once the owner of the listing accepts, requester borrows the item
// once the requester returns the item, the owner gets the item back

bool Manager::notifyUser(Notification *notificationToSend) {
    for (User* user: users) {
        if (user->getUsername() == notificationToSend->getToUsername()) {
            user->addNotification(notificationToSend);
            return true;
        }
    }
    return false;
}

void Manager::requestOwnerToBorrow(int listing_number) {
    if (listing_number < 1 || listing_number > this->listings.size()) {
        cout << "Invalid listing number." << endl;
        return;
    }
    Listing *listing = this->listings[listing_number - 1];
    if (!listing->isAvailable()) {
        cout << "Listing is not available." << endl;
        return;
    }
    if (listing->getOwner()->getUsername() == current_user->getUsername()) {
        cout << "You cannot borrow your own item." << endl;
        return;
    }
    if (current_user->getCoinBalance() < listing->getPrice()) {
        cout << "You don't have enough coins to borrow this item." << endl;
        return;
    }
    Notification *notification = new Notification(current_user->getUsername(), listing->getOwner()->getUsername(),
                                                  listing, requestOwnerToBorrowItem);
    notifyUser(notification);
    cout << "Request sent to owner." << endl;
}

void Manager::notifyRequestersAboutNewListing(Listing *new_listing) {
    vector<Item*> search_results = searchRequestsForListing(new_listing);
    for (Item *request: search_results) {
        Notification *notification = new Notification(new_listing->getOwner()->getUsername(), request->getOwner()->getUsername(),
                                                 new_listing, requestOwnerToBorrowItem);
        notifyUser(notification);
    }
}

void Manager::notifyRequesterOfNewRequest(Item *new_request) {
    vector<Listing*> search_results = searchListingsForRequest(new_request);
    cout << "Found " << search_results.size() << " listings that match the request." << endl;
    string from_username = new_request->getBorrower()->getUsername();
    for (Listing *listing: search_results) {
        cout << "Notifying " << listing->getOwner()->getUsername() << " about the request." << endl;
        string owner_username = listing->getOwner()->getUsername();
        Notification *notification = new Notification(from_username, owner_username, listing,
                                                      requestOwnerToBorrowItem);
        notifyUser(notification);
    }
}

bool Manager::replyToNotification(int notification_number, string &action) {
    Notification *notification = current_user->getNotification(notification_number - 1);
    // we need to delete the notification from user's notification now
    if (notification->getType() != requestOwnerToBorrowItem) {
        current_user->removeNotification(notification_number - 1);
        return false;
    }
    // notification is a request from borrower to owner to borrow an item
    if (action == "yes") {
        // accept the requester
        Listing *listing = notification->getListing();
        User* borrower;
        for (User* user: users) {
            if (user->getUsername() == notification->getFromUsername()) {
                borrower = user;
                break;
            }
        }
        if (listing->isAvailable()) {
            if (borrower->getCoinBalance() < listing->getPrice()) {
                cout << "Requester doesn't have enough coins to borrow the item." << endl;
                return false;
            }
            borrowItem(listing, borrower);
            Notification *reply_notification = new Notification(current_user->getUsername(), notification->getFromUsername(),listing,acceptBorrower);
            notifyUser(reply_notification);
        }
        else {
            cout << "Item is no longer available." << endl;
            current_user->removeNotification(notification_number - 1);
            return false;
        }
    } else if (action == "no") {
        // reject the requester
        Notification *reply_notification = new Notification(current_user->getUsername(), notification->getFromUsername(),
                                                            notification->getListing(), rejectBorrower);
        notifyUser(reply_notification);
    }
    current_user->removeNotification(notification_number - 1);
    return true;
}


void Manager::borrowItem(Listing *item_to_borrow, User *borrower) {
//    User* item_owner = item_to_borrow->getOwner();
    item_to_borrow->bookItem();
    borrower->borrowItem(item_to_borrow->getItem());
    borrower->spendCoins(item_to_borrow->getPrice());
    current_user->acceptCoins(item_to_borrow->getPrice());
    cout << "Item borrowed successfully." << endl;
}

void Manager::returnItem(int borrowed_number) {
    if (borrowed_number < 1 || borrowed_number > current_user->getBorrowedItems().size()) {
        cout << "Invalid borrowed item number." << endl;
        return;
    }
    Item* borrowed_item = current_user->getBorrowedItems()[borrowed_number - 1];
    User* item_owner = borrowed_item->getOwner();
    Listing* listing = nullptr;
    for (Listing* item_listing: item_owner->getListings()) {
        if (item_listing->getItem() == borrowed_item) {
            listing = item_listing;
            break;
        }
    }
    if (listing == nullptr) {
        cout << "Error: borrowed item not found in owner's listings." << endl;
        return;
    }
    listing->freeItem();
    current_user->returnItem(borrowed_number - 1);
    cout << "Item returned successfully." << endl;
}

void Manager::shareCoins(string &receiving_username, int coins_to_share) {
    // Assumed that a valid coins_to_share amount is given a.k.a 0 < coins_to_share <= user->coins
    bool shared = false;
    if (coins_to_share <= 0) {
        cout << "Invalid number of coins to share." << endl;
        return;
    }
    if (coins_to_share > current_user->getCoinBalance()) {
        cout << "You don't have enough coins to share." << endl;
        return;
    }
    for (User* user: users) {
        if (user->getUsername() == receiving_username) {
            current_user->spendCoins(coins_to_share);
            user->acceptCoins(coins_to_share);
            shared = true;
            cout << "Coins shared successfully." << endl;
            break;
        }
    }
    if (!shared)
        cout << "Transaction failed: user '" << receiving_username << "' doesn't exist." << endl;
}

int Manager::getBalance() const {
    return current_user->getCoinBalance();
}

bool Manager::isNotCurrentUser(string username) const {
    return current_user->getUsername() != username;
}

bool Manager::userExists(string username) const {
    return std::any_of(users.begin(), users.end(), [&](const User* user) {
        return user->getUsername() == username;
    });
}

void Manager::returnItemsIfOverdue() {
    for (User* user: users) {
        for (Item* borrowed_item: user->getBorrowedItems()) {
            if (compareDates(currentDate, borrowed_item->getToDate()) > 0) {
                Listing* listing = nullptr;
                for (Listing* item_listing: user->getListings()) {
                    if (item_listing->getItem() == borrowed_item) {
                        listing = item_listing;
                        break;
                    }
                }
                if (listing == nullptr) {
                    cout << "Error: borrowed item not found in owner's listings." << endl;
                    return;
                }
                listing->freeItem();
                user->returnItem(borrowed_item);
                cout << "Item " << borrowed_item->getName() << " returned due to overdue." << endl;
            }
        }
    }
}
