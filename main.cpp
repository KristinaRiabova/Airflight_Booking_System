#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <sstream>


class Ticket {
private:
    int bookingID;
    std::string date;
    std::string flightNo;
    std::string seatNumber;
    std::string passengerName;
    int ticketPrice;

public:
    Ticket(int id, const std::string& d, const std::string& flight, const std::string& seat, const std::string& name, int price)
            : bookingID(id), date(d), flightNo(flight), seatNumber(seat), passengerName(name), ticketPrice(price) {}

    int getBookingID() const {
        return bookingID;
    }

    const std::string& getDate() const {
        return date;
    }

    const std::string& getFlightInfo() const {
        return flightNo + " " + date;
    }

    const std::string& getSeatNumber() const {
        return seatNumber;
    }

    const std::string& getPassengerName() const {
        return passengerName;
    }

    int getTicketPrice() const {
        return ticketPrice;
    }
};
class Airplane {
private:
    int numSeats;
    std::map<std::string, bool> seatAvailability;
    std::map<std::string, std::vector<std::pair<int, int>>> seatPrices;

    std::vector<Ticket> bookedTickets;

public:
    Airplane(int numSeats, const std::map<std::string, std::vector<std::pair<int, int>>>& seatPrices)
            : numSeats(numSeats), seatPrices(seatPrices) {

        for (char row = 'A'; row <= 'F'; ++row) {
            for (int seat = 1; seat <= numSeats; ++seat) {
                std::string seatNumber = std::to_string(seat) + row;
                seatAvailability[seatNumber] = true;
            }
        }
    }



    bool checkSeatAvailability(const std::string& seatNumber) {
        return seatAvailability.find(seatNumber) != seatAvailability.end() && seatAvailability[seatNumber];
    }

    int bookTicket(const std::string& date, const std::string& flightNo, const std::string& seat, const std::string& username) {
        if (!checkSeatAvailability(seat)) {
            return -1; // Seat not available
        }


        std::vector<std::pair<int, int>> prices = seatPrices[flightNo];
        int price = calculateTicketPrice(seat, prices);
        int bookingID = generateBookingID();
        seatAvailability[seat] = false;
        bookedTickets.emplace_back(bookingID, date, flightNo, seat, username, price);

        return bookingID;
    }

    int returnTicket(int bookingID) {
        for (auto it = bookedTickets.begin(); it != bookedTickets.end(); ++it) {
            if (it->getBookingID() == bookingID) {
                seatAvailability[it->getSeatNumber()] = true;
                int refundAmount = it->getTicketPrice();
                bookedTickets.erase(it);
                return refundAmount;
            }
        }
        return -1;
    }

    void viewBookingInfo(int bookingID) {
        for (const Ticket& ticket : bookedTickets) {
            if (ticket.getBookingID() == bookingID) {
                std::cout << "Flight " << ticket.getFlightInfo() << ", Seat " << ticket.getSeatNumber()
                          << ", Price " << ticket.getTicketPrice() << ", Passenger " << ticket.getPassengerName() << std::endl;
                return;
            }
        }
        std::cout << "Booking ID not found." << std::endl;
    }

    void viewUserBookings(const std::string& username) {
        for (const Ticket& ticket : bookedTickets) {
            if (ticket.getPassengerName() == username) {
                std::cout << "Flight " << ticket.getFlightInfo() << ", Seat " << ticket.getSeatNumber()
                          << ", Price " << ticket.getTicketPrice() << std::endl;
            }
        }
    }

    int calculateTicketPrice(const std::string& seat, const std::vector<std::pair<int, int>>& prices) {
        int seatNumber;
        try {
            seatNumber = std::stoi(seat.substr(0, seat.size() - 1));
        } catch (const std::invalid_argument& e) {
            std::cerr << "Error: Invalid seat number." << std::endl;
            return -1;
        }

        for (const auto& priceRange : prices) {
            if (seatNumber >= priceRange.first && seatNumber <= priceRange.second) {
                return priceRange.second;
            }
        }
        return -1;
    }





    int getSeatPrice(const std::string& flightNo, const std::string& seat) {
        if (seatPrices.find(flightNo) != seatPrices.end()) {
            for (const std::pair<int, int>& priceRange : seatPrices[flightNo]) {
                int seatNumber;
                try {
                    seatNumber = std::stoi(seat.substr(0, seat.size() - 1));
                } catch (const std::invalid_argument& e) {
                    std::cerr << "Error: Invalid seat number." << std::endl;
                    return -1;
                }
                if (seatNumber >= priceRange.first && seatNumber <= priceRange.second) {
                    return priceRange.second;
                }
            }
        }
        return -1;
    }

private:
    int generateBookingID() {
        static int id = 1;
        return id++;
    }
};
class ConfigReader {
public:
    static std::map<std::string, std::vector<std::pair<int, int>>>
    readConfigFromFile(const std::string &filename) {
        std::map<std::string, std::vector<std::pair<int, int>>> seatPrices;

        std::ifstream configFile(filename);
        if (!configFile) {
            std::cerr << "Error: Cannot open configuration file." << std::endl;
            exit(1);
        }

        int numRecords;
        configFile >> numRecords;
        configFile.ignore();

        for (int recordIndex = 0; recordIndex < numRecords; ++recordIndex) {
            std::string record;
            std::getline(configFile, record);
            std::istringstream iss(record);

            std::string date, flightNo;
            int numSeats;
            iss >> date >> flightNo >> numSeats;

            std::vector<std::pair<int, int>> prices;


            for (int j = 0; j < numSeats; ++j) {
                std::string priceRange;
                iss >> priceRange;

                size_t dashPos = priceRange.find('-');
                if (dashPos != std::string::npos) {
                    int minRow = std::stoi(priceRange.substr(0, dashPos));
                    int maxRow = std::stoi(priceRange.substr(dashPos + 1));


                    int price;
                    iss >> price;


                    prices.push_back(std::make_pair(minRow, price));
                }
            }

            seatPrices[flightNo] = prices;
        }

        configFile.close();

        return seatPrices;
    }

};
class UserInterface {
public:
    static void showMenu(Airplane& airplane) {
        while (true) {
            std::cout << "Enter a command (check, book, return, view, quit): ";
            std::string command;
            std::cin >> command;

            if (command == "check") {
                std::string date, flightNo;
                std::cout << "Enter date and flight number: ";
                std::cin >> date >> flightNo;
                std::vector<std::string> availableSeats = checkAvailableSeats(airplane, date, flightNo);
                if (!availableSeats.empty()) {
                    std::cout << "Available seats for " << flightNo << " on " << date << ":" << std::endl;
                    for (const std::string& seat : availableSeats) {
                        int price = airplane.getSeatPrice(flightNo, seat);
                        if (price != -1) {
                            std::cout << seat << " " << price << "$" << std::endl;
                        } else {
                            std::cout << "Price not found for " << flightNo + " " + seat << std::endl;
                        }
                    }
                } else {
                    std::cout << "No available seats for " << flightNo << " on " << date << "." << std::endl;
                }
            } else if (command == "book") {
                std::string date, flightNo, seat, username;
                std::cout << "Enter date, flight number, seat, and username: ";
                std::cin >> date >> flightNo >> seat >> username;
                int bookingID = airplane.bookTicket(date, flightNo, seat, username);
                if (bookingID != -1) {
                    std::cout << "Confirmed with ID " << bookingID << std::endl;
                } else {
                    std::cout << "Booking failed. The seat is not available." << std::endl;
                }
            } else if (command == "return") {
                int bookingID;
                std::cout << "Enter booking ID: ";
                std::cin >> bookingID;
                int refundAmount = airplane.returnTicket(bookingID);
                if (refundAmount != -1) {
                    std::cout << "Confirmed " << refundAmount << "$ refund." << std::endl;
                } else {
                    std::cout << "Booking ID not found." << std::endl;
                }
            } else if (command == "view") {
                std::string option;
                std::cout << "Enter 'id' to view booking by ID or 'username' to view all user bookings: ";
                std::cin >> option;
                if (option == "id") {
                    int bookingID;
                    std::cout << "Enter booking ID: ";
                    std::cin >> bookingID;
                    airplane.viewBookingInfo(bookingID);
                } else if (option == "username") {
                    std::string username;
                    std::cout << "Enter username: ";
                    std::cin >> username;
                    airplane.viewUserBookings(username);
                } else {
                    std::cout << "Invalid option. Try again." << std::endl;
                }
            } else if (command == "quit") {
                break;
            } else {
                std::cout << "Invalid command. Try again." << std::endl;
            }
        }
    }

    static std::vector<std::string> checkAvailableSeats(Airplane& airplane, const std::string& date, const std::string& flightNo) {
        std::vector<std::string> availableSeats;
        for (char row = 'A'; row <= 'F'; ++row) {
            for (int seat = 1; seat <= 6; ++seat) {
                std::string seatNumber = std::to_string(seat) + row;
                if (airplane.checkSeatAvailability(seatNumber)) {
                    availableSeats.push_back(seatNumber);
                }
            }
        }
        return availableSeats;
    }
};