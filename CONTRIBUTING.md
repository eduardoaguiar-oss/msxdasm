---

# Contributing to MSXDasm

Thank you for your interest in contributing to **MSXDasm**! We welcome contributions from the community, whether it's reporting bugs, suggesting new features, improving documentation, or submitting code changes.

This guide will help you get started with contributing to the project.

---

## Table of Contents
1. [How to Contribute](#how-to-contribute)
   - [Reporting Issues](#reporting-issues)
   - [Suggesting Enhancements](#suggesting-enhancements)
   - [Submitting Pull Requests](#submitting-pull-requests)
2. [Setting Up the Development Environment](#setting-up-the-development-environment)
3. [Coding Guidelines](#coding-guidelines)
4. [Commit Message Format](#commit-message-format)
5. [License](#license)

---

## How to Contribute

### Reporting Issues
If you encounter a bug or have a question about the project, please [open an issue](https://github.com/eduardoaguiar-oss/msxdasm/issues) on GitHub. When reporting an issue, please include:

- A clear and descriptive title.
- Steps to reproduce the issue.
- Expected vs. actual behavior.
- Screenshots or logs (if applicable).
- Your operating system and environment details.

### Suggesting Enhancements
We welcome suggestions for new features or improvements! To suggest an enhancement:

1. [Open an issue](https://github.com/eduardoaguiar-oss/msxdasm/issues) with the "enhancement" label.
2. Describe the feature or improvement in detail.
3. Explain why it would be valuable to the project.

### Submitting Pull Requests
If you'd like to contribute code, please follow these steps:

1. Fork the repository to your GitHub account.
2. Create a new branch for your changes:
   ```git checkout -b feature/your-feature-name
   ```
3. Make your changes and ensure they follow the [Coding Guidelines](#coding-guidelines).
4. Commit your changes with a clear and descriptive commit message (see [Commit Message Format](#commit-message-format)).
5. Push your branch to your forked repository:
   ```git push origin feature/your-feature-name
   ```
6. Open a pull request (PR) against the `main` branch of the [MSXDasm repository](https://github.com/eduardoaguiar-oss/msxdasm).
7. Provide a detailed description of your changes in the PR, including:
   - The purpose of the changes.
   - Any related issues or PRs.
   - Screenshots or test results (if applicable).

---

## Setting Up the Development Environment
To set up the project locally for development, follow these steps:

1. **Clone the repository:**

   ```
   git clone https://github.com/eduardoaguiar-oss/msxdasm.git
   cd msxdasm
   ```

2. **Install dependencies:**
   - Ensure you have the required tools installed (e.g., C++ compiler, build tools).
   - Follow the build instructions in the [README.md](README.md).

3. **Build the project:**
   - Use the provided build system (e.g., Makefile, CMake) to compile the project.
   - Example:

     ```make
     ```

4. **Run tests:**
   - If the project includes tests, run them to ensure everything works as expected:

     ```make test
     ```

---

## Coding Guidelines
To maintain consistency across the codebase, please adhere to the following guidelines:

- Follow the existing code style (e.g., indentation, naming conventions).
- Write clear and concise comments where necessary.
- Ensure your code is well-tested.
- Keep functions and classes focused and modular.

---

## Commit Message Format
Please follow this format for commit messages:

```
<type>: <description>

[optional body]

[optional footer]
```

- **Type**: The type of change (e.g., `feat`, `fix`, `docs`, `style`, `refactor`, `test`, `chore`).
- **Description**: A concise summary of the changes.
- **Body**: (Optional) A detailed explanation of the changes.
- **Footer**: (Optional) References to related issues or PRs.

Example:

```
feat: add support for disassembling Z80 opcodes

This commit adds a new module for disassembling Z80 opcodes, which is required for feature X.

Closes #123
```

---

## License
By contributing to MSXDasm, you agree that your contributions will be licensed under the project's [LICENSE](LICENSE).

---

Thank you for contributing to MSXDasm! Your efforts help make this project better for everyone.

---

This `CONTRIBUTING.md` file provides clear instructions for contributors and ensures a smooth collaboration process. You can customize it further to fit the specific needs of the MSXDasm project.
