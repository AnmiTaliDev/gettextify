# Copyright 2026 AnmiTaliDev <anmitalidev@nuros.org>
# This code licensed under LGPL 3.0

project('gettextify', 'cpp',
    version: '1.0.0',
    default_options: [
        'warning_level=2',
        'cpp_std=c++17'
    ])

# Core library sources
core_sources = [
    'src/core/catalog.cpp',
]

# Format parsers sources
format_sources = [
    'src/format/csv/csv_parser.cpp',
]

# Engine sources
engine_sources = [
    'src/engine/mo_compiler.cpp',
]

# CLI sources
cli_sources = [
    'src/cli/main.cpp',
]

# Build executable
executable('gettextify',
    sources: core_sources + format_sources + engine_sources + cli_sources,
    install: true,
    cpp_args: ['-O2']
)

# Tests
test_catalog = executable('test_catalog',
    sources: ['src/tests/test_catalog.cpp'] + core_sources,
    cpp_args: ['-O0', '-g']
)

test_csv_parser = executable('test_csv_parser',
    sources: ['src/tests/test_csv_parser.cpp'] + core_sources + format_sources,
    cpp_args: ['-O0', '-g']
)

test_mo_compiler = executable('test_mo_compiler',
    sources: ['src/tests/test_mo_compiler.cpp'] + core_sources + engine_sources,
    cpp_args: ['-O0', '-g']
)

test('Catalog Tests', test_catalog, workdir: meson.project_source_root())
test('CSV Parser Tests', test_csv_parser, workdir: meson.project_source_root())
test('MO Compiler Tests', test_mo_compiler, workdir: meson.project_source_root())
