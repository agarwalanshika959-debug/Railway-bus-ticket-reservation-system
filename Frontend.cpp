#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <limits>
using namespace std;

struct Journey
{
    int id, fare, seats;
    string name, type, source, destination;
};

vector<Journey> journeys = {
    {1, 450, 40, "Shatabdi", "Train", "Delhi", "Dehradun"},
    {2, 300, 35, "JanShatabdi", "Train", "Haridwar", "Delhi"},
    {3, 550, 25, "Volvo", "Bus", "Roorkee", "Delhi"},
    {4, 120, 30, "OrdinaryBus", "Bus", "Dehradun", "Haridwar"},
    {5, 380, 20, "MussoorieExp", "Train", "Dehradun", "Delhi"},
    {6, 250, 32, "ExpressBus", "Bus", "Delhi", "Haridwar"}
};

void line()
{
    cout << "\n============================================================\n";
}

void welcomeScreen()
{
    cout << "\n";
    line();
    cout << "          RAILWAY & BUS TICKET RESERVATION SYSTEM\n";
    cout << "                    SMART TRAVEL PORTAL\n";
    line();

    cout << "\n                 WELCOME TO OUR SYSTEM\n";
    cout << "              BOOK YOUR JOURNEY EASILY\n";
    cout << "\n              TRAIN  |  BUS  |  TRAVEL\n";
    line();
}

void showJourneys()
{
    line();
    cout << "                    AVAILABLE JOURNEYS\n";
    line();

    cout << left
         << setw(5) << "ID"
         << setw(17) << "Journey"
         << setw(10) << "Type"
         << setw(13) << "Source"
         << setw(13) << "Destination"
         << setw(8) << "Fare"
         << "Seats\n";

    line();

    for (const auto &j : journeys)
    {
        cout << left
             << setw(5) << j.id
             << setw(17) << j.name
             << setw(10) << j.type
             << setw(13) << j.source
             << setw(13) << j.destination
             << setw(8) << j.fare
             << j.seats << "\n";
    }

    line();
}

void searchJourney()
{
    string source, destination;
    bool found = false;

    cout << "\nEnter Source: ";
    cin >> source;

    cout << "Enter Destination: ";
    cin >> destination;

    for (const auto &j : journeys)
    {
        if (j.source == source &&
            j.destination == destination)
        {
            cout << "\nJourney Found!\n";
            cout << "Name: " << j.name << "\n";
            cout << "Type: " << j.type << "\n";
            cout << "Fare: Rs. " << j.fare << "\n";
            cout << "Available Seats: " << j.seats << "\n";
            found = true;
        }
    }

    if (!found)
        cout << "\nNo Journey Found!\n";
}

void bookTicket()
{
    int id, count;
    bool found = false;

    showJourneys();

    cout << "\nEnter Journey ID: ";
    cin >> id;

    for (auto &j : journeys)
    {
        if (j.id == id)
        {
            found = true;

            cout << "Enter Number of Seats: ";
            cin >> count;

            if (count > 0 && count <= j.seats)
            {
                j.seats -= count;

                cout << "\n        BOOKING SUCCESSFUL!\n";
                cout << "Journey: " << j.name << "\n";
                cout << "Seats Booked: " << count << "\n";
                cout << "Total Fare: Rs. " << count * j.fare << "\n";
            }
            else
            {
                cout << "\nInvalid seat quantity or insufficient seats!\n";
            }
            break;
        }
    }

    if (!found)
        cout << "\nInvalid Journey ID!\n";
}

void sortByFare()
{
    for (int i = 0; i < (int)journeys.size() - 1; i++)
    {
        for (int j = 0; j < (int)journeys.size() - i - 1; j++)
        {
            if (journeys[j].fare > journeys[j + 1].fare)
            {
                swap(journeys[j], journeys[j + 1]);
            }
        }
    }

    cout << "\nJourneys Sorted by Fare (Low to High)!\n";
    showJourneys();
}

void adminMenu()
{
    int choice;

    do
    {
        line();
        cout << "                    ADMIN DASHBOARD\n";
        line();

        cout << "1. View All Journeys\n";
        cout << "2. Sort Journeys by Fare\n";
        cout << "3. Logout\n";

        cout << "\nEnter Choice: ";
        cin >> choice;

        switch (choice)
        {
            case 1:
                showJourneys();
                break;

            case 2:
                sortByFare();
                break;

            case 3:
                cout << "\nAdmin Logout Successful!\n";
                break;

            default:
                cout << "\nInvalid Choice!\n";
        }

    } while (choice != 3);
}

void userMenu()
{
    int choice;

    do
    {
        line();
        cout << "                     USER DASHBOARD\n";
        line();

        cout << "1. View Journeys\n";
        cout << "2. Search Journey\n";
        cout << "3. Book Ticket\n";
        cout << "4. Sort by Fare\n";
        cout << "5. Logout\n";

        cout << "\nEnter Choice: ";
        cin >> choice;

        switch (choice)
        {
            case 1:
                showJourneys();
                break;

            case 2:
                searchJourney();
                break;

            case 3:
                bookTicket();
                break;

            case 4:
                sortByFare();
                break;

            case 5:
                cout << "\nUser Logout Successful!\n";
                break;

            default:
                cout << "\nInvalid Choice!\n";
        }

    } while (choice != 5);
}

int main()
{
    int choice;

    welcomeScreen();

    do
    {
        cout << "\n1. Admin Dashboard\n";
        cout << "2. User Dashboard\n";
        cout << "3. Explore Journeys\n";
        cout << "4. Exit\n";

        cout << "\nEnter Your Choice: ";
        cin >> choice;

        switch (choice)
        {
            case 1:
                adminMenu();
                break;

            case 2:
                userMenu();
                break;

            case 3:
                showJourneys();
                break;

            case 4:
                cout << "\nThank You for Using Our System!\n";
                cout << "Have a Safe Journey!\n";
                break;

            default:
                cout << "\nInvalid Choice! Try Again.\n";
        }

    } while (choice != 4);

    return 0;
}
