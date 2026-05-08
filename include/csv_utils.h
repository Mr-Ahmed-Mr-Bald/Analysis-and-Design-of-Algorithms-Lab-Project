#pragma once

#include <string>
#include <vector>

namespace scheduler {

// remove spaces from the beginning of a string
std::string trim(const std::string& value);
// turn a csv row string into a sequence of tokens
std::vector<std::string> split_csv_row(const std::string& row);
// format string to be safely written to csv file
std::string escape_csv(const std::string& value);
// ensure that the directory path leading to a file exists
void ensure_parent_directory(const std::string& path);

}  // namespace scheduler
