#include "problem.h"
#include "csv_utils.h"

#include <fstream>
#include <sstream>
#include <stdexcept>
#include <unordered_set>

namespace scheduler {
namespace {

int parse_positive_int(const std::string& raw_value, const std::string& field_name, int line_number) {
    try {
        const int value = std::stoi(raw_value);
        if (value < 0) {
            throw std::runtime_error("negative value");
        }
        return value;
    } catch (const std::exception&) {
        throw std::runtime_error(
            "Invalid " + field_name + " at line " + std::to_string(line_number)
        );
    }
}

}  // namespace

std::vector<std::string> split_topics(const std::string& raw_topics) {
    std::vector<std::string> topics;
    std::stringstream stream(raw_topics);
    std::string topic;

    // topics are separated by |
    while (std::getline(stream, topic, '|')) {
        topic = trim(topic);
        if (!topic.empty()) {
            topics.push_back(topic);
        }
    }

    return topics;
}

std::string join_topics(const std::vector<std::string>& topics) {
    std::ostringstream output;
    for (std::size_t index = 0; index < topics.size(); ++index) {
        if (index > 0) {
            output << '|';
        }
        output << topics[index];
    }
    return output.str();
}

std::vector<Problem> load_problems_from_csv(const std::string& path) {
    std::ifstream input(path);
    if (!input) {
        throw std::runtime_error("Unable to open problem file: " + path);
    }

    std::string header;
    std::getline(input, header);

    std::vector<Problem> problems;
    std::unordered_set<std::string> ids;
    std::string row;
    int line_number = 1;

    while (std::getline(input, row)) {
        ++line_number;
        if (trim(row).empty()) {
            continue;
        }

        const auto values = split_csv_row(row);
        if (values.size() != 4) {
            throw std::runtime_error("Invalid problem row at line " + std::to_string(line_number));
        }

        Problem problem;
        problem.id = trim(values[0]);
        problem.difficulty = parse_positive_int(values[1], "difficulty", line_number);
        problem.estimated_time = parse_positive_int(values[2], "estimated_time", line_number);
        problem.topics = split_topics(values[3]);

        if (problem.id.empty()) {
            throw std::runtime_error("Problem ID cannot be empty at line " + std::to_string(line_number));
        }
        if (problem.estimated_time == 0) {
            throw std::runtime_error("Problem estimated_time must be positive at line " + std::to_string(line_number));
        }
        if (problem.topics.empty()) {
            throw std::runtime_error("Problem topics cannot be empty at line " + std::to_string(line_number));
        }
        if (!ids.insert(problem.id).second) {
            throw std::runtime_error("Duplicate problem ID found: " + problem.id);
        }

        problems.push_back(problem);
    }

    return problems;
}

void save_problems_to_csv(const std::string& path, const std::vector<Problem>& problems) {
    ensure_parent_directory(path);
    std::ofstream output(path);
    if (!output) {
        throw std::runtime_error("Unable to write problem file: " + path);
    }

    output << "id,difficulty,estimated_time,topics\n";
    for (const Problem& problem : problems) {
        output
            << escape_csv(problem.id) << ','
            << problem.difficulty << ','
            << problem.estimated_time << ','
            << escape_csv(join_topics(problem.topics)) << '\n';
    }
}

}  // namespace scheduler
