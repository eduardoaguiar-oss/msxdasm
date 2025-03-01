
---

# MSXDasm

[![License](https://img.shields.io/badge/license-GPL%20v2-blue.svg)](LICENSE)
[![GitHub Issues](https://img.shields.io/github/issues/eduardoaguiar-oss/msxdasm)](https://github.com/eduardoaguiar-oss/msxdasm/issues)
[![GitHub Stars](https://img.shields.io/github/stars/eduardoaguiar-oss/msxdasm)](https://github.com/eduardoaguiar-oss/msxdasm/stargazers)

**MSXDasm** is an open-source disassembler for MSX (Z80) binaries, written in C++. It is designed to help developers and enthusiasts analyze and understand MSX ROMs, binaries, and other Z80 machine code.

---

## Table of Contents
1. [Features](#features)
2. [Installation](#installation)
3. [Usage](#usage)
4. [Contributing](#contributing)
5. [License](#license)

---

## Features
- **Code Navigator**: A powerful opcode navigator that distinguishes between code and data, enabling precise analysis of binary files.
- **Z80 Disassembly**: Accurately disassembles Z80 machine code into human-readable assembly, including not documented opcodes.
- **MSX ROM Support**: Designed specifically for MSX ROMs and binaries.
- **Cross-Platform**: Works on Windows, macOS, and Linux.
- **Customizable Output**: Supports various output formats and configurations.
- **Open Source**: Fully open-source and community-driven.

---

## Installation

### Prerequisites
- A C++ compiler (e.g., GCC, Clang, or MSVC).
- CMake (version 3.10 or higher).

### Steps
1. Clone the repository:

   ```git clone https://github.com/eduardoaguiar-oss/msxdasm.git
   cd msxdasm
   ```

2. Build the project:

   ```mkdir build
   cd build
   cmake ..
   make
   ```

3. (Optional) Install the binary:

   ```sudo make install
   ```

---

## Usage

### Basic Usage
To disassemble an MSX ROM or binary, run:

```bash
msxdasm [options] <input_file>
```

Example:

```bash
msxdasm kvalley.rom -o kvalley.asm
```

### Command-Line Options
Below is a list of available command-line options:

| Option                  | Description                                                                 |
|-------------------------|-----------------------------------------------------------------------------|
| `-d <definition_file>`  | Specify an address definition file (e.g., `msxrom.def`). Can be used multiple times.   |
| `-e <entry_point>`      | Set the execution entry point (e.g., `-e 406c`). Default: ROM entry point.  |
| `-o <output_file>`      | Specify the output file for the disassembled code. Can be used multiple times, one for each output format.  |
| `-p <entry_point>`      | Add another code entry points, for unreachable code. Can be used multiple times.  |
| `-s <start_address>`    | Set the ROM start (ORG) address (e.g., `-s 4000`).                        |
| `-h`                    | Show the help message and exit.                                             |

### Output formats

- **.asm**: Z80 assembly code.
- **.lst**: Z80 assembly code with opcode listing and addresses for each instruction.
- **.hex**: Hex dump, with opcodes highlighted.

### Examples

1. **Disassemble a ROM with default settings:**

   ```bash
   msxdasm -o kvalley.asm kvalley.rom
   ```

2. **Generate an asm full listing instead of assembly:**

   ```bash
   msxdasm -o tennis.lst tennis.rom
   ```

3. **Generate a hex dump instead of assembly:**

   ```bash
   msxdasm -o YieArKungFu.lst YieArKungFu.rom
   ```

4. **Generate both .asm and .lst:**

   ```bash
   msxdasm -o YieArKungFu.lst -o YieArKungFu.asm YieArKungFu.rom
   ```

5. **Use an address definition file:**

   ```bash
   msxdasm -d msxrom.def -o frogger.asm frogger.rom
   ```

6. **Set the execution entry point:**

   ```bash
   msxdasm -e 406c -o kvalley.asm kvalley.rom
   ```

7. **Set the ROM start address:**

   ```bash
   msxdasm -s 4000 -o golf.asm golf.rom
   ```

---

## Support This Project
If you find **MSXDasm** useful and would like to support its development, consider donating cryptocurrency. Your contributions help maintain and improve the project.

### Donation Addresses
- **Bitcoin (BTC)**: `bc1qa92rvaru86hr5lup2a6ewqqgnran3p2qf3djah`
- **Litecoin (LTC)**: `LhiBHWgrqysZsoZfiuc3tUyzFRWwmDZipo`
- **Monero (XMR)**: `42DmNUJ3yQ7bByCgkcyx8nVScC7HrJd83DxUYWPqNo7oQaq8Tnu3BqAQ57J7tRGFkW7wYjzukphLLA5gJXaF9paGJ5X17LD`

Thank you for your support! Every contribution makes a difference.

---

## Contributing
We welcome contributions to MSXDasm! Please read our [CONTRIBUTING.md](CONTRIBUTING.md) file for guidelines on how to contribute, including:

- Reporting issues.
- Suggesting enhancements.
- Submitting pull requests.

---

## License
MSXDasm is licensed under the **GNU General Public License v2 (GPL v2)**. See the [LICENSE](LICENSE) file for details.

---

## Acknowledgments
- The MSX community for their support and inspiration.
- Contributors who have helped improve MSXDasm.
- Mariæ Beatæ Virgini!

---

Thank you for using MSXDasm! If you have any questions or feedback, feel free to [open an issue](https://github.com/eduardoaguiar-oss/msxdasm/issues) or reach out to the maintainers.
