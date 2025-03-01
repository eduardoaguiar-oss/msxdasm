

# Building MSXDasm

This guide provides step-by-step instructions for building **MSXDasm** from source. MSXDasm is a disassembler for MSX (Z80) binaries, written in C++. Follow these instructions to compile the project on your system.

---

## Table of Contents
1. [Prerequisites](#prerequisites)
2. [Building on Linux/macOS](#building-on-linuxmacos)
3. [Building on Windows](#building-on-windows)
4. [Troubleshooting](#troubleshooting)

---

## Prerequisites

Before building MSXDasm, ensure you have the following installed on your system:

- **C++ Compiler**: GCC, Clang, or MSVC (Visual Studio).
- **CMake**: Version 3.10 or higher.
- **Git**: For cloning the repository.

---

## Building on Linux/macOS

### Step 1: Clone the Repository
Clone the MSXDasm repository to your local machine:

```bash
git clone https://github.com/eduardoaguiar-oss/msxdasm.git
cd msxdasm
```

### Step 2: Create a Build Directory
Create a `build` directory to keep the source tree clean:

```bash
mkdir build
cd build
```

### Step 3: Run CMake
Generate the build files using CMake:

```bash
cmake ..
```

### Step 4: Build the Project
Compile the project using `make`:

```bash
make
```

### Step 5: (Optional) Install the Binary
Install the compiled binary to your system:

```bash
sudo make install
```

---

## Building on Windows

### Step 1: Clone the Repository
Clone the MSXDasm repository using Git:

```bash
git clone https://github.com/eduardoaguiar-oss/msxdasm.git
cd msxdasm
```

### Step 2: Generate Build Files with CMake
1. Open CMake GUI.
2. Set the **Source Directory** to the path of the cloned repository.
3. Set the **Build Directory** to a new folder (e.g., `build`) inside the repository.
4. Click **Configure** and select your compiler (e.g., Visual Studio).
5. Click **Generate** to create the build files.

### Step 3: Build the Project
1. Open the generated solution file (e.g., `MSXDasm.sln`) in Visual Studio.
2. Build the project by selecting **Build Solution** from the **Build** menu.

### Step 4: (Optional) Install the Binary
Run the `INSTALL` target in Visual Studio to install the binary.

---

## Troubleshooting

### Common Issues
1. **CMake Not Found**:
   - Ensure CMake is installed and added to your system's PATH.
   - On Linux/macOS, you can install CMake using your package manager:

     ```bash
     sudo apt install cmake  # Ubuntu/Debian
     brew install cmake      # macOS
     ```

2. **Compiler Not Found**:
   - Ensure you have a compatible C++ compiler installed.
   - On Windows, make sure Visual Studio or the necessary build tools are installed.

3. **Build Errors**:
   - Ensure all dependencies are installed.
   - Check the CMake output for any missing libraries or configuration issues.

### Getting Help
If you encounter issues not covered in this guide, please [open an issue](https://github.com/eduardoaguiar-oss/msxdasm/issues) on GitHub. Provide details about your system, the steps you followed, and any error messages you received.

---

## Conclusion
You should now have a working build of MSXDasm! If you have any questions or need further assistance, feel free to reach out to the maintainers or the community.
