#include <string>
#include <iostream>
#include <fstream>
#include <map>
#include <vector>


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
