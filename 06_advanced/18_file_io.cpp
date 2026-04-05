/*
================================================================================
  LESSON 18: File I/O — Complete Guide
================================================================================

  THEORY:
  -------
  File I/O in C++ uses STREAMS — same concept as cin/cout but connected to files.

  STREAM CLASSES:
  ---------------
  ifstream  — input file stream (reading)
  ofstream  — output file stream (writing)
  fstream   — both input and output

  FILE OPEN MODES (flags):
  ------------------------
  ios::in       — open for reading (default for ifstream)
  ios::out      — open for writing (default for ofstream, truncates file!)
  ios::app      — append to end of file
  ios::ate      — open and seek to end
  ios::trunc    — truncate file to zero length
  ios::binary   — binary mode (no newline translation)

  Combine with |: ios::out | ios::binary

  STREAM STATE FLAGS:
  -------------------
  good()  — no errors
  eof()   — end of file reached
  fail()  — logical error (wrong type, etc.)
  bad()   — read/write error

  BUFFERING:
  ----------
  File I/O is buffered — data is collected in memory and written in chunks.
  flush() or close() forces the buffer to be written to disk.

================================================================================
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <iomanip>

// =========================================================================
// HELPER: print file contents
// =========================================================================
void print_file(const std::string& filename) {
    std::ifstream f(filename);
    if (!f.is_open()) { std::cout << "Cannot open " << filename << std::endl; return; }
    std::cout << "--- " << filename << " ---" << std::endl;
    std::string line;
    while (std::getline(f, line)) {
        std::cout << line << std::endl;
    }
    std::cout << "---" << std::endl;
}

int main() {

    // =========================================================================
    // SECTION 1: WRITING TO A FILE
    // =========================================================================

    std::cout << "=== WRITING ===" << std::endl;

    // ofstream: creates file if not exists, TRUNCATES if exists
    std::ofstream outfile("lesson18_output.txt");

    if (!outfile.is_open()) {
        std::cerr << "ERROR: Could not open file for writing!" << std::endl;
        return 1;
    }

    outfile << "Line 1: Hello, File!" << std::endl;
    outfile << "Line 2: C++ File I/O" << std::endl;
    outfile << "Line 3: Numbers: " << 42 << " " << 3.14 << std::endl;

    // Formatting works just like cout:
    outfile << std::setw(10) << "Name" << std::setw(6) << "Age" << std::endl;
    outfile << std::setw(10) << "Alice"  << std::setw(6) << 30  << std::endl;
    outfile << std::setw(10) << "Bob"    << std::setw(6) << 25  << std::endl;

    outfile.close();  // flush and close
    std::cout << "Written to lesson18_output.txt" << std::endl;
    print_file("lesson18_output.txt");

    // APPEND mode — add to existing file:
    std::ofstream appendfile("lesson18_output.txt", std::ios::app);
    appendfile << "Line 4: Appended!" << std::endl;
    appendfile.close();
    std::cout << "After append:" << std::endl;
    print_file("lesson18_output.txt");

    // =========================================================================
    // SECTION 2: READING FROM A FILE
    // =========================================================================

    std::cout << "\n=== READING ===" << std::endl;

    // --- Read line by line ---
    std::ifstream infile("lesson18_output.txt");
    if (!infile) {  // shorthand for !infile.is_open()
        std::cerr << "Cannot open file!" << std::endl;
        return 1;
    }

    std::cout << "Line by line:" << std::endl;
    std::string line;
    int line_num = 1;
    while (std::getline(infile, line)) {
        std::cout << line_num++ << ": " << line << std::endl;
    }
    infile.close();

    // --- Read word by word ---
    std::ofstream wfile("words.txt");
    wfile << "apple banana cherry date elderberry fig" << std::endl;
    wfile.close();

    std::ifstream wread("words.txt");
    std::cout << "\nWord by word:" << std::endl;
    std::string word;
    int count = 0;
    while (wread >> word) {
        std::cout << ++count << ". " << word << std::endl;
    }
    wread.close();

    // --- Read character by character ---
    std::ifstream cread("words.txt");
    std::cout << "\nChar by char (first 10): ";
    char ch;
    int char_count = 0;
    while (cread.get(ch) && char_count < 10) {
        std::cout << ch;
        char_count++;
    }
    std::cout << std::endl;
    cread.close();

    // =========================================================================
    // SECTION 3: CSV PARSING
    // =========================================================================

    std::cout << "\n=== CSV PARSING ===" << std::endl;

    // Write CSV:
    std::ofstream csv("students.csv");
    csv << "Name,Age,Grade,Score\n";
    csv << "Alice,20,A,95.5\n";
    csv << "Bob,22,B,87.3\n";
    csv << "Charlie,21,A,92.1\n";
    csv << "Diana,23,C,74.8\n";
    csv.close();

    // Parse CSV:
    struct Student {
        std::string name;
        int age;
        char grade;
        double score;
    };

    std::vector<Student> students;
    std::ifstream csv_read("students.csv");
    std::string header;
    std::getline(csv_read, header);  // skip header line
    std::cout << "Header: " << header << std::endl;

    while (std::getline(csv_read, line)) {
        std::stringstream ss(line);
        Student s;
        std::string age_str, grade_str, score_str;

        std::getline(ss, s.name,  ',');
        std::getline(ss, age_str, ',');
        std::getline(ss, grade_str, ',');
        std::getline(ss, score_str);

        s.age   = std::stoi(age_str);
        s.grade = grade_str[0];
        s.score = std::stod(score_str);

        students.push_back(s);
    }
    csv_read.close();

    std::cout << std::left;
    std::cout << std::setw(12) << "Name" << std::setw(6) << "Age"
              << std::setw(8) << "Grade" << std::setw(8) << "Score" << std::endl;
    std::cout << std::string(34, '-') << std::endl;
    for (const auto& s : students) {
        std::cout << std::setw(12) << s.name << std::setw(6) << s.age
                  << std::setw(8) << s.grade << std::setw(8) << s.score << std::endl;
    }

    // =========================================================================
    // SECTION 4: BINARY FILES
    // =========================================================================

    std::cout << "\n=== BINARY FILES ===" << std::endl;

    struct Record {
        int id;
        char name[32];
        double value;
    };

    // Write binary:
    std::ofstream bin_out("data.bin", std::ios::binary);
    Record records[] = {
        {1, "Alice", 95.5},
        {2, "Bob",   87.3},
        {3, "Charlie", 92.1}
    };
    bin_out.write(reinterpret_cast<char*>(records), sizeof(records));
    bin_out.close();
    std::cout << "Written " << sizeof(records) << " bytes to data.bin" << std::endl;

    // Read binary:
    std::ifstream bin_in("data.bin", std::ios::binary);
    Record read_records[3];
    bin_in.read(reinterpret_cast<char*>(read_records), sizeof(read_records));
    bin_in.close();

    std::cout << "Read from binary:" << std::endl;
    for (const auto& r : read_records) {
        std::cout << "  ID=" << r.id << " Name=" << r.name << " Value=" << r.value << std::endl;
    }

    // =========================================================================
    // SECTION 5: FILE SEEKING
    // =========================================================================

    std::cout << "\n=== FILE SEEKING ===" << std::endl;

    // seekg = seek get (read position)
    // seekp = seek put (write position)
    // tellg = tell get position
    // tellp = tell put position

    std::ifstream seek_file("lesson18_output.txt");

    // Get file size:
    seek_file.seekg(0, std::ios::end);   // seek to end
    long file_size = seek_file.tellg();  // get position (= file size)
    seek_file.seekg(0, std::ios::beg);   // seek back to beginning
    std::cout << "File size: " << file_size << " bytes" << std::endl;

    // Read from specific position:
    seek_file.seekg(7);  // skip first 7 bytes
    std::string rest;
    std::getline(seek_file, rest);
    std::cout << "From position 7: " << rest << std::endl;
    seek_file.close();

    // =========================================================================
    // SECTION 6: STRING STREAMS
    // =========================================================================

    std::cout << "\n=== STRING STREAMS ===" << std::endl;

    // istringstream — parse a string like a file:
    std::string data = "Alice 30 95.5";
    std::istringstream iss(data);
    std::string sname; int sage; double sscore;
    iss >> sname >> sage >> sscore;
    std::cout << "Parsed: " << sname << " age=" << sage << " score=" << sscore << std::endl;

    // Parse CSV line:
    std::string csv_line = "Bob,25,87.3";
    std::istringstream csv_ss(csv_line);
    std::string token;
    std::vector<std::string> tokens;
    while (std::getline(csv_ss, token, ',')) {
        tokens.push_back(token);
    }
    std::cout << "CSV tokens: ";
    for (const auto& t : tokens) std::cout << "[" << t << "] ";
    std::cout << std::endl;

    // ostringstream — build a string like writing to a file:
    std::ostringstream oss;
    oss << "Name: " << "Alice" << ", Age: " << 30 << ", Score: " << std::fixed << std::setprecision(1) << 95.5;
    std::string built = oss.str();
    std::cout << "Built: " << built << std::endl;

    // Use ostringstream for string formatting (like sprintf):
    auto format_record = [](int id, const std::string& name, double val) -> std::string {
        std::ostringstream os;
        os << "[" << std::setw(3) << std::setfill('0') << id << "] "
           << std::setw(10) << std::setfill(' ') << std::left << name
           << " = " << std::fixed << std::setprecision(2) << val;
        return os.str();
    };

    std::cout << format_record(1, "Alice", 95.5) << std::endl;
    std::cout << format_record(42, "Bob", 87.333) << std::endl;

    // =========================================================================
    // SECTION 7: ERROR HANDLING
    // =========================================================================

    std::cout << "\n=== ERROR HANDLING ===" << std::endl;

    std::ifstream bad_file("nonexistent_file.txt");
    if (!bad_file) {
        std::cerr << "File not found!" << std::endl;
    }

    // Check stream state:
    std::ifstream test("lesson18_output.txt");
    std::cout << "good: " << test.good() << std::endl;
    std::cout << "fail: " << test.fail() << std::endl;
    std::cout << "eof:  " << test.eof()  << std::endl;

    // Read past end:
    std::string all_content((std::istreambuf_iterator<char>(test)),
                             std::istreambuf_iterator<char>());
    std::cout << "After reading all: eof=" << test.eof() << std::endl;
    test.close();

    return 0;
}
