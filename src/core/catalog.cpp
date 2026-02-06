// Copyright 2026 AnmiTaliDev <anmitalidev@nuros.org>
// This code licensed under LGPL 3.0

#include "catalog.h"

namespace gettextify {
namespace core {

void Catalog::add_entry(const TranslationEntry& entry) {
    entries_.push_back(entry);
}

void Catalog::add_entry(const std::string& msgid, const std::string& msgstr, const std::string& comment) {
    entries_.emplace_back(msgid, msgstr, comment);
}

const std::vector<TranslationEntry>& Catalog::get_entries() const {
    return entries_;
}

std::vector<TranslationEntry>& Catalog::get_entries() {
    return entries_;
}

size_t Catalog::size() const {
    return entries_.size();
}

bool Catalog::empty() const {
    return entries_.empty();
}

void Catalog::clear() {
    entries_.clear();
}

} // namespace core
} // namespace gettextify
