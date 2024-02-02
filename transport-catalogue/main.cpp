#include <fstream>
#include <iostream>
#include <string_view>
#include "json_reader.h"
#include "serialization.h"

using namespace std::literals;

void PrintUsage(std::ostream& stream = std::cerr) {
    stream << "Usage: transport_catalogue [make_base|process_requests]\n"sv;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        PrintUsage();
        return 1;
    }

    const std::string_view mode(argv[1]);

    if (mode == "make_base"sv) {
        catalogue::TransportCatalogue catalog;
        json_reader::JsonReader reader(catalog);
        reader.MakeBase();
    }
    else if (mode == "process_requests"sv) {
        catalogue::TransportCatalogue catalog;
        json_reader::JsonReader reader(catalog);
        reader.ProcessRequests();
        reader.OutData();
    }
    else {
        PrintUsage();
        return 1;
    }
    return 0;
}