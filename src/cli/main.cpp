// Copyright 2026 AnmiTaliDev <anmitalidev@nuros.org>
// This code licensed under LGPL 3.0

#include <iostream>
#include <filesystem>
#include <exception>
#include <cstring>
#include "../core/catalog.h"
#include "../core/metadata.h"
#include "../format/csv/csv_parser.h"
#include "../engine/mo_compiler.h"

namespace fs = std::filesystem;

void print_usage() {
    std::cout << "Usage: gettextify <input.csv> <output.mo> [options]\n\n"
              << "Positional arguments:\n"
              << "  input.csv              CSV file with columns: msgid,msgstr,comment\n"
              << "  output.mo              Output MO file path\n\n"
              << "Options:\n"
              << "  -d, --delimiter CHAR   CSV delimiter character (default: ',')\n"
              << "  --project-id VERSION   Project-Id-Version header\n"
              << "  --bugs-to EMAIL        Report-Msgid-Bugs-To header\n"
              << "  --translator NAME      Last-Translator header\n"
              << "  --team NAME            Language-Team header\n"
              << "  --language CODE        Language code (e.g. ru, en)\n"
              << "  -h, --help             Show this help message\n\n"
              << "Example:\n"
              << "  gettextify input.csv output.mo -d ';' --project-id 'MyApp 2.0'\n";
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        print_usage();
        return 1;
    }
    
    // Check for help flag
    for (int i = 1; i < argc; ++i) {
        if (std::strcmp(argv[i], "-h") == 0 || std::strcmp(argv[i], "--help") == 0) {
            print_usage();
            return 0;
        }
    }
    
    std::string input_file = argv[1];
    std::string output_file = argv[2];
    char delimiter = ',';
    gettextify::core::Metadata metadata;
    
    // Parse options
    for (int i = 3; i < argc; ++i) {
        std::string arg = argv[i];
        
        if ((arg == "-d" || arg == "--delimiter") && i + 1 < argc) {
            delimiter = argv[++i][0];
        } else if (arg == "--project-id" && i + 1 < argc) {
            metadata.project_id_version = argv[++i];
        } else if (arg == "--bugs-to" && i + 1 < argc) {
            metadata.report_msgid_bugs_to = argv[++i];
        } else if (arg == "--translator" && i + 1 < argc) {
            metadata.last_translator = argv[++i];
        } else if (arg == "--team" && i + 1 < argc) {
            metadata.language_team = argv[++i];
        } else if (arg == "--language" && i + 1 < argc) {
            metadata.language = argv[++i];
        }
    }
    
    try {
        if (!fs::exists(input_file)) {
            std::cerr << "Error: Input file '" << input_file << "' not found.\n";
            return 1;
        }
        
        std::cout << "Converting " << input_file << " to " << output_file << "...\n";
        
        // Parse CSV
        gettextify::core::Catalog catalog;
        gettextify::format::csv::CsvParser parser(delimiter);
        parser.parse(input_file, catalog);
        
        std::cout << "Found " << catalog.size() << " translation entries.\n";
        
        // Create output directory if needed
        fs::path output_path(output_file);
        if (output_path.has_parent_path()) {
            fs::path output_dir = output_path.parent_path();
            if (!fs::exists(output_dir)) {
                fs::create_directories(output_dir);
                std::cout << "Created directory: " << output_dir << "\n";
            }
        }
        
        // Compile to MO
        gettextify::engine::MoCompiler compiler(metadata);
        compiler.compile(catalog, output_file);
        
        std::cout << "MO file created successfully: " << output_file << "\n";
        
    } catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << "\n";
        return 1;
    }
    
    return 0;
}
