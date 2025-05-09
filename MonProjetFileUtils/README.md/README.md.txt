# SysMon-CPP Project

## Description
This project demonstrates a modular C++ file reader using best practices like namespace usage, exception handling, and file cleanup.

## Project Structure

- `FileUtils.h` / `FileUtils.cpp`: Contains the `readFileLines` function that reads a text file line-by-line into a vector.
- `main.cpp`: Demonstrates reading a file and displaying its contents. Cleans up the file after reading.
- `test_FileUtils.cpp`: A basic unit test that ensures `readFileLines` works as expected.
- `SysMon-cpp.png`: A system monitoring architecture image (describe its use here).

## Features

- Modular code structure using namespaces.
- Error handling with clear messages (includes filename).
- Handles empty files gracefully.
- Avoids global `using namespace std;`.
- Test file cleanup (`test.txt` is deleted after use).
- Written entirely in English.

## How to Compile & Run

```bash
g++ -o main main.cpp FileUtils.cpp
./main

g++ -o test test_FileUtils.cpp FileUtils.cpp
./test
