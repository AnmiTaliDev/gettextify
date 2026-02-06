# Contributing to gettextify

Thank you for your interest in contributing to gettextify! This document provides guidelines for contributing to the project.

## Code of Conduct

- Be respectful and inclusive
- Focus on constructive feedback
- Help others learn and grow

## How to Contribute

### Reporting Bugs

When reporting bugs, please include:

1. **Description** - Clear description of the issue
2. **Steps to reproduce** - Minimal steps to reproduce the problem
3. **Expected behavior** - What you expected to happen
4. **Actual behavior** - What actually happened
5. **Environment** - OS, compiler version, meson version
6. **Sample files** - CSV files or commands that trigger the issue (if applicable)

### Suggesting Features

Feature requests are welcome! Please include:

1. **Use case** - Why is this feature needed?
2. **Proposed solution** - How should it work?
3. **Alternatives** - What alternatives have you considered?

## Development Setup

### Prerequisites

```bash
# Install dependencies (Ubuntu/Debian)
sudo apt install build-essential meson ninja-build

# Install dependencies (Fedora)
sudo dnf install gcc-c++ meson ninja-build

# Install dependencies (Arch)
sudo pacman -S base-devel meson ninja
```

### Building from Source

```bash
# Clone repository
git clone https://github.com/AnmiTaliDev/gettextify.git
cd gettextify

# Configure and build
meson setup builddir
meson compile -C builddir

# Run tests (when available)
meson test -C builddir
```

## Coding Standards

### C++ Style Guide

- **Standard**: C++17
- **Indentation**: 4 spaces (no tabs)
- **Line length**: Maximum 100 characters
- **Naming conventions**:
  - Classes: `PascalCase` (e.g., `MoCompiler`)
  - Functions/methods: `snake_case` (e.g., `parse_csv`)
  - Variables: `snake_case` (e.g., `entry_count`)
  - Constants: `UPPER_SNAKE_CASE` (e.g., `MO_MAGIC`)
  - Namespaces: `lowercase` (e.g., `gettextify::engine`)

### File Organization

```cpp
// Copyright 2026 AnmiTaliDev <anmitalidev@nuros.org>
// This code licensed under LGPL 3.0

#include "header.h"  // Corresponding header first
#include <system>    // System headers
#include <headers>   
#include "local/headers.h"  // Project headers

namespace gettextify {
namespace subnamespace {

// Code here

} // namespace subnamespace
} // namespace gettextify
```

### Best Practices

1. **RAII** - Use RAII for resource management
2. **const correctness** - Mark methods and parameters const where appropriate
3. **Smart pointers** - Prefer smart pointers over raw pointers
4. **Error handling** - Use exceptions for error conditions
5. **Documentation** - Add comments for complex logic
6. **Modularity** - Keep components loosely coupled

## Project Architecture

### Adding a New Input Format

Example: Adding JSON support

1. Create `src/format/json/` directory
2. Create `json_parser.h` and `json_parser.cpp`:

```cpp
// src/format/json/json_parser.h
namespace gettextify {
namespace format {
namespace json {

class JsonParser {
public:
    void parse(const std::string& file_path, core::Catalog& catalog);
};

} // namespace json
} // namespace format
} // namespace gettextify
```

3. Update `meson.build` to include new sources
4. Update CLI to recognize `.json` files
5. Add tests for the new parser

### Adding a New Output Format

Example: Adding PO file writer

1. Create `src/engine/po_writer.h` and `po_writer.cpp`
2. Implement write method that reads from `core::Catalog`
3. Update `meson.build`
4. Update CLI to support new output format
5. Add tests

## Commit Guidelines

### Commit Message Format

```
<type>: <short summary>

<detailed description (optional)>

<breaking changes (optional)>
```

### Commit Types

- `feat:` - New feature
- `fix:` - Bug fix
- `docs:` - Documentation changes
- `style:` - Code style changes (formatting, no logic change)
- `refactor:` - Code refactoring
- `test:` - Adding or updating tests
- `build:` - Build system changes
- `chore:` - Other changes (dependencies, etc.)

### Examples

```
feat: add JSON input format support

Implements JsonParser class that reads translation data from JSON files
and populates the Catalog.

- Added src/format/json/json_parser.{h,cpp}
- Updated meson.build
- Added JSON format detection in CLI
```

```
fix: handle escaped quotes in CSV parser

The CSV parser was not correctly handling escaped quotes ("") within
quoted fields. This commit fixes the quote escaping logic.

Fixes #42
```

## Pull Request Process

1. **Fork** the repository
2. **Create a branch** from `main`:
   ```bash
   git checkout -b feature/my-feature
   ```
3. **Make your changes** following coding standards
4. **Test your changes** thoroughly
5. **Commit** with clear messages
6. **Push** to your fork
7. **Create a Pull Request** with:
   - Clear title and description
   - Reference to related issues
   - Screenshots/examples if applicable

### PR Review Process

- Maintainers will review your PR
- Address any feedback or requested changes
- Once approved, your PR will be merged

## Testing

### Manual Testing

```bash
# Create test CSV file
cat > test.csv << EOF
msgid,msgstr,comment
"Hello","Привет","Greeting"
"World","Мир","World"
EOF

# Test conversion
./builddir/gettextify test.csv test.mo

# Verify MO file
msgunfmt test.mo
```

### Unit Tests (Future)

When unit tests are added:

```bash
meson test -C builddir
meson test -C builddir --verbose  # Verbose output
```

## License

By contributing to gettextify, you agree that your contributions will be licensed under the LGPL 3.0 license.

All source files must include the copyright header:

```cpp
// Copyright 2026 AnmiTaliDev <anmitalidev@nuros.org>
// This code licensed under LGPL 3.0
```

## Questions?

If you have questions about contributing, feel free to open an issue with the `question` label.

## Recognition

Contributors will be added to the [AUTHORS](AUTHORS) file. Please add yourself when submitting your first contribution!

---

Thank you for contributing to gettextify!
