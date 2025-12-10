#include "classes.h"
#include <bits/stdc++.h>

using namespace std;

int main() {
    // getting current date
    time_t t = time(nullptr);
    tm* now = localtime(&t);
    string currentDate = to_string(now->tm_mday) + "-" + to_string(now->tm_mon + 1) + "-" + to_string(now->tm_year + 1900);
    Manager manager;
    manager.setDate(currentDate);

    string command;
    string username, password, action, name, category, condition, from_date, to_date;
    int quantity, price, notification_number, num_notifications, request_number, listing_number, coins, borrowed_number;
    bool isActionableNotifications;

    cout << "\nWelcome to the Inventory Management System!\n";
    cout << "Type 'help' to see available commands or 'exit' to quit.\n";
    cout << "-------------------------------------------------------\n";

    while (true) {
        cout << "\n>> ";
        getline(cin, command);

        if (command == "help") {
            // Define the commands and descriptions
            vector<pair<string, string>> commands;

            if (!manager.logged_in) {
                commands = {
                        {"login", "Log into your account"},
                        {"logout", "Log out of your account"},
                        {"register", "Create a new account"},
                        {"Exit/exit", "Exit the Shareit program"}
                };
            }
            else {
                commands = {
                        {"print listings", "View all available listings"},
                        {"print my listings", "View your listings"},
                        {"print requests", "View all current requests"},
                        {"print my requests", "View all current requests"},
                        {"print notifications", "View your notifications"},
                        {"add listing", "Add a new listing"},
                        {"remove listing", "Remove an existing listing"},
                        {"add request", "Request an item"},
                        {"remove request", "Remove a request"},
                        {"ask to borrow item", "Ask to borrow an item based on listing number"},
                        {"show borrowed items", "View all your borrowed items"},
                        {"return item", "Return a borrowed item"},
                        {"get balance", "Get your current balance"},
                        {"send coins", "Send coins to another user"}
                };
            }

            // Print the table header
            cout << "\n------------------------- Commands Available -------------------------\n";
            cout << left << setw(20) << "Command" << setw(50) << "Description" << endl;
            cout << string(70, '-') << endl;

            // Print the table rows
            for (const auto& [cmd, desc] : commands) {
                cout << left << setw(20) << cmd << setw(50) << desc << endl;
            }

            cout << string(70, '-') << endl;
        }
        else if (command == "exit" or command == "Exit") {
            cout << "Exiting the program. Goodbye!\n";
            break;
        }
        else if (!manager.logged_in) {
            if (command == "login") {
                cout << "Enter username: ";
                getline(cin, username);
                cout << "Enter password: ";
                getline(cin, password);
                manager.login(username, password);
            }
            else if (command == "logout") {
                cout << "You are not logged in.\n";
            }
            else if (command == "register") {
                cout << "Enter username: ";
                getline(cin, username);
                cout << "Enter password: ";
                getline(cin, password);
                manager.registerUser(username, password);
            }
            else {
                cout << "Invalid command! Type 'help' to see available commands.\n";
            }
        }
        else {
            if (command == "logout") {
                manager.logout();
            }
            else if (command == "print listings") {
                manager.printListings();
            }
            else if (command == "print my listings") {
                manager.printMyListings();
            }
            else if (command == "print requests") {
                manager.printRequests();
            }
            else if (command == "print my requests") {
                manager.printMyRequests();
            }
            else if (command == "print notifications") {
                manager.printNotifications();
                num_notifications = manager.getNumberOfNotifications();
                if (num_notifications == 0)
                    continue;

                cout << "\nWould you like to take any action on notifications? (yes/no): ";
                getline(cin, action);
                if (action == "no") continue;

                cout << "Enter the notification number to act on: ";
                getline(cin, command);  // Using command temporarily for input
                if (command.find_first_not_of("0123456789") != string::npos) {
                    cout << "Invalid notification number.\n";
                    continue;
                }
                notification_number = stoi(command);
                if (notification_number < 1 || notification_number > num_notifications) {
                    cout << "Invalid notification number.\n";
                    continue;
                }
                isActionableNotifications = manager.printNotificationActions(notification_number);
                if (!isActionableNotifications)
                    continue;

                cout << "Enter the action to take: ";
                getline(cin, action);
                manager.replyToNotification(notification_number, action);
            }
            else if (command == "add listing") {
                cout << "Enter item name: ";
                getline(cin, name);
                printAvailableCategories();
                cout << "Enter category: ";
                getline(cin, category);
                cout << "Enter quantity: ";
                getline(cin, command); // Using command temporarily for input
                if (command.find_first_not_of("0123456789") != string::npos) {
                    cout << "Invalid quantity.\n";
                    continue;
                }
                quantity = stoi(command);
                cout << "Enter from date (DD-MM-YYYY): ";
                getline(cin, from_date);
                cout << "Enter to date (DD-MM-YYYY): ";
                getline(cin, to_date);
                printAvailableConditions();
                cout << "Enter condition: ";
                getline(cin, condition);
                cout << "Enter price: ";
                getline(cin, command); // Using command temporarily for input
                if (command.find_first_not_of("0123456789") != string::npos) {
                    cout << "Invalid price.\n";
                    continue;
                }
                price = stoi(command);
                manager.addListing(name, category, quantity, price, from_date, to_date, condition);
            }
            else if (command == "remove listing") {
                cout << "Enter listing number to remove: ";
                getline(cin, command);
                if (command.find_first_not_of("0123456789") != string::npos) {
                    cout << "Invalid listing number.\n";
                    continue;
                }
                listing_number = stoi(command);
                manager.removeListing(listing_number);
            }
            else if (command == "add request") {
                cout << "Enter item name: ";
                getline(cin, name);
                cout << "Enter category: ";
                getline(cin, category);
                cout << "Enter quantity: ";
                getline(cin, command); // Using command temporarily for input
                if (command.find_first_not_of("0123456789") != string::npos) {
                    cout << "Invalid quantity.\n";
                    continue;
                }
                quantity = stoi(command);
                cout << "Enter from date (DD-MM-YYYY): ";
                getline(cin, from_date);
                cout << "Enter to date (DD-MM-YYYY): ";
                getline(cin, to_date);
                manager.addRequest(name, category, quantity, from_date, to_date);
            }
            else if (command == "remove request") {
                cout << "Enter request number to remove: ";
                getline(cin, command); // Using command temporarily for input
                if (command.find_first_not_of("0123456789") != string::npos) {
                    cout << "Invalid request number.\n";
                    continue;
                }
                request_number = stoi(command);
                manager.removeRequest(request_number);
            }
            else if (command == "ask to borrow item") {
                cout << "Enter listing number of listing you want to borrow: ";
                getline(cin, command); // Using command temporarily for input
                if (command.find_first_not_of("0123456789") != string::npos) {
                    cout << "Invalid listing number.\n";
                    continue;
                }
                listing_number = stoi(command);
                manager.requestOwnerToBorrow(listing_number);
            }
            else if (command == "show borrowed items") {
                manager.printBorrowedItems();
            }
            else if (command == "return item") {
                cout << "Enter borrowed item number to return: ";
                getline(cin, command); // Using command temporarily for input
                if (command.find_first_not_of("0123456789") != string::npos) {
                    cout << "Invalid borrowed item number.\n";
                    continue;
                }
                borrowed_number = stoi(command);
                manager.returnItem(borrowed_number);
            }
            else if (command == "get balance") {
                cout << "Your current balance is: " << manager.getBalance() << endl;
            }
            else if (command == "send coins") {
                cout << "Enter recipient's username: ";
                getline(cin, username);
                if (!manager.isNotCurrentUser(username)) {
                    cout << "Cannot send coins to yourself.\n";
                    continue;
                }
                if (!manager.userExists(username)) {
                    cout << "User '" << username << "' doesn't exist.\n";
                    continue;
                }
                cout << "Enter number of coins to send: ";
                getline(cin, command); // Using command temporarily for input
                // first we should check if command is integer or not, then use stoi function
                if (command.find_first_not_of("0123456789") != string::npos) {
                    cout << "Invalid number of coins to send.\n";
                    continue;
                }
                coins = stoi(command);
                manager.shareCoins(username, coins);
            }
            else if (command == "setDate") {
                cout << "Enter the current date (DD-MM-YYYY): ";
                getline(cin, command);
                manager.setDate(command);
                manager.returnItemsIfOverdue();
            }
            else {
                cout << "Invalid command! Type 'help' to see available commands.\n";
            }
        }
    }

    return 0;
}
