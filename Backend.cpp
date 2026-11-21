#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <queue>
#include <stack>
#include <string>
#include <limits>
#include <algorithm>
using namespace std;

// =============================================
// RAILWAY / BUS TICKET RESERVATION SYSTEM
// C++17 + DSA
// =============================================

// STRUCTURES

struct Journey {
    int id, fare, totalSeats;
    string type, name, from, to;
};

struct User {
    string username, password, fullName;
};

struct Booking {
    int pnr, journeyId, seatCount, totalFare;
    string username, passenger;
    bool active;
};

struct WaitingRequest {
    string username, passenger;
    int journeyId, seatCount;
};

// GLOBAL VARIABLES

vector<Journey> journeys;
vector<User> users;
vector<Booking> bookings;

queue<WaitingRequest> waitingList;
stack<string> recentHistory;

int nextPNR = 1001;
int nextJourneyID = 7;

// INPUT FUNCTIONS

string readText(string message) {
    string value;

    while (true) {
        cout << message;
        getline(cin, value);

        if (!value.empty() && value.find('|') == string::npos)
            return value;

        cout << "Invalid input. Please try again.\n";
    }
}

int readInt(string message) {
    string line;
    int value;
    char extra;

    while (true) {
        cout << message;
        getline(cin, line);

        stringstream ss(line);

        if (ss >> value && !(ss >> extra))
            return value;

        cout << "Please enter a valid number.\n";
    }
}

// SEARCH FUNCTIONS

int findJourney(int id) {
    for (int i = 0; i < (int)journeys.size(); i++) {
        if (journeys[i].id == id)
            return i;
    }
    return -1;
}

int findUser(string username) {
    for (int i = 0; i < (int)users.size(); i++) {
        if (users[i].username == username)
            return i;
    }
    return -1;
}

int findBooking(int pnr) {
    for (int i = 0; i < (int)bookings.size(); i++) {
        if (bookings[i].pnr == pnr && bookings[i].active)
            return i;
    }
    return -1;
}

// AVAILABLE SEATS

int availableSeats(int journeyId) {
    int index = findJourney(journeyId);

    if (index == -1)
        return 0;

    int bookedSeats = 0;

    for (const auto &b : bookings) {
        if (b.journeyId == journeyId && b.active)
            bookedSeats += b.seatCount;
    }

    return journeys[index].totalSeats - bookedSeats;
}

// FILE HANDLING

void saveJourneys() {
    ofstream file("journeys.txt");

    for (const auto &j : journeys) {
        file << j.id << "|"
             << j.fare << "|"
             << j.totalSeats << "|"
             << j.type << "|"
             << j.name << "|"
             << j.from << "|"
             << j.to << "\n";
    }
}

void saveUsers() {
    ofstream file("users.txt");

    for (const auto &u : users) {
        file << u.username << "|"
             << u.password << "|"
             << u.fullName << "\n";
    }
}

void saveBookings() {
    ofstream file("bookings.txt");

    for (const auto &b : bookings) {
        file << b.pnr << "|"
             << b.journeyId << "|"
             << b.seatCount << "|"
             << b.totalFare << "|"
             << b.username << "|"
             << b.passenger << "|"
             << b.active << "\n";
    }
}

void saveWaitingList() {
    ofstream file("waiting.txt");
    queue<WaitingRequest> temp = waitingList;

    while (!temp.empty()) {
        WaitingRequest w = temp.front();
        temp.pop();

        file << w.username << "|"
             << w.passenger << "|"
             << w.journeyId << "|"
             << w.seatCount << "\n";
    }
}

void loadJourneys() {
    ifstream file("journeys.txt");
    string line;

    while (getline(file, line)) {
        try {
            stringstream ss(line);
            string part;
            Journey j;

            getline(ss, part, '|');
            j.id = stoi(part);

            getline(ss, part, '|');
            j.fare = stoi(part);

            getline(ss, part, '|');
            j.totalSeats = stoi(part);

            getline(ss, j.type, '|');
            getline(ss, j.name, '|');
            getline(ss, j.from, '|');
            getline(ss, j.to);

            if (j.id > 0 && j.fare > 0 &&
                j.totalSeats > 0 && findJourney(j.id) == -1) {
                journeys.push_back(j);
                nextJourneyID = max(nextJourneyID, j.id + 1);
            }
        }
        catch (...) {
            cout << "Invalid journey record skipped.\n";
        }
    }

    if (journeys.empty()) {
        journeys = {
            {1, 450, 40, "Train", "Shatabdi", "Delhi", "Dehradun"},
            {2, 300, 35, "Train", "JanShatabdi", "Haridwar", "Delhi"},
            {3, 550, 25, "Bus", "Volvo", "Roorkee", "Delhi"},
            {4, 120, 30, "Bus", "OrdinaryBus", "Dehradun", "Haridwar"},
            {5, 380, 20, "Train", "MussoorieExp", "Dehradun", "Delhi"},
            {6, 250, 32, "Bus", "ExpressBus", "Delhi", "Haridwar"}
        };

        saveJourneys();
    }
}

void loadUsers() {
    ifstream file("users.txt");
    string line;

    while (getline(file, line)) {
        stringstream ss(line);
        User u;

        getline(ss, u.username, '|');
        getline(ss, u.password, '|');
        getline(ss, u.fullName);

        if (!u.username.empty() &&
            !u.password.empty() &&
            findUser(u.username) == -1) {
            users.push_back(u);
        }
    }
}

void loadBookings() {
    ifstream file("bookings.txt");
    string line;

    while (getline(file, line)) {
        try {
            stringstream ss(line);
            string part;
            Booking b;

            getline(ss, part, '|');
            b.pnr = stoi(part);

            getline(ss, part, '|');
            b.journeyId = stoi(part);

            getline(ss, part, '|');
            b.seatCount = stoi(part);

            getline(ss, part, '|');
            b.totalFare = stoi(part);

            getline(ss, b.username, '|');
            getline(ss, b.passenger, '|');
            getline(ss, part);

            b.active = (part == "1");

            if (b.pnr > 0 && b.seatCount > 0 &&
                findJourney(b.journeyId) != -1) {
                bookings.push_back(b);
                nextPNR = max(nextPNR, b.pnr + 1);
            }
        }
        catch (...) {
            cout << "Invalid booking record skipped.\n";
        }
    }
}

void loadWaitingList() {
    ifstream file("waiting.txt");
    string line;

    while (getline(file, line)) {
        try {
            stringstream ss(line);
            string part;
            WaitingRequest w;

            getline(ss, w.username, '|');
            getline(ss, w.passenger, '|');

            getline(ss, part, '|');
            w.journeyId = stoi(part);

            getline(ss, part);
            w.seatCount = stoi(part);

            int index = findJourney(w.journeyId);

            if (index != -1 &&
                w.seatCount > 0 &&
                w.seatCount <= journeys[index].totalSeats) {
                waitingList.push(w);
            }
        }
        catch (...) {
            cout << "Invalid waiting-list record skipped.\n";
        }
    }
}

// DISPLAY JOURNEYS

void showJourneys() {
    cout << "\n========== AVAILABLE JOURNEYS ==========\n";

    for (const auto &j : journeys) {
        cout << "\nID: " << j.id
             << "\nType: " << j.type
             << "\nName: " << j.name
             << "\nRoute: " << j.from << " to " << j.to
             << "\nFare: Rs." << j.fare
             << "\nAvailable Seats: " << availableSeats(j.id)
             << "\n";
    }

    if (journeys.empty())
        cout << "No journeys available.\n";
}

// SEARCH JOURNEY

void searchJourney() {
    string source = readText("Enter source: ");
    string destination = readText("Enter destination: ");

    bool found = false;

    for (const auto &j : journeys) {
        if (j.from == source && j.to == destination) {
            cout << "\nID: " << j.id
                 << " | " << j.name
                 << " | Fare: Rs." << j.fare
                 << " | Seats: " << availableSeats(j.id) << "\n";

            found = true;
        }
    }

    if (!found)
        cout << "No matching journey found.\n";
}

// BUBBLE SORT

void sortByFare() {
    vector<Journey> temp = journeys;

    for (int i = 0; i < (int)temp.size() - 1; i++) {
        for (int j = 0; j < (int)temp.size() - i - 1; j++) {
            if (temp[j].fare > temp[j + 1].fare) {
                swap(temp[j], temp[j + 1]);
            }
        }
    }

    cout << "\n========== SORTED JOURNEYS ==========\n";

    for (const auto &j : temp) {
        cout << j.name << " | Rs." << j.fare
             << " | Seats: " << availableSeats(j.id) << "\n";
    }
}

// USER REGISTRATION

void registerUser() {
    User u;

    cout << "\n========== USER REGISTRATION ==========\n";

    u.fullName = readText("Enter full name: ");
    u.username = readText("Create username: ");

    if (u.username == "admin" || findUser(u.username) != -1) {
        cout << "Username already exists or is reserved.\n";
        return;
    }

    u.password = readText("Create password: ");

    users.push_back(u);
    saveUsers();

    cout << "Registration successful!\n";
}

// USER LOGIN

bool userLogin(string &loggedUser) {
    string username = readText("Enter username: ");
    string password = readText("Enter password: ");

    int index = findUser(username);

    if (index != -1 && users[index].password == password) {
        loggedUser = username;
        cout << "Login successful!\n";
        return true;
    }

    cout << "Invalid username or password.\n";
    return false;
}

// ADMIN LOGIN

bool adminLogin() {
    string username = readText("Admin username: ");
    string password = readText("Admin password: ");

    if (username == "admin" && password == "1234") {
        cout << "Admin login successful!\n";
        return true;
    }

    cout << "Invalid admin credentials.\n";
    return false;
}

// ADD JOURNEY

void addJourney() {
    Journey j;

    cout << "\n========== ADD JOURNEY ==========\n";

    j.type = readText("Enter type (Train/Bus): ");

    if (j.type != "Train" && j.type != "Bus") {
        cout << "Enter Train or Bus only.\n";
        return;
    }

    j.name = readText("Enter journey name: ");
    j.from = readText("Enter source: ");
    j.to = readText("Enter destination: ");

    if (j.from == j.to) {
        cout << "Source and destination cannot be same.\n";
        return;
    }

    j.fare = readInt("Enter fare: ");
    j.totalSeats = readInt("Enter total seats: ");

    if (j.fare <= 0 || j.totalSeats <= 0) {
        cout << "Invalid fare or seat count.\n";
        return;
    }

    j.id = nextJourneyID++;
    journeys.push_back(j);

    saveJourneys();

    cout << "Journey added successfully. ID: " << j.id << "\n";
}

// UPDATE JOURNEY

void updateJourney() {
    int id = readInt("Enter journey ID: ");
    int index = findJourney(id);

    if (index == -1) {
        cout << "Journey not found.\n";
        return;
    }

    int fare = readInt("Enter new fare: ");
    int seats = readInt("Enter new total seats: ");

    int booked = journeys[index].totalSeats -
                 availableSeats(id);

    if (fare <= 0 || seats < booked) {
        cout << "Invalid fare or capacity is below booked seats.\n";
        return;
    }

    journeys[index].fare = fare;
    journeys[index].totalSeats = seats;

    saveJourneys();

    cout << "Journey updated successfully.\n";
}

// DELETE JOURNEY

void deleteJourney() {
    int id = readInt("Enter journey ID: ");
    int index = findJourney(id);

    if (index == -1) {
        cout << "Journey not found.\n";
        return;
    }

    for (const auto &b : bookings) {
        if (b.journeyId == id && b.active) {
            cout << "Cannot delete: active bookings exist.\n";
            return;
        }
    }

    queue<WaitingRequest> temp;

    while (!waitingList.empty()) {
        WaitingRequest w = waitingList.front();
        waitingList.pop();

        if (w.journeyId != id)
            temp.push(w);
    }

    waitingList = temp;

    journeys.erase(journeys.begin() + index);

    saveJourneys();
    saveWaitingList();

    cout << "Journey deleted successfully.\n";
}

// CREATE BOOKING

void createBooking(string username, string passenger,
                   int journeyId, int count) {
    int index = findJourney(journeyId);

    if (index == -1 || count <= 0 ||
        count > availableSeats(journeyId)) {
        cout << "Booking not possible.\n";
        return;
    }

    Booking b;

    b.pnr = nextPNR++;
    b.journeyId = journeyId;
    b.seatCount = count;
    b.totalFare = count * journeys[index].fare;
    b.username = username;
    b.passenger = passenger;
    b.active = true;

    bookings.push_back(b);

    recentHistory.push("Ticket booked. PNR: " + to_string(b.pnr));

    saveBookings();

    cout << "\n========== BOOKING SUCCESSFUL ==========\n";
    cout << "PNR: " << b.pnr
         << "\nPassenger: " << b.passenger
         << "\nJourney: " << journeys[index].name
         << "\nSeats: " << b.seatCount
         << "\nTotal Fare: Rs." << b.totalFare << "\n";
}

// BOOK TICKET

void bookTicket(string username) {
    showJourneys();

    int id = readInt("Enter journey ID: ");
    int index = findJourney(id);

    if (index == -1) {
        cout << "Invalid journey ID.\n";
        return;
    }

    string passenger = readText("Enter passenger name: ");
    int count = readInt("Enter number of seats: ");

    if (count <= 0 || count > journeys[index].totalSeats) {
        cout << "Invalid number of seats.\n";
        return;
    }

    if (count <= availableSeats(id)) {
        createBooking(username, passenger, id, count);
    }
    else {
        cout << "Seats are currently unavailable.\n";

        int choice = readInt("Join waiting list? (1=Yes, 0=No): ");

        if (choice == 1) {
            waitingList.push({username, passenger, id, count});
            saveWaitingList();

            cout << "Added to waiting list successfully.\n";
        }
        else {
            cout << "Request cancelled.\n";
        }
    }
}

// PROCESS WAITING LIST

void processWaitingList(int journeyId) {
    queue<WaitingRequest> temp;
    int total = (int)waitingList.size();

    for (int i = 0; i < total; i++) {
        WaitingRequest w = waitingList.front();
        waitingList.pop();

        if (w.journeyId == journeyId &&
            w.seatCount <= availableSeats(journeyId)) {

            cout << "\nWaiting-list request processed for "
                 << w.username << ".\n";

            createBooking(w.username, w.passenger,
                          w.journeyId, w.seatCount);
        }
        else {
            temp.push(w);
        }
    }

    waitingList = temp;
    saveWaitingList();
}

// MY BOOKINGS

void myBookings(string username) {
    bool found = false;

    cout << "\n========== MY BOOKINGS ==========\n";

    for (const auto &b : bookings) {
        if (b.username == username && b.active) {
            int index = findJourney(b.journeyId);

            cout << "\nPNR: " << b.pnr
                 << "\nPassenger: " << b.passenger
                 << "\nSeats: " << b.seatCount
                 << "\nFare: Rs." << b.totalFare;

            if (index != -1)
                cout << "\nJourney: " << journeys[index].name;

            cout << "\n";
            found = true;
        }
    }

    if (!found)
        cout << "No active bookings found.\n";
}

// CANCEL TICKET

void cancelTicket(string username, bool isAdmin = false) {
    int pnr = readInt("Enter PNR to cancel: ");
    int index = findBooking(pnr);

    if (index == -1) {
        cout << "Active booking not found.\n";
        return;
    }

    if (!isAdmin && bookings[index].username != username) {
        cout << "You cannot cancel another user's ticket.\n";
        return;
    }

    int journeyId = bookings[index].journeyId;

    // Mark cancelled before processing waiting list
    bookings[index].active = false;

    recentHistory.push("Ticket cancelled. PNR: " + to_string(pnr));

    saveBookings();

    cout << "Ticket cancelled successfully.\n";

    processWaitingList(journeyId);
}

// ALL BOOKINGS

void showAllBookings() {
    bool found = false;

    cout << "\n========== ALL ACTIVE BOOKINGS ==========\n";

    for (const auto &b : bookings) {
        if (b.active) {
            cout << "\nPNR: " << b.pnr
                 << " | User: " << b.username
                 << " | Passenger: " << b.passenger
                 << " | Seats: " << b.seatCount
                 << " | Fare: Rs." << b.totalFare << "\n";

            found = true;
        }
    }

    if (!found)
        cout << "No active bookings.\n";
}

// WAITING LIST

void showWaitingList() {
    queue<WaitingRequest> temp = waitingList;

    cout << "\n========== WAITING LIST ==========\n";

    if (temp.empty()) {
        cout << "Waiting list is empty.\n";
        return;
    }

    while (!temp.empty()) {
        WaitingRequest w = temp.front();
        temp.pop();

        cout << "User: " << w.username
             << " | Passenger: " << w.passenger
             << " | Journey ID: " << w.journeyId
             << " | Seats: " << w.seatCount << "\n";
    }
}

// RECENT HISTORY USING STACK

void showRecentHistory() {
    stack<string> temp = recentHistory;

    cout << "\n========== RECENT HISTORY ==========\n";

    if (temp.empty()) {
        cout << "No activity in this session.\n";
        return;
    }

    while (!temp.empty()) {
        cout << temp.top() << "\n";
        temp.pop();
    }
}

// ADMIN PANEL

void adminPanel() {
    int choice;

    do {
        cout << "\n========== ADMIN PANEL ==========\n";
        cout << "1. Show Journeys\n";
        cout << "2. Add Journey\n";
        cout << "3. Update Journey\n";
        cout << "4. Delete Journey\n";
        cout << "5. Search Journey\n";
        cout << "6. Sort by Fare\n";
        cout << "7. View All Bookings\n";
        cout << "8. View Waiting List\n";
        cout << "9. Cancel Booking\n";
        cout << "10. Recent History\n";
        cout << "11. Logout\n";

        choice = readInt("Enter choice: ");

        switch (choice) {
            case 1: showJourneys(); break;
            case 2: addJourney(); break;
            case 3: updateJourney(); break;
            case 4: deleteJourney(); break;
            case 5: searchJourney(); break;
            case 6: sortByFare(); break;
            case 7: showAllBookings(); break;
            case 8: showWaitingList(); break;
            case 9: cancelTicket("", true); break;
            case 10: showRecentHistory(); break;
            case 11: cout << "Admin logged out.\n"; break;
            default: cout << "Invalid choice.\n";
        }

    } while (choice != 11);
}

// USER PANEL

void userPanel(string username) {
    int choice;

    do {
        cout << "\n========== USER PANEL ==========\n";
        cout << "1. Show Journeys\n";
        cout << "2. Search Journey\n";
        cout << "3. Sort by Fare\n";
        cout << "4. Book Ticket\n";
        cout << "5. My Bookings\n";
        cout << "6. Cancel Ticket\n";
        cout << "7. Recent History\n";
        cout << "8. Logout\n";

        choice = readInt("Enter choice: ");

        switch (choice) {
            case 1: showJourneys(); break;
            case 2: searchJourney(); break;
            case 3: sortByFare(); break;
            case 4: bookTicket(username); break;
            case 5: myBookings(username); break;
            case 6: cancelTicket(username); break;
            case 7: showRecentHistory(); break;
            case 8: cout << "User logged out.\n"; break;
            default: cout << "Invalid choice.\n";
        }

    } while (choice != 8);
}

// MAIN FUNCTION

int main() {
    loadJourneys();
    loadUsers();
    loadBookings();
    loadWaitingList();

    int choice;

    cout << "\n==========================================\n";
    cout << " RAILWAY / BUS TICKET RESERVATION SYSTEM\n";
    cout << "==========================================\n";

    do {
        cout << "\n========== MAIN MENU ==========\n";
        cout << "1. Admin Login\n";
        cout << "2. User Registration\n";
        cout << "3. User Login\n";
        cout << "4. Show Journeys\n";
        cout << "5. Sort Journeys by Fare\n";
        cout << "6. Exit\n";

        choice = readInt("Enter your choice: ");

        switch (choice) {
            case 1:
                if (adminLogin())
                    adminPanel();
                break;

            case 2:
                registerUser();
                break;

            case 3: {
                string username;

                if (userLogin(username))
                    userPanel(username);

                break;
            }

            case 4:
                showJourneys();
                break;

            case 5:
                sortByFare();
                break;

            case 6:
                saveJourneys();
                saveUsers();
                saveBookings();
                saveWaitingList();

                cout << "Thank you for using the system!\n";
                break;

            default:
                cout << "Invalid choice. Try again.\n";
        }

    } while (choice != 6);

    return 0;
}
