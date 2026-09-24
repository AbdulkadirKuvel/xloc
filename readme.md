# xloc - a Code Counter Made in C++ for Pure Speed

![C++](https://img.shields.io/badge/C++-23-blue.svg)
![License](https://img.shields.io/badge/License-MIT-green.svg)

A high-performance, modern C++23 Command Line Interface (CLI) utility engineered to parse and analyze lines of code, comments, and blank lines across large-scale codebases. 

Built with **systems programming** principles in mind, **xloc** leverages C++23 features to deliver maximum performance through zero-cost abstractions, memory safety, and highly optimized I/O operations.

Note: xloc is currently in active development. While the core parsing engine is stable and functional, the feature set is being continuously expanded.

---

## ✨ Features

*   **Zero-Cost String Parsing:** Utilizes `std::string_view` extensively to eliminate unnecessary memory allocations and copying during file and configuration parsing, strictly preventing dangling references.
*   **Extensible Architecture (SOLID):** Implements the **Strategy Pattern** for report generation. Output logic is decoupled using a pure virtual `IReportFormatter` interface and `std::ostream` polymorphism, allowing seamless additions of new formats without modifying core logic.
*   **Multiple Output Formats:** Generates comprehensive code statistics across multiple standardized formats, including ANSI-escaped CLI tables, structured JSON, and Markdown.
*   **Robust CLI Parser:** A defensive and strict command-line argument parser that handles missing arguments, invalid paths, and directory/file distinctions securely.
*   **Modern C++ Standards:** Makes heavy use of C++17/20/23 features including `std::filesystem`, `std::println`, `std::unique_ptr` (RAII), and structured bindings.

---

## 🏗️ Architecture

CodeCounter is built on **Clean Architecture** principles. The core business logic (analyzing files) is strictly separated from the presentation layer (formatting output).

### The Reporting Strategy

Depending on the user's CLI arguments (`--json`, `--m`, or default table), the corresponding formatter (`JsonFormatter`, or `TableFormatter`, more to come...) is instantiated and passed to either `std::cout` or a `std::ofstream`.

---

## Roadmap / Upcoming Features

Performance optimization is an ongoing priority. The following architectural enhancements are scheduled for upcoming releases:

* **Multithreading & Concurrency:** Distributing file I/O and parsing workloads across multiple CPU cores using `std::thread`, `std::mutex`, and `std::atomic` to prevent race conditions.
* **Memory-Mapped Files (mmap):** Bypassing standard `std::ifstream` overhead by mapping files directly into RAM and utilizing `reinterpret_cast` for raw byte processing, aiming to parse millions of lines in milliseconds.

---

## 🛠️ Build Instructions

### A. Pre-compiled Binaries (Executable)

For standalone usage without compiling from source:

1. Download the latest executable from the releases page.
2. (Optional) Add the executable to your system's PATH environment variable.
3. Execute via `xloc [options]`.

To see all commands, type in `xloc --help`

If you prefer not to deal with compiler and the version, this should be your pick.

### B. Build From Source

If you prefer to compile the tool yourself from the source code:

1. Download the latest source code from releases page

2. Build the project

For Linux and macOS:
```Bash
make
```
For Windows (MinGW):
```Bash
mingw32-make
```

3. The compiled xloc executable will be generated in the bin folder. You can now run it locally or move it to a directory included in your system's PATH.

Optional: Move to a PATH directory (Linux/macOS example)

```Bash
sudo cp xloc /usr/local/bin/
```

### Execution

#### Target Directory

- Current directory: `xloc`
- Specific directory: `xloc <path>` 

#### Recursive traversal

- Use the `-r` or `--recursive` flag to recursively traverse and alayze all subdirectories.

  - Current directory & subdirectories: `xloc -r`
  - Specific directory & subdirectories: `xloc <path> -r` 

#### Output Redirection

- Use the `-o <outputpath>` or `--output <outputpath>` flag to write the analysis results directly to a file.

  - `xloc -o <outputpath>`

#### Formatting

- Use the `-j` or `--json` flag to format output as a json.

  - `xloc -j`


- Use the `-m` or `--markdown` flag to format output as a markdown.

  - `xloc -m`

#### Configuration

- Use config flag `-c <configpath>` or `--config <configpath>` to utilize a config.json file

  - `xloc -c <configpath>`

A configuration file can only be a json file.

```json
{
    "recursive": false, // Toggles recursive director traversal
    "blacklist": [], // add directories to exclude from analysis
    "whitelist": []  // add files and directories to include in analysis
}
```

#### Prerequisites To Run Source Code

* A C++23 compatible compiler (GCC 13+, Clang 16+, or MSVC 19.38+)

* Build System: make (GNU Make) installed. (Use mingw32-make on Windows). 

### Task List

- [ ] Implement Multithreading.
    - Improves speed.

- [x] Implement Memory maps.
    - Improves speed.
    - Allows reading bigger sized files.

- [ ] Seperate Docstring From Comments 
    - Creates more accurate lexers.

- [ ] Improve Config File
    - Lets user have more control.

- [x] Implement Benchmarking
    - Show how much time consumed for each step.

- [x] Fix C style single line comments can be multiline with the use of "\\", for example:

```C
int main() {
    // Single line comment.
    bool var = true;

    // Single line comment \
    sike! this line is also considered a comment.
    return 0;
}

```