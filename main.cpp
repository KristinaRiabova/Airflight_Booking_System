
#include <string>


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
