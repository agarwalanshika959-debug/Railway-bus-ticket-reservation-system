#include <iostream>
#include <string>
using namespace std;

struct Journey {
    int id, fare, seats;
    string type, name, from, to;
};

struct Booking {
    int pnr, journeyId, seatCount, totalFare;
    string passenger;
    bool active;
};

Journey journeys[6] = {
    {1,450,40,"Train","Shatabdi","Delhi","Dehradun"},
    {2,300,35,"Train","JanShatabdi","Haridwar","Delhi"},
    {3,550,25,"Bus","Volvo","Roorkee","Delhi"},
    {4,120,30,"Bus","OrdinaryBus","Dehradun","Haridwar"},
    {5,380,20,"Train","MussoorieExp","Dehradun","Delhi"},
    {6,250,32,"Bus","ExpressBus","Delhi","Haridwar"}
};

Booking bookings[100];
int bookingCount = 0;
int nextPNR = 1001;

int findJourney(int id) {
    for (int i = 0; i < 6; i++)
        if (journeys[i].id == id)
            return i;
    return -1;
}

void showJourneys() {
    cout << "\n--- Available Journeys ---\n";
    for (int i = 0; i < 6; i++) {
        cout << journeys[i].id << ". "
             << journeys[i].type << " "
             << journeys[i].name << " | "
             << journeys[i].from << " to "
             << journeys[i].to << " | Fare: Rs."
             << journeys[i].fare << " | Seats: "
             << journeys[i].seats << "\n";
    }
}

void searchJourney() {
    string city;
    bool found = false;

    cout << "Enter source or destination: ";
    cin >> city;

    for (int i = 0; i < 6; i++) {
        if (journeys[i].from == city ||
            journeys[i].to == city) {
            cout << journeys[i].name << " | "
                 << journeys[i].from << " to "
                 << journeys[i].to << " | Rs."
                 << journeys[i].fare << "\n";
            found = true;
        }
    }

    if (!found)
        cout << "No journey found.\n";
}

void sortByFare() {
    Journey temp[6];

    for (int i = 0; i < 6; i++)
        temp[i] = journeys[i];

    // Bubble Sort
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5 - i; j++) {
            if (temp[j].fare > temp[j + 1].fare) {
                Journey t = temp[j];
                temp[j] = temp[j + 1];
                temp[j + 1] = t;
            }
        }
    }

    cout << "\n--- Sorted by Fare ---\n";
    for (int i = 0; i < 6; i++)
        cout << temp[i].name << " Rs."
             << temp[i].fare << "\n";
}

void bookTicket() {
    if (bookingCount >= 100) {
        cout << "Booking limit reached.\n";
        return;
    }

    int id, count;
    string passenger;

    showJourneys();

    cout << "Enter journey ID: ";
    cin >> id;

    int index = findJourney(id);
    if (index == -1) {
        cout << "Invalid journey ID.\n";
        return;
    }

    cout << "Enter passenger name (one word): ";
    cin >> passenger;

    cout << "Enter number of seats: ";
    cin >> count;

    if (count <= 0 || count > journeys[index].seats) {
        cout << "Invalid number of seats.\n";
        return;
    }

    bookings[bookingCount].pnr = nextPNR++;
    bookings[bookingCount].journeyId = id;
    bookings[bookingCount].seatCount = count;
    bookings[bookingCount].totalFare =
        count * journeys[index].fare;
    bookings[bookingCount].passenger = passenger;
    bookings[bookingCount].active = true;

    journeys[index].seats -= count;

    cout << "\nBooking Successful!\n";
    cout << "PNR: " << bookings[bookingCount].pnr << "\n";
    cout << "Passenger: "
         << bookings[bookingCount].passenger << "\n";
    cout << "Journey: " << journeys[index].name << "\n";
    cout << "Total Fare: Rs."
         << bookings[bookingCount].totalFare << "\n";

    bookingCount++;
}

void showBookings() {
    bool found = false;

    cout << "\n--- My Bookings ---\n";

    for (int i = 0; i < bookingCount; i++) {
        if (bookings[i].active) {
            int index = findJourney(bookings[i].journeyId);

            cout << "PNR: " << bookings[i].pnr
                 << " | Passenger: "
                 << bookings[i].passenger;

            if (index != -1)
                cout << " | Journey: " << journeys[index].name;

            cout << " | Seats: " << bookings[i].seatCount
                 << " | Fare: Rs." << bookings[i].totalFare
                 << "\n";

            found = true;
        }
    }

    if (!found)
        cout << "No active bookings.\n";
}

void cancelTicket() {
    int pnr;
    cout << "Enter PNR to cancel: ";
    cin >> pnr;

    for (int i = 0; i < bookingCount; i++) {
        if (bookings[i].pnr == pnr && bookings[i].active) {
            int index = findJourney(bookings[i].journeyId);

            if (index != -1)
                journeys[index].seats += bookings[i].seatCount;

            bookings[i].active = false;
            cout << "Ticket cancelled successfully.\n";
            return;
        }
    }

    cout << "Active booking not found.\n";
}

int main() {
    string username, password;

    cout << "RAILWAY / BUS RESERVATION SYSTEM\n";
    cout << "Demo username: admin | Password: 1234\n";
    cout << "Username: ";
    cin >> username;
    cout << "Password: ";
    cin >> password;

    if (username != "admin" || password != "1234") {
        cout << "Login failed.\n";
        return 0;
    }

    cout << "Login successful!\n";

    int choice;

    do {
        cout << "\n1. Show Journeys\n";
        cout << "2. Search Journey\n";
        cout << "3. Sort by Fare\n";
        cout << "4. Book Ticket\n";
        cout << "5. My Bookings\n";
        cout << "6. Cancel Ticket\n";
        cout << "7. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
            case 1: showJourneys(); break;
            case 2: searchJourney(); break;
            case 3: sortByFare(); break;
            case 4: bookTicket(); break;
            case 5: showBookings(); break;
            case 6: cancelTicket(); break;
            case 7: cout << "Thank you!\n"; break;
            default: cout << "Invalid choice.\n";
        }
    } while (choice != 7);

    return 0;
}
