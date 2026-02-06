# gettextify

[![License: LGPL v3](https://img.shields.io/badge/License-LGPL%20v3-blue.svg)](https://www.gnu.org/licenses/lgpl-3.0)
[![Language: C++17](https://img.shields.io/badge/C%2B%2B-17-00599C.svg)](https://en.cppreference.com/w/cpp/17)
[![Build System: Meson](https://img.shields.io/badge/Build-Meson-orange.svg)](https://mesonbuild.com/)
[![GitHub](https://img.shields.io/badge/GitHub-gettextify-181717.svg?logo=github)](https://github.com/AnmiTaliDev/gettextify)

A modular, extensible tool for converting translation files to GNU gettext formats.

## Overview

**gettextify** is a modern C++17 utility that converts translation data from various formats (currently CSV) into binary MO (Machine Object) files compatible with GNU gettext. The project is designed with modularity in mind, making it easy to add support for new input/output formats.

## Features

- **CSV to MO conversion** - Parse CSV translation files and compile to binary MO format
- **Modular architecture** - Easy to extend with new formats and compilers
- **GNU gettext compatible** - Generates standard MO files with proper headers
- **Custom delimiters** - Support for different CSV delimiter characters
- **UTF-8 encoding** - Full Unicode support
- **Fast compilation** - Efficient binary file generation with proper sorting

## Building

### Requirements

- C++17 compatible compiler (GCC 7+, Clang 5+, MSVC 2017+)
- Meson build system (0.55+)
- Ninja (recommended)

### Build Instructions

```bash
# Configure build
meson setup builddir

# Compile
meson compile -C builddir

# Install (optional)
meson install -C builddir
```

## Testing

### Run Unit Tests

```bash
meson test -C builddir
```

For verbose output:

```bash
meson test -C builddir --verbose
```

### Manual Testing

```bash
# Create test CSV
cat > test.csv << EOF
msgid,msgstr,comment
"Hello","Привет","Greeting"
"Goodbye","До свидания","Farewell"
EOF

# Convert to MO
./builddir/gettextify test.csv test.mo

# Verify (requires gettext tools)
file test.mo
msgunfmt test.mo

# Cleanup
rm test.csv test.mo
```

### Test Coverage

The test suite includes:

- **Catalog Tests** - Core data structure operations
- **CSV Parser Tests** - Format parsing with various delimiters and quoting
- **MO Compiler Tests** - Binary file generation and format validation

## Usage

```bash
gettextify <input.csv> <output.mo> [options]
```

### Positional Arguments

- `input.csv` - CSV file with columns: `msgid`, `msgstr`, `comment`
- `output.mo` - Output MO file path

### Options

- `-d, --delimiter CHAR` - CSV delimiter character (default: `,`)
- `--project-id VERSION` - Set Project-Id-Version header
- `--bugs-to EMAIL` - Set Report-Msgid-Bugs-To header
- `--translator NAME` - Set Last-Translator header
- `--team NAME` - Set Language-Team header
- `--language CODE` - Set Language code (e.g., `ru`, `en`)
- `-h, --help` - Show help message

### CSV Format

The input CSV file must have at least two columns: `msgid` and `msgstr`. The `comment` column is optional.

**Example CSV:**

```csv
msgid,msgstr,comment
"Hello","Привет","Greeting"
"Goodbye","До свидания","Farewell"
"Welcome","Добро пожаловать","Welcome message"
```

### Examples

#### Basic Conversion

```bash
# Simple conversion
gettextify translations.csv translations.mo

# Use semicolon as delimiter
gettextify translations.csv translations.mo -d ';'

# Output to specific directory
gettextify input.csv locale/ru/LC_MESSAGES/app.mo
```

#### Custom Metadata

```bash
# Set metadata via command line
gettextify input.csv output.mo \
  --project-id "MyApp 2.0" \
  --translator "John Doe <john@example.com>" \
  --language "ru"

## Architecture

The project follows a modular design:

```
src/
├── core/           # Core data structures
│   ├── translation_entry.h
│   ├── catalog.h
│   └── catalog.cpp
├── format/         # Input format parsers
│   └── csv/
│       ├── csv_parser.h
│       └── csv_parser.cpp
├── engine/         # Output format compilers
│   ├── mo_compiler.h
│   └── mo_compiler.cpp
└── cli/            # Command-line interface
    └── main.cpp
```

### Adding New Formats

To add support for a new input format (e.g., JSON, PO, XML):

1. Create `src/format/<format>/` directory
2. Implement a parser class that populates a `core::Catalog`
3. Update CLI to support the new format

To add a new output format:

1. Create a compiler class in `src/engine/`
2. Implement compilation from `core::Catalog`
3. Update CLI to support the new output

## Technical Details

### MO File Format

The compiler generates binary MO files following the GNU gettext specification:

- **Magic number**: `0x950412de`
- **Sorted entries**: Binary search compatible ordering
- **UTF-8 encoding**: Full Unicode support
- **Metadata header**: Automatic generation of gettext headers

### CSV Parsing

The CSV parser handles:

- Quoted fields with proper quote escaping (`""`)
- Custom delimiters
- Multi-line handling
- Whitespace trimming
- Column mapping by header names

## License

This code is licensed under **LGPL 3.0** (GNU Lesser General Public License version 3.0).

Copyright 2026 AnmiTaliDev <anmitalidev@nuros.org>

This program is free software: you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

## Contributing

Contributions are welcome! See [CONTRIBUTING.md](CONTRIBUTING.md) for guidelines.

### Development Priorities

- [ ] Add PO file input support
- [ ] Add JSON format support
- [ ] Add PO file output (decompiler)
- [ ] Improve error messages
- [ ] Add unit tests
- [ ] Add validation mode
- [ ] Performance optimizations

## Authors

See [AUTHORS](AUTHORS) file for the list of contributors.

## Support

For bug reports and feature requests, please use the issue tracker.
