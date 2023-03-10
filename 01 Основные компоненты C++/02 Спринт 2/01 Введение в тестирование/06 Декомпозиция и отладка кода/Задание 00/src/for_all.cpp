#include <cassert>
#include <iostream>
#include <map>
#include <string>
#include <vector>

using namespace std;

enum class QueryType {
    NewBus,
    BusesForStop,
    StopsForBus,
    AllBuses,
};

struct Query {
    QueryType type;
    string bus;
    string stop;
    vector<string> stops;
};

istream& operator>>(istream& is, Query& q) {
    // Реализуйте эту функцию
    string operation_code;
    is >> operation_code;
    if (operation_code == "NEW_BUS"s) {
        q.type = QueryType::NewBus;
        is >> q.bus;
        int count;
        is >> count;
        q.stops.clear();
        for (int i = 0; i < count; i++) {
            string stop;
            is >> stop;
            q.stops.push_back(stop);
        }
    }
    else if (operation_code == "BUSES_FOR_STOP"s) {
        q.type = QueryType::BusesForStop;
        is >> q.stop;
    }
    else if (operation_code == "STOPS_FOR_BUS"s) {
        q.type = QueryType::StopsForBus;
        is >> q.bus;
    }
    else if (operation_code == "ALL_BUSES"s) {
        q.type = QueryType::AllBuses;
    }
    return is;
}

struct BusesForStopResponse {
    // Наполните полями эту структуру
    vector<string> buses_to_stops;
};

ostream& operator<<(ostream& os, const BusesForStopResponse& r) {
    // Реализуйте эту функцию
    if (r.buses_to_stops.empty())
        os << "No stop";
    else
        for (auto bus : r.buses_to_stops)
            os << bus << " ";
    return os;
}

struct StopsForBusResponse {
    // Наполните полями эту структуру
    vector<pair<string, vector<string>>> stops_to_buses;
    string bus;
};

ostream& operator<<(ostream& os, const StopsForBusResponse& r) {
    // Реализуйте эту функцию
    if (r.stops_to_buses.size() == 0) {
        os << "No bus"s;
    }
    else {
        for (const auto stops_buses : r.stops_to_buses) {
            os << "Stop "s << stops_buses.first << ": "s;
            if (stops_buses.second.size() == 1) {
                os << "no interchange"s;
            }
            else {
                for (const string& other_bus : stops_buses.second) {
                    if (r.bus != other_bus) {
                        os << other_bus << " "s;
                    }
                }
            }
        }
    }
    return os;
}

struct AllBusesResponse {
    // Наполните полями эту структуру
    map<string, vector<string>> buses_to_stops;
};

ostream& operator<<(ostream& os, const AllBusesResponse& r) {
    // Реализуйте эту функцию
    if (r.buses_to_stops.empty()) {
        os << "No buses"s;
    }
    else {
        for (const auto& bus_item : r.buses_to_stops) {
            os << "Bus "s << bus_item.first << ": "s;
            for (const string& stop : bus_item.second) {
                os << stop << " "s;
            }
            os << endl;
        }
    }
    return os;
}

class BusManager {
private:
    map<string, vector<string>> buses_to_stops, stops_to_buses;
public:
    void AddBus(const string& bus, const vector<string>& stops) {
        // Реализуйте этот метод
        for (const string& stop : stops) {
            stops_to_buses[stop].push_back(bus);
            buses_to_stops[bus].push_back(stop);
        }

    }

    BusesForStopResponse GetBusesForStop(const string& stop) const {
        // Реализуйте этот метод
        BusesForStopResponse responce;
        if (stops_to_buses.count(stop) != 0) {
            for (const string& bus : stops_to_buses.at(stop)) {
                responce.buses_to_stops.push_back(bus);
            }
        }
        return responce;
    }

    StopsForBusResponse GetStopsForBus(const string& bus) const {
        // Реализуйте этот метод
        StopsForBusResponse responce;
        if (buses_to_stops.count(bus) > 0) {
            responce.bus = bus;
            for (const auto& stop : buses_to_stops.at(bus)) {
                responce.stops_to_buses.push_back(make_pair(stop, stops_to_buses.at(stop)));
            }
        }
        return responce;
    }

    AllBusesResponse GetAllBuses() const {
        // Реализуйте этот метод
        AllBusesResponse responce;
        if (!buses_to_stops.empty()) {
            for (const auto& bus_item : buses_to_stops) {
                
                for (const string& stop : bus_item.second) {
                    responce.buses_to_stops[bus_item.first].push_back(stop);
                }
            }
        }
        return responce;
    }
};

// Не меняя тела функции main, реализуйте функции и классы выше

int main() {
    int query_count;
    Query q;

    cin >> query_count;

    BusManager bm;
    for (int i = 0; i < query_count; ++i) {
        cin >> q;
        switch (q.type) {
        case QueryType::NewBus:
            bm.AddBus(q.bus, q.stops);
            break;
        case QueryType::BusesForStop:
            cout << bm.GetBusesForStop(q.stop) << endl;
            break;
        case QueryType::StopsForBus:
            cout << bm.GetStopsForBus(q.bus) << endl;
            break;
        case QueryType::AllBuses:
            cout << bm.GetAllBuses() << endl;
            break;
        }
    }
}