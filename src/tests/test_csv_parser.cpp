// Copyright 2026 AnmiTaliDev <anmitalidev@nuros.org>
// This code licensed under LGPL 3.0

#include "../format/csv/csv_parser.h"
#include "../core/catalog.h"
#include <cassert>
#include <iostream>
#include <filesystem>

using namespace gettextify::format::csv;
using namespace gettextify::core;

namespace fs = std::filesystem;

void test_csv_basic_parsing() {
    CsvParser parser(',');
    Catalog catalog;
    
    fs::path test_file = "src/tests/data/basic.csv";
    parser.parse(test_file.string(), catalog);
    
    assert(catalog.size() == 5);
    assert(catalog.get_entries()[0].msgid == "Hello");
    assert(catalog.get_entries()[0].msgstr == "Привет");
    
    std::cout << "✓ test_csv_basic_parsing passed\n";
}

void test_csv_semicolon_delimiter() {
    CsvParser parser(';');
    Catalog catalog;
    
    fs::path test_file = "src/tests/data/semicolon.csv";
    parser.parse(test_file.string(), catalog);
    
    assert(catalog.size() == 3);
    assert(catalog.get_entries()[0].msgid == "Test");
    assert(catalog.get_entries()[0].msgstr == "Тест");
    
    std::cout << "✓ test_csv_semicolon_delimiter passed\n";
}

void test_csv_quoted_fields() {
    CsvParser parser(',');
    Catalog catalog;
    
    fs::path test_file = "src/tests/data/quoted.csv";
    parser.parse(test_file.string(), catalog);
    
    assert(catalog.size() == 2);
    assert(catalog.get_entries()[0].msgid == "She said \"Hello\"");
    assert(catalog.get_entries()[0].msgstr == "Она сказала \"Привет\"");
    
    std::cout << "✓ test_csv_quoted_fields passed\n";
}

void test_csv_delimiter_change() {
    CsvParser parser(',');
    assert(parser.get_delimiter() == ',');
    
    parser.set_delimiter(';');
    assert(parser.get_delimiter() == ';');
    
    std::cout << "✓ test_csv_delimiter_change passed\n";
}

int main() {
    std::cout << "Running CSV Parser tests...\n";
    
    test_csv_basic_parsing();
    test_csv_semicolon_delimiter();
    test_csv_quoted_fields();
    test_csv_delimiter_change();
    
    std::cout << "\nAll CSV Parser tests passed!\n";
    return 0;
}
