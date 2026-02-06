// Copyright 2026 AnmiTaliDev <anmitalidev@nuros.org>
// This code licensed under LGPL 3.0

#ifndef GETTEXTIFY_CORE_CATALOG_H
#define GETTEXTIFY_CORE_CATALOG_H

#include <vector>
#include "translation_entry.h"

namespace gettextify {
namespace core {

/**
 * Collection of translation entries
 */
class Catalog {
public:
    Catalog() = default;
    
    void add_entry(const TranslationEntry& entry);
    void add_entry(const std::string& msgid, const std::string& msgstr, const std::string& comment = "");
    
    const std::vector<TranslationEntry>& get_entries() const;
    std::vector<TranslationEntry>& get_entries();
    
    size_t size() const;
    bool empty() const;
    void clear();
    
private:
    std::vector<TranslationEntry> entries_;
};

} // namespace core
} // namespace gettextify

#endif // GETTEXTIFY_CORE_CATALOG_H
