#pragma once
#include "flight_provider.h"
#include "hotel_provider.h"

#include <string>
#include <vector>

class Trip {
public:
    Trip(HotelProvider& hp, FlightProvider& fp) : hotel_provider_(hp), flight_provider_(fp) { }
    Trip(const Trip& trip) : flight_provider_(trip.flight_provider_), hotel_provider_(trip.hotel_provider_) { }
    Trip(Trip&& trip) : flight_provider_(trip.flight_provider_), hotel_provider_(trip.hotel_provider_) { }
    Trip& operator=(const Trip& trip) {
        flight_provider_ = trip.flight_provider_;
        hotel_provider_ = trip.hotel_provider_;
        return *this;
    }
    Trip& operator=(Trip&& trip) {
        flight_provider_ = trip.flight_provider_;
        hotel_provider_ = trip.hotel_provider_;
        return *this;
    }

    void Cancel() {
        for (FlightProvider::BookingId& flight : flights) {
            flight_provider_.Cancel(flight);
        }
        for (HotelProvider::BookingId& hotel : hotels) {
            hotel_provider_.Cancel(hotel);
        }
    }

    ~Trip() {
        Cancel();
    }

    std::vector<FlightProvider::BookingId> flights;
    std::vector<HotelProvider::BookingId> hotels;

private:
    FlightProvider& flight_provider_;
    HotelProvider& hotel_provider_;
};

class TripManager {
public:
    using BookingId = std::string;
    struct BookingData {
        std::string city_from;
        std::string city_to;
        std::string date_from;
        std::string date_to;
    };

    Trip Book(const BookingData& data) {
        Trip trip(hotel_provider_, flight_provider_);
        {
            FlightProvider::BookingData flight_booking_data(data.city_from, data.city_to, data.date_from);
            trip.flights.push_back(flight_provider_.Book(flight_booking_data));
        }
        {
            HotelProvider::BookingData hotel_booking_data(data.city_to, data.date_from, data.date_to);
            trip.hotels.push_back(hotel_provider_.Book(hotel_booking_data));
        }
        {
            FlightProvider::BookingData flight_booking_data(data.city_to, data.city_from, data.date_to);
            trip.flights.push_back(flight_provider_.Book(flight_booking_data));
        }
        return trip;
    }

    void Cancel(Trip& trip) {
        trip.Cancel();
    }

private:
    HotelProvider hotel_provider_;
    FlightProvider flight_provider_;
};