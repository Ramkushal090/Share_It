#include "classes.h"

using namespace std;

categories stringToCategory(string &category) {
    if (category == "electronics") {
        return electronics;
    } else if (category == "furniture") {
        return furniture;
    } else if (category == "clothing") {
        return clothing;
    } else if (category == "books") {
        return books;
    } else if (category == "sports") {
        return sports;
    } else {
        return other;
    }
}

string categoryToString(categories category) {
    switch (category) {
        case electronics:
            return "electronics";
        case furniture:
            return "furniture";
        case clothing:
            return "clothing";
        case books:
            return "books";
        case sports:
            return "sports";
        default:
            return "other";
    }
}

conditions stringToCondition(string &condition) {
    if (condition == "excellent") {
        return excellent;
    } else if (condition == "good") {
        return good;
    } else if (condition == "fair") {
        return fair;
    } else if (condition == "poor") {
        return poor;
    } else {
        return INVALID;
    }
}

string conditionToString(conditions condition) {
    switch (condition) {
        case excellent:
            return "excellent";
        case good:
            return "good";
        case fair:
            return "fair";
        case poor:
            return "poor";
        default:
            return "INVALID";
    }
}

string typeToString(notificationTypes type) {
    switch (type) {
        case requestOwnerToBorrowItem:
            return "request";
        case acceptBorrower:
            return "accept";
        case rejectBorrower:
            return "reject";
        default:
            return "INVALID";
    }
}

notificationTypes stringToType(string &type) {
    if (type == "request") {
        return requestOwnerToBorrowItem;
    } else if (type == "accept") {
        return acceptBorrower;
    } else if (type == "reject") {
        return rejectBorrower;
    }
}

void printAvailableConditions() {
    cout << "Available conditions: excellent, good, fair, poor" << endl;
}

void printAvailableCategories() {
    cout << "Available categories: electronics, furniture, clothing, books, sports, other\n";
}

// Function to validate the date format and extract day, month, year
bool isValidDate(const string& date, int &day, int &month, int &year) {
    // Check if date matches the "DD-MM-YYYY" format
    regex date_pattern(R"(^(\d{1,2})-(\d{1,2})-(\d{4})$)");
    smatch match;

    if (regex_match(date, match, date_pattern)) {
        day = stoi(match[1]);
        month = stoi(match[2]);
        year = stoi(match[3]);

        // Basic validation for day, month, and year ranges
        if (year >= 1900 && year <= 4000 && month >= 1 && month <= 12) { // hoping this is used till the year 4000 XD
            // Check day validity based on month
            if ((day >= 1 && day <= 31) && (month == 1 || month == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month == 12)) {
                return true;
            } else if ((day >= 1 && day <= 30) && (month == 4 || month == 6 || month == 9 || month == 11)) {
                return true;
            } else if ((day >= 1 && day <= 28) && (month == 2)) {
                return true;
            } else if (day == 29 && month == 2 && ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0))) {
                return true; // Leap year check
            }
        }
    }
    return false;
}

// Function to compare two dates (returns -1 if date1 < date2, 0 if equal, 1 if date1 > date2)
int compareDates(const string& date1, const string& date2) {
    int day1, month1, year1, day2, month2, year2;
    isValidDate(date1, day1, month1, year1);
    isValidDate(date2, day2, month2, year2);

    if (year1 != year2) return year1 < year2 ? -1 : 1;
    if (month1 != month2) return month1 < month2 ? -1 : 1;
    if (day1 != day2) return day1 < day2 ? -1 : 1;

    return 0;
}

bool areValidateDates(const string& from_date, const string& to_date) {
    int day, month, year;
    if (!isValidDate(from_date, day, month, year)) {
        cout << "Invalid from_date format. Please use 'DD-MM-YYYY'." << endl;
        return false;
    }
    if (!isValidDate(to_date, day, month, year)) {
        cout << "Invalid to_date format. Please use 'DD-MM-YYYY'." << endl;
        return false;
    }
    if (compareDates(from_date, to_date) > 0) {
        cout << "from_date cannot be after to_date." << endl;
        return false;
    }
    return true;
}


