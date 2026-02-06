// Copyright 2026 AnmiTaliDev <anmitalidev@nuros.org>
// This code licensed under LGPL 3.0

#include "../core/catalog.h"
#include <cassert>
#include <iostream>

using namespace gettextify::core;

void test_catalog_empty() {
    Catalog catalog;
    assert(catalog.empty());
    assert(catalog.size() == 0);
    std::cout << "✓ test_catalog_empty passed\n";
}

void test_catalog_add_entry() {
    Catalog catalog;
    catalog.add_entry("Hello", "Привет", "Greeting");
    
    assert(!catalog.empty());
    assert(catalog.size() == 1);
    assert(catalog.get_entries()[0].msgid == "Hello");
    assert(catalog.get_entries()[0].msgstr == "Привет");
    assert(catalog.get_entries()[0].comment == "Greeting");
    
    std::cout << "✓ test_catalog_add_entry passed\n";
}

void test_catalog_multiple_entries() {
    Catalog catalog;
    catalog.add_entry("Hello", "Привет");
    catalog.add_entry("Goodbye", "До свидания");
    catalog.add_entry("Welcome", "Добро пожаловать");
    
    assert(catalog.size() == 3);
    assert(catalog.get_entries()[1].msgid == "Goodbye");
    
    std::cout << "✓ test_catalog_multiple_entries passed\n";
}

void test_catalog_clear() {
    Catalog catalog;
    catalog.add_entry("Test", "Тест");
    catalog.add_entry("Exit", "Выход");
    
    assert(catalog.size() == 2);
    catalog.clear();
    assert(catalog.empty());
    assert(catalog.size() == 0);
    
    std::cout << "✓ test_catalog_clear passed\n";
}

int main() {
    std::cout << "Running Catalog tests...\n";
    
    test_catalog_empty();
    test_catalog_add_entry();
    test_catalog_multiple_entries();
    test_catalog_clear();
    
    std::cout << "\nAll Catalog tests passed!\n";
    return 0;
}
