#include "csv_utils.h"

#include <algorithm>
#include <cctype>
#include <filesystem>

namespace scheduler {

std::string trim(const std::string& value) {
    std::string result = value;
    // return an iterator to the first element that is not a space
    auto not_space = [](unsigned char ch) { return !std::isspace(ch); };
    result.erase(result.begin(), std::find_if(result.begin(), result.end(), not_space));
    result.erase(std::find_if(result.rbegin(), result.rend(), not_space).base(), result.end());
    return result;
}

std::vector<std::string> split_csv_row(const std::string& row) {
    std::vector<std::string> values;
    std::string current;
    bool in_quotes = false;

    for (std::size_t index = 0; index < row.size(); ++index) {
        const char ch = row[index];

        if (ch == '"') {
            // we encountered "" inside quotes => add a single "
            if (in_quotes && index + 1 < row.size() && row[index + 1] == '"') {
                current.push_back('"');
                ++index;
            } else { // we are just opening/closing new quotes
                in_quotes = !in_quotes;
            }
        } else if (ch == ',' && !in_quotes) { // new field
            values.push_back(trim(current));
            current.clear();
        } else { // regular character
            current.push_back(ch);
        }
    }

    values.push_back(trim(current));
    return values;
}

std::string escape_csv(const std::string& value) {

    // no special characters
    if (value.find_first_of(",\"") == std::string::npos) {
        return value;
    }

    // if special characters exits, wrap the whole value with quotes
    std::string escaped = "\"";
    for (const char ch : value) {
        if (ch == '"') { // " is transformed to ""
            escaped += "\"\"";
        } else {
            escaped.push_back(ch);
        }
    }
    escaped.push_back('"');
    return escaped;
}

void ensure_parent_directory(const std::string& path) {
    const std::filesystem::path file_path(path);
    if (file_path.has_parent_path()) {
        std::filesystem::create_directories(file_path.parent_path());
    }
}

}  // namespace scheduler
