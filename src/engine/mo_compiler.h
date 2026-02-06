// Copyright 2026 AnmiTaliDev <anmitalidev@nuros.org>
// This code licensed under LGPL 3.0

#ifndef GETTEXTIFY_ENGINE_MO_COMPILER_H
#define GETTEXTIFY_ENGINE_MO_COMPILER_H

#include <string>
#include <cstdint>
#include "../core/catalog.h"
#include "../core/metadata.h"

namespace gettextify {
namespace engine {

/**
 * Compiler for MO (Machine Object) binary gettext files
 */
class MoCompiler {
public:
    MoCompiler() = default;
    explicit MoCompiler(const core::Metadata& metadata) : metadata_(metadata) {}
    
    void compile(const core::Catalog& catalog, const std::string& output_path);
    
    void set_metadata(const core::Metadata& metadata);
    const core::Metadata& get_metadata() const;
    
private:
    static constexpr uint32_t MO_MAGIC = 0x950412de;
    static constexpr uint32_t MO_FORMAT_REVISION = 0;
    static constexpr int MO_HEADER_SIZE = 28;
    
    std::string create_header_entry() const;
    core::Catalog prepare_catalog(const core::Catalog& catalog) const;
    std::string bytes_key(const std::string& str) const;
    
    core::Metadata metadata_;
};

} // namespace engine
} // namespace gettextify

#endif // GETTEXTIFY_ENGINE_MO_COMPILER_H
