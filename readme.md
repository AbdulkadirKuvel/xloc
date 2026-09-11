# xloc - a Code Counter Made in C++ for Pure Speed

![C++](https://img.shields.io/badge/C++-23-blue.svg)
![License](https://img.shields.io/badge/License-MIT-green.svg)

**xloc** is aiming to be blazing-fast, modern C++ Command Line Interface (CLI) utility designed for analyzing and counting lines of code, comments, and blank lines across large-scale codebases. 

Built with **systems programming** principles in mind, it leverages C++23 features to deliver maximum performance through zero-cost abstractions, memory safety, and highly optimized I/O operations.

Please note that this tool is under development. The current code here works fine but lacks a lot of features.

*Some of this readme is created by generative AI, sorry if it feels a bit boring.*

---

## ✨ Features

*   **Zero-Cost String Parsing:** Utilizes `std::string_view` extensively to eliminate unnecessary memory allocations and copying during file and configuration parsing, strictly preventing dangling references.
*   **Extensible Architecture (SOLID):** Implements the **Strategy Pattern** for report generation. Output logic is decoupled using a pure virtual `IReportFormatter` interface and `std::ostream` polymorphism, allowing seamless additions of new formats without modifying core logic.
*   **Multiple Output Formats:** Generate code statistics in beautifully formatted CLI Tables (with custom ANSI colors), JSON, or Markdown files.
*   **Robust CLI Parser:** A defensive and strict command-line argument parser that handles missing arguments, invalid paths, and directory/file distinctions securely.
*   **Modern C++ Standards:** Makes heavy use of C++17/20/23 features including `std::filesystem`, `std::println`, `std::unique_ptr` (RAII), and structured bindings.

---

## 🏗️ Architecture

CodeCounter is built on **Clean Architecture** principles. The core business logic (analyzing files) is strictly separated from the presentation layer (formatting output).

### The Reporting Strategy

Depending on the user's CLI arguments (`--json`, `--m`, or default table), the corresponding formatter (`JsonFormatter`, or `TableFormatter`, more to come...) is instantiated and passed to either `std::cout` or a `std::ofstream`.

---

## Roadmap / Upcoming Features

We are currently working on constantly pushing the boundaries of performance. The following features will be implemented in near time:

* **Multithreading & Concurrency:** Distributing file I/O and parsing workloads across multiple CPU cores using `std::thread`, `std::mutex`, and `std::atomic` to prevent race conditions.
* **Memory-Mapped Files (mmap):** Bypassing standard `std::ifstream` overhead by mapping files directly into RAM and utilizing `reinterpret_cast` for raw byte processing, aiming to parse millions of lines in milliseconds.

---

## 🛠️ Build Instructions

### 1. Use Executable

If you do not want the source code, just: 
1. Download the executable file provided to your desired location
2. Add to your path if wanted
3. Run by `xloc [\<commands\>]` 

### 2. Build From Source

If you prefer to compile the tool yourself from the source code:

A. Clone the repository (or download the .zip)

```Bash
git clone https://github.com/AbdulkadirKuvel/xloc.git
cd xloc
```
B. Build the project

For Linux and macOS:
```Bash
make
```
For Windows (MinGW):
```Bash
mingw32-make
```

C. The compiled xloc executable will be generated in the bin folder. You can now run it locally or move it to a directory included in your system's PATH.

Optional: Move to a PATH directory (Linux/macOS example)

```Bash
sudo cp xloc /usr/local/bin/
```
#### Prerequisites

* A C++23 compatible compiler (GCC 13+, Clang 16+, or MSVC 19.38+)

* Build System: make (GNU Make) installed. (Use mingw32-make on Windows). 

### TODOS to Myself for Future

[ ] Implement Multithreading.

[ ] Implement Memory maps.

[ ] Create more accurate lexers

[x] Realize C style single line comments can be multiline with the use of "\\", for example:

```C
int main() {
    // Single line comment
    bool var = true;

    // Single line comment \
    sike!
    return 0;
}

```