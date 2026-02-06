// Copyright 2026 AnmiTaliDev <anmitalidev@nuros.org>
// This code licensed under LGPL 3.0

#ifndef GETTEXTIFY_FORMAT_CSV_PARSER_H
#define GETTEXTIFY_FORMAT_CSV_PARSER_H

#include <string>
#include "../../core/catalog.h"

namespace gettextify {
namespace format {
namespace csv {

/**
 * Parser for CSV files containing translation data
 */
class CsvParser {
public:
    explicit CsvParser(char delimiter = ',');
    
    void parse(const std::string& file_path, core::Catalog& catalog);
    
    void set_delimiter(char delimiter);
    char get_delimiter() const;
    
private:
    char delimiter_;
    
    std::vector<std::string> split_line(const std::string& line) const;
    std::string remove_quotes(const std::string& value) const;
};

} // namespace csv
} // namespace format
} // namespace gettextify

#endif // GETTEXTIFY_FORMAT_CSV_PARSER_H
