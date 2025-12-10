#include "classes.h"

using namespace std;

Notification::Notification(string from_username, string to_username, Listing *listing_referred,
                           notificationTypes type) :
        from_username(from_username), to_username(to_username), listing_referred(listing_referred),
        type(type) {}

string Notification::getToUsername() const {
    return to_username;
}

string Notification::getFromUsername() const {
    return from_username;
}

Listing *Notification::getListing() const {
    return listing_referred;
}

notificationTypes Notification::getType() const {
    return type;
}

string Notification::getNotificationString() const {
    if (type == requestOwnerToBorrowItem) {
        return "User " + from_username + " has requested to borrow " + listing_referred->getName() + ".";
    }
    else if (type == acceptBorrower) {
        return "User " + from_username + " has accepted your request to borrow " + listing_referred->getName() + ".";
    }
    else if (type == rejectBorrower) {
        return "User " + from_username + " has rejected your request to borrow " + listing_referred->getName() + ".";
    }
    return "";
}

void Notification::printNotification() {
    if (type == requestOwnerToBorrowItem) {
        cout << "User " << from_username << " has requested to borrow " << listing_referred->getName() << "." << endl;
    }
    else if (type == acceptBorrower) {
        cout << "User " << from_username << " has accepted your request to borrow " << listing_referred->getName() << "." << endl;
    }
    else if (type == rejectBorrower) {
        cout << "User " << from_username << " has rejected your request to borrow " << listing_referred->getName() << "." << endl;
    }
}

bool Notification::printActions() const {
    if (type == requestOwnerToBorrowItem) {
        cout << "yes: accept the requester to borrow " << this->listing_referred->getName() << endl;
        cout << "no: reject the requester to borrow " << this->listing_referred->getName() << endl;
        return true;
    }
    else if (type == acceptBorrower) {
        cout << "no actions available." << endl;
        return false;
    }
    else if (type == rejectBorrower) {
        cout << "no actions available." << endl;
        return false;
    }
    return false;
}
