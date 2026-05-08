#include "user.h"
#include "csv_utils.h"

#include <algorithm>
#include <fstream>
#include <map>
#include <stdexcept>

namespace scheduler {
namespace {

int parse_non_negative_int(const std::string& raw_value, const std::string& field_name, int line_number) {
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

double parse_probability(const std::string& raw_value, int line_number) {
    try {
        const double value = std::stod(raw_value);
        if (value < 0.0 || value > 1.0) {
            throw std::runtime_error("out of range");
        }
        return value;
    } catch (const std::exception&) {
        throw std::runtime_error("Invalid proficiency at line " + std::to_string(line_number));
    }
}

bool parse_bool_flag(const std::string& raw_value, int line_number) {
    if (raw_value == "1") {
        return true;
    }
    if (raw_value == "0") {
        return false;
    }
    throw std::runtime_error("Invalid solved flag at line " + std::to_string(line_number));
}

}  // namespace

bool UserProfile::has_attempted(const std::string& problem_id) const {
    return std::any_of(
        history.begin(),
        history.end(),
        [&problem_id](const AttemptRecord& attempt) { return attempt.problem_id == problem_id; }
    );
}

bool UserProfile::has_solved(const std::string& problem_id) const {
    return std::any_of(
        history.begin(),
        history.end(),
        [&problem_id](const AttemptRecord& attempt) {
            return attempt.problem_id == problem_id && attempt.solved;
        }
    );
}

double UserProfile::get_topic_proficiency(const std::string& topic) const {
    const auto iterator = topic_proficiency.find(topic);
    if (iterator == topic_proficiency.end()) {
        return 0.5;
    }
    return iterator->second;
}

UserProfile load_user_profile(
    const std::string& profile_path,
    const std::string& topic_path,
    const std::string& history_path
) {
    UserProfile profile;

    {
        std::ifstream profile_input(profile_path);
        if (!profile_input) {
            throw std::runtime_error("Unable to open user profile file: " + profile_path);
        }

        std::string header;
        std::getline(profile_input, header);

        std::string row;
        if (!std::getline(profile_input, row)) {
            throw std::runtime_error("User profile file is empty: " + profile_path);
        }

        const auto values = split_csv_row(row);
        if (values.size() != 2) {
            throw std::runtime_error("User profile row must contain username and skill_level");
        }

        profile.username = trim(values[0]);
        profile.skill_level = parse_non_negative_int(values[1], "skill_level", 2);
        if (profile.username.empty()) {
            throw std::runtime_error("Username cannot be empty");
        }
    }

    {
        std::ifstream topic_input(topic_path);
        if (!topic_input) {
            throw std::runtime_error("Unable to open topic proficiency file: " + topic_path);
        }

        std::string header;
        std::getline(topic_input, header);

        std::string row;
        int line_number = 1;
        while (std::getline(topic_input, row)) {
            ++line_number;
            if (trim(row).empty()) {
                continue;
            }

            const auto values = split_csv_row(row);
            if (values.size() != 2) {
                throw std::runtime_error("Invalid topic proficiency row at line " + std::to_string(line_number));
            }

            const std::string topic = trim(values[0]);
            if (topic.empty()) {
                throw std::runtime_error("Topic cannot be empty at line " + std::to_string(line_number));
            }

            profile.topic_proficiency[topic] = parse_probability(values[1], line_number);
        }
    }

    {
        std::ifstream history_input(history_path);
        if (!history_input) {
            throw std::runtime_error("Unable to open user history file: " + history_path);
        }

        std::string header;
        std::getline(history_input, header);

        std::string row;
        int line_number = 1;
        while (std::getline(history_input, row)) {
            ++line_number;
            if (trim(row).empty()) {
                continue;
            }

            const auto values = split_csv_row(row);
            if (values.size() != 4) {
                throw std::runtime_error("Invalid history row at line " + std::to_string(line_number));
            }

            AttemptRecord attempt;
            attempt.problem_id = trim(values[0]);
            attempt.solved = parse_bool_flag(values[1], line_number);
            attempt.time_taken = parse_non_negative_int(values[2], "time_taken", line_number);
            attempt.wrong_submissions =
                parse_non_negative_int(values[3], "wrong_submissions", line_number);

            if (attempt.problem_id.empty()) {
                throw std::runtime_error("History problem_id cannot be empty at line " + std::to_string(line_number));
            }

            profile.history.push_back(std::move(attempt));
        }
    }

    return profile;
}

void save_user_profile(
    const std::string& profile_path,
    const std::string& topic_path,
    const std::string& history_path,
    const UserProfile& profile
) {
    ensure_parent_directory(profile_path);
    ensure_parent_directory(topic_path);
    ensure_parent_directory(history_path);

    {
        std::ofstream profile_output(profile_path);
        if (!profile_output) {
            throw std::runtime_error("Unable to write user profile file: " + profile_path);
        }

        profile_output << "username,skill_level\n";
        profile_output << escape_csv(profile.username) << ',' << profile.skill_level << '\n';
    }

    {
        std::ofstream topic_output(topic_path);
        if (!topic_output) {
            throw std::runtime_error("Unable to write topic proficiency file: " + topic_path);
        }

        topic_output << "topic,proficiency\n";
        std::map<std::string, double> sorted_topics(
            profile.topic_proficiency.begin(),
            profile.topic_proficiency.end()
        );
        for (const auto& [topic, proficiency] : sorted_topics) {
            topic_output << escape_csv(topic) << ',' << proficiency << '\n';
        }
    }

    {
        std::ofstream history_output(history_path);
        if (!history_output) {
            throw std::runtime_error("Unable to write user history file: " + history_path);
        }

        history_output << "problem_id,solved,time_taken,wrong_submissions\n";
        for (const AttemptRecord& attempt : profile.history) {
            history_output
                << escape_csv(attempt.problem_id) << ','
                << (attempt.solved ? 1 : 0) << ','
                << attempt.time_taken << ','
                << attempt.wrong_submissions << '\n';
        }
    }
}

}  // namespace scheduler
