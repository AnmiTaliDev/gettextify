// Copyright 2026 AnmiTaliDev <anmitalidev@nuros.org>
// This code licensed under LGPL 3.0

#ifndef GETTEXTIFY_CORE_TRANSLATION_ENTRY_H
#define GETTEXTIFY_CORE_TRANSLATION_ENTRY_H

#include <string>

namespace gettextify {
namespace core {

/**
 * Represents a single translation entry
 */
struct TranslationEntry {
    std::string msgid;
    std::string msgstr;
    std::string comment;
    
    TranslationEntry() = default;
    
    TranslationEntry(const std::string& id, const std::string& str, const std::string& cmt = "")
        : msgid(id), msgstr(str), comment(cmt) {}
};

} // namespace core
} // namespace gettextify

#endif // GETTEXTIFY_CORE_TRANSLATION_ENTRY_H
