#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "json.h"
#include "json_reader.h"

int main() {
    catalogue::TransportCatalogue catalog;
    json_reader::JsonReader reader(catalog);
    reader.InputData();
    return 0;
}
