// Copyright 2026 AnmiTaliDev <anmitalidev@nuros.org>
// This code licensed under LGPL 3.0

#include "csv_parser.h"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <iostream>
#include <algorithm>

namespace gettextify {
namespace format {
namespace csv {

CsvParser::CsvParser(char delimiter) : delimiter_(delimiter) {}

void CsvParser::set_delimiter(char delimiter) {
    delimiter_ = delimiter;
}

char CsvParser::get_delimiter() const {
    return delimiter_;
}

std::vector<std::string> CsvParser::split_line(const std::string& line) const {
    std::vector<std::string> fields;
    bool in_quotes = false;
    std::string field;
    
    for (size_t i = 0; i < line.length(); ++i) {
        char c = line[i];
        
        if (c == '"') {
            if (in_quotes && i + 1 < line.length() && line[i + 1] == '"') {
                // Escaped quote
                field += '"';
                ++i;
            } else {
                // Toggle quote mode
                in_quotes = !in_quotes;
            }
        } else if (c == delimiter_ && !in_quotes) {
            // End of field
            fields.push_back(field);
            field.clear();
        } else {
            field += c;
        }
    }
    
    // Add last field
    fields.push_back(field);
    
    return fields;
}

std::string CsvParser::remove_quotes(const std::string& value) const {
    if (value.length() >= 2 && value.front() == '"' && value.back() == '"') {
        return value.substr(1, value.length() - 2);
    }
    return value;
}

void CsvParser::parse(const std::string& file_path, core::Catalog& catalog) {
    std::ifstream file(file_path);
    if (!file) {
        throw std::runtime_error("Cannot open file: " + file_path);
    }
    
    std::string line;
    if (!std::getline(file, line)) {
        throw std::runtime_error("CSV file is empty");
    }
    
    // Parse header
    auto header = split_line(line);
    int msgid_idx = -1;
    int msgstr_idx = -1;
    int comment_idx = -1;
    
    for (size_t i = 0; i < header.size(); ++i) {
        std::string col = header[i];
        // Trim whitespace
        col.erase(0, col.find_first_not_of(" \t\r\n"));
        col.erase(col.find_last_not_of(" \t\r\n") + 1);
        
        if (col == "msgid") msgid_idx = i;
        else if (col == "msgstr") msgstr_idx = i;
        else if (col == "comment") comment_idx = i;
    }
    
    if (msgid_idx == -1 || msgstr_idx == -1) {
        throw std::runtime_error("CSV must have 'msgid' and 'msgstr' columns");
    }
    
    // Parse data rows
    int line_num = 1;
    while (std::getline(file, line)) {
        ++line_num;
        
        // Skip empty lines
        if (line.empty() || line.find_first_not_of(" \t\r\n") == std::string::npos) {
            continue;
        }
        
        auto fields = split_line(line);
        
        int max_idx = std::max({msgid_idx, msgstr_idx, comment_idx});
        if (static_cast<int>(fields.size()) <= max_idx) {
            std::cerr << "Warning: Skipping line " << line_num 
                      << " due to insufficient fields" << std::endl;
            continue;
        }
        
        std::string msgid = remove_quotes(fields[msgid_idx]);
        std::string msgstr = remove_quotes(fields[msgstr_idx]);
        std::string comment = (comment_idx >= 0 && comment_idx < static_cast<int>(fields.size())) 
                              ? remove_quotes(fields[comment_idx]) : "";
        
        // Trim whitespace
        msgid.erase(0, msgid.find_first_not_of(" \t\r\n"));
        msgid.erase(msgid.find_last_not_of(" \t\r\n") + 1);
        msgstr.erase(0, msgstr.find_first_not_of(" \t\r\n"));
        msgstr.erase(msgstr.find_last_not_of(" \t\r\n") + 1);
        
        catalog.add_entry(msgid, msgstr, comment);
    }
}

} // namespace csv
} // namespace format
} // namespace gettextify
