#include <iostream>
#include <vector>
#include <queue>
#include <string>

using namespace std;

// Structure to represent a customer (student)
struct Customer {
    int id;
    string name;
    int floor;
    int roomNumber;
    bool bookingConfirmed;

    Customer(int i, string n, int f = -1, int r = -1, bool bc = false)
        : id(i), name(n), floor(f), roomNumber(r), bookingConfirmed(bc) {}
};

// Class to manage hostel bookings
class HostelBookingSystem {
private:
    vector<vector<bool>> rooms;               // 2D array to represent rooms (false = available)
    queue<Customer> bookingRequests;          // Queue for booking requests
    vector<Customer> customers;               // List of all customers

public:
    // Constructor to initialize rooms
    HostelBookingSystem() : rooms(2, vector<bool>(5, false)) {
        // Initialize all rooms as available (false)
    }

    // Display all available rooms in a 2D grid
    void showAvailableRooms() {
        cout << "\nRoom Availability (0 = Available, 1 = Booked):\n";
        for (size_t floor = 0; floor < rooms.size(); ++floor) {
            cout << "Floor " << (floor + 1) << ": ";
            for (size_t room = 0; room < rooms[floor].size(); ++room) {
                cout << rooms[floor][room] << " ";
            }
            cout << "\n";
        }
    }

    // Request a room booking
    void requestBooking() {
        int id;
        string name;

        cout << "\nEnter your ID: ";
        cin >> id;
        cin.ignore(); // Ignore leftover newline
        for (const auto& customer : customers) {
            if (customer.id == id) {
                cout << "A customer with this ID already exists. Booking cannot be duplicated.\n";
                return;
            }
        }

        cout << "Enter your name: ";
        getline(cin, name);

        Customer newCustomer(id, name);
        bookingRequests.push(newCustomer);
        customers.push_back(newCustomer);

        cout << "Booking request submitted for " << name << ". Your booking is pending.\n";
    }

    // Confirm booking for the next customer in the queue
    void confirmBooking() {
        if (bookingRequests.empty()) {
            cout << "No pending booking requests.\n";
            return;
        }

        // Find the first available room
        for (size_t floor = 0; floor < rooms.size(); ++floor) {
            for (size_t room = 0; room < rooms[floor].size(); ++room) {
                if (!rooms[floor][room]) { // Room is available
                    Customer currentCustomer = bookingRequests.front();
                    bookingRequests.pop();

                    // Assign the room to the customer
                    rooms[floor][room] = true;
                    currentCustomer.floor = floor + 1;
                    currentCustomer.roomNumber = room + 1;
                    currentCustomer.bookingConfirmed = true;

                    // Update the customer in the list
                    for (auto& customer : customers) {
                        if (customer.id == currentCustomer.id) {
                            customer = currentCustomer;
                            break;
                        }
                    }

                    cout << "Booking confirmed for " << currentCustomer.name
                        << " in Floor " << currentCustomer.floor
                        << ", Room " << currentCustomer.roomNumber << ".\n";
                    return;
                }
            }
        }

        cout << "No rooms available at the moment. Booking remains pending.\n";
    }

    // Check booking status
    void checkBookingStatus() {
        int id;
        cout << "\nEnter your ID to check booking status: ";
        cin >> id;

        for (const auto& customer : customers) {
            if (customer.id == id) {
                if (customer.bookingConfirmed) {
                    cout << "Hello " << customer.name
                        << ", your booking is confirmed in Floor " << customer.floor
                        << ", Room " << customer.roomNumber << ".\n";
                }
                else {
                    cout << "Hello " << customer.name
                        << ", your booking is still pending. Please wait for confirmation.\n";
                }
                return;
            }
        }
        cout << "No booking request found with ID " << id << ".\n";
    }

    // Cancel a booking
    void cancelBooking() {
        int id;
        cout << "\nEnter your ID to cancel your booking: ";
        cin >> id;

        for (size_t i = 0; i < customers.size(); ++i) {
            if (customers[i].id == id) {
                if (customers[i].bookingConfirmed) {
                    // Free the room
                    int floor = customers[i].floor - 1;
                    int room = customers[i].roomNumber - 1;
                    rooms[floor][room] = false;
                }

                customers.erase(customers.begin() + i); // Remove customer from the list
                cout << "Your booking has been canceled.\n";
                return;
            }
        }
        cout << "No booking found with ID " << id << ".\n";
    }

    // List all customers
    void listAllCustomers() {
        if (customers.empty()) {
            cout << "\nNo customers found.\n";
            return;
        }

        cout << "\nList of All Customers:\n";
        for (const auto& customer : customers) {
            cout << "ID: " << customer.id
                << ", Name: " << customer.name
                << ", Floor: " << (customer.bookingConfirmed ? to_string(customer.floor) : "Pending")
                << ", Room: " << (customer.bookingConfirmed ? to_string(customer.roomNumber) : "Pending")
                << ", Status: " << (customer.bookingConfirmed ? "Confirmed" : "Pending") << "\n";
        }
    }
};

// Main function to simulate the booking system
int main() {
    HostelBookingSystem hbs;
    int choice;

    while (true) {
        cout << "\n--- Hostel Booking System ---\n";
        cout << "1. Show Available Rooms\n";
        cout << "2. Request Room Booking\n";
        cout << "3. Check Booking Status\n";
        cout << "4. Cancel Booking\n";
        cout << "5. Confirm Next Booking (Admin)\n";
        cout << "6. List All Customers\n";
        cout << "7. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            hbs.showAvailableRooms();
            break;
        case 2:
            hbs.requestBooking();
            break;
        case 3:
            hbs.checkBookingStatus();
            break;
        case 4:
            hbs.cancelBooking();
            break;
        case 5:
            hbs.confirmBooking();
            break;
        case 6:
            hbs.listAllCustomers();
            break;
        case 7:
            cout << "Thank you for using the Hostel Booking System. Goodbye!\n";
            return 0;
        default:
            cout << "Invalid choice. Please try again.\n";
        }
    }
}
