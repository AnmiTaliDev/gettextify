// Copyright 2026 AnmiTaliDev <anmitalidev@nuros.org>
// This code licensed under LGPL 3.0

#ifndef GETTEXTIFY_CORE_METADATA_H
#define GETTEXTIFY_CORE_METADATA_H

#include <string>

namespace gettextify {
namespace core {

/**
 * Metadata for translation catalogs
 */
struct Metadata {
    std::string project_id_version;
    std::string report_msgid_bugs_to;
    std::string last_translator;
    std::string language_team;
    std::string language;
    std::string charset;
    std::string content_transfer_encoding;
    
    // Constructors
    Metadata() 
        : project_id_version("gettextify 1.0")
        , report_msgid_bugs_to("")
        , last_translator("gettextify")
        , language_team("")
        , language("")
        , charset("UTF-8")
        , content_transfer_encoding("8bit")
    {}
    
    Metadata(const std::string& project_version,
             const std::string& bugs_to = "",
             const std::string& translator = "gettextify",
             const std::string& team = "",
             const std::string& lang = "")
        : project_id_version(project_version)
        , report_msgid_bugs_to(bugs_to)
        , last_translator(translator)
        , language_team(team)
        , language(lang)
        , charset("UTF-8")
        , content_transfer_encoding("8bit")
    {}
};

} // namespace core
} // namespace gettextify

#endif // GETTEXTIFY_CORE_METADATA_H
