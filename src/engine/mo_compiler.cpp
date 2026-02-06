// Copyright 2026 AnmiTaliDev <anmitalidev@nuros.org>
// This code licensed under LGPL 3.0

#include "mo_compiler.h"
#include <fstream>
#include <algorithm>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <stdexcept>

namespace gettextify {
namespace engine {

void MoCompiler::set_metadata(const core::Metadata& metadata) {
    metadata_ = metadata;
}

const core::Metadata& MoCompiler::get_metadata() const {
    return metadata_;
}

std::string MoCompiler::create_header_entry() const {
    std::time_t now = std::time(nullptr);
    char time_buf[32];
    std::strftime(time_buf, sizeof(time_buf), "%Y-%m-%d %H:%M%z", std::localtime(&now));
    
    std::ostringstream header;
    header << "Project-Id-Version: " << metadata_.project_id_version << "\n"
           << "Report-Msgid-Bugs-To: " << metadata_.report_msgid_bugs_to << "\n"
           << "POT-Creation-Date: " << time_buf << "\n"
           << "PO-Revision-Date: " << time_buf << "\n"
           << "Last-Translator: " << metadata_.last_translator << "\n"
           << "Language-Team: " << metadata_.language_team << "\n";
    
    if (!metadata_.language.empty()) {
        header << "Language: " << metadata_.language << "\n";
    }
    
    header << "MIME-Version: 1.0\n"
           << "Content-Type: text/plain; charset=" << metadata_.charset << "\n"
           << "Content-Transfer-Encoding: " << metadata_.content_transfer_encoding << "\n";
    
    return header.str();
}

std::string MoCompiler::bytes_key(const std::string& str) const {
    std::ostringstream oss;
    for (unsigned char c : str) {
        oss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(c);
    }
    return oss.str();
}

core::Catalog MoCompiler::prepare_catalog(const core::Catalog& catalog) const {
    core::Catalog result;
    
    // Copy entries
    for (const auto& entry : catalog.get_entries()) {
        result.add_entry(entry);
    }
    
    // Sort by msgid bytes for binary search compatibility
    auto& entries = result.get_entries();
    std::sort(entries.begin(), entries.end(), 
        [this](const core::TranslationEntry& a, const core::TranslationEntry& b) {
            if (a.msgid.empty()) return true;
            if (b.msgid.empty()) return false;
            return bytes_key(a.msgid) < bytes_key(b.msgid);
        });
    
    // Add header entry if not present
    if (result.empty() || !result.get_entries()[0].msgid.empty()) {
        core::Catalog temp;
        temp.add_entry("", create_header_entry());
        for (const auto& entry : result.get_entries()) {
            temp.add_entry(entry);
        }
        result = temp;
    }
    
    return result;
}

void MoCompiler::compile(const core::Catalog& catalog, const std::string& output_path) {
    auto prepared = prepare_catalog(catalog);
    const auto& entries = prepared.get_entries();
    
    std::ofstream file(output_path, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Cannot create output file: " + output_path);
    }
    
    uint32_t num_strings = entries.size();
    uint32_t strings_offset = MO_HEADER_SIZE;
    uint32_t string_table_offset = strings_offset + num_strings * 8 * 2;
    
    // Write header
    auto write_u32 = [&file](uint32_t value) {
        file.write(reinterpret_cast<const char*>(&value), sizeof(value));
    };
    
    write_u32(MO_MAGIC);
    write_u32(MO_FORMAT_REVISION);
    write_u32(num_strings);
    write_u32(strings_offset);
    write_u32(strings_offset + num_strings * 8);
    write_u32(0);
    write_u32(string_table_offset);
    
    // Prepare string data
    std::vector<std::string> original_strings;
    std::vector<std::string> translated_strings;
    
    for (const auto& entry : entries) {
        original_strings.push_back(entry.msgid);
        translated_strings.push_back(entry.msgstr);
    }
    
    // Write original strings table
    uint32_t current_offset = string_table_offset;
    for (const auto& str : original_strings) {
        write_u32(str.length());
        write_u32(current_offset);
        current_offset += str.length() + 1;
    }
    
    // Write translated strings table
    for (const auto& str : translated_strings) {
        write_u32(str.length());
        write_u32(current_offset);
        current_offset += str.length() + 1;
    }
    
    // Write original strings data
    for (const auto& str : original_strings) {
        file.write(str.c_str(), str.length() + 1);
    }
    
    // Write translated strings data
    for (const auto& str : translated_strings) {
        file.write(str.c_str(), str.length() + 1);
    }
    
    file.close();
}

} // namespace engine
} // namespace gettextify
