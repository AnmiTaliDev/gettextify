// Copyright 2026 AnmiTaliDev <anmitalidev@nuros.org>
// This code licensed under LGPL 3.0

#include "../engine/mo_compiler.h"
#include "../core/catalog.h"
#include <cassert>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <cstdint>

using namespace gettextify::engine;
using namespace gettextify::core;

namespace fs = std::filesystem;

void test_mo_compilation() {
    Catalog catalog;
    catalog.add_entry("Hello", "Привет");
    catalog.add_entry("Goodbye", "До свидания");
    
    MoCompiler compiler;
    std::string output = "test_output.mo";
    
    compiler.compile(catalog, output);
    
    assert(fs::exists(output));
    assert(fs::file_size(output) > 0);
    
    fs::remove(output);
    std::cout << "✓ test_mo_compilation passed\n";
}

void test_mo_magic_number() {
    Catalog catalog;
    catalog.add_entry("Test", "Тест");
    
    MoCompiler compiler;
    std::string output = "test_magic.mo";
    
    compiler.compile(catalog, output);
    
    // Read magic number
    std::ifstream file(output, std::ios::binary);
    uint32_t magic;
    file.read(reinterpret_cast<char*>(&magic), sizeof(magic));
    file.close();
    
    assert(magic == 0x950412de);
    
    fs::remove(output);
    std::cout << "✓ test_mo_magic_number passed\n";
}

void test_mo_empty_catalog() {
    Catalog catalog;
    
    MoCompiler compiler;
    std::string output = "test_empty.mo";
    
    compiler.compile(catalog, output);
    
    // Should still create file with header entry
    assert(fs::exists(output));
    assert(fs::file_size(output) > 0);
    
    fs::remove(output);
    std::cout << "✓ test_mo_empty_catalog passed\n";
}

void test_mo_utf8_handling() {
    Catalog catalog;
    catalog.add_entry("Привет", "Hello");
    catalog.add_entry("Мир", "World");
    catalog.add_entry("Тест", "Test");
    
    MoCompiler compiler;
    std::string output = "test_utf8.mo";
    
    compiler.compile(catalog, output);
    
    assert(fs::exists(output));
    assert(fs::file_size(output) > 100); // Should have meaningful content
    
    fs::remove(output);
    std::cout << "✓ test_mo_utf8_handling passed\n";
}

int main() {
    std::cout << "Running MO Compiler tests...\n";
    
    test_mo_compilation();
    test_mo_magic_number();
    test_mo_empty_catalog();
    test_mo_utf8_handling();
    
    std::cout << "\nAll MO Compiler tests passed!\n";
    return 0;
}
