#pragma once

#include <string>
#include <unordered_map>
#include <vector>

namespace scheduler {

struct AttemptRecord {
    std::string problem_id;
    bool solved = false;
    int time_taken = 0;
    int wrong_submissions = 0;
};

struct UserProfile {
    std::string username;
    int skill_level = 0;
    std::unordered_map<std::string, double> topic_proficiency;

    // has the user attempted the problem before?
    bool has_attempted(const std::string& problem_id) const;
    // has the user solved the problem before?
    bool has_solved(const std::string& problem_id) const;
    // what the proficiency of the user for the given topic?
    double get_topic_proficiency(const std::string& topic) const;

    std::vector<AttemptRecord> history;
};

// load user profile
UserProfile load_user_profile(
    const std::string& profile_path,
    const std::string& topic_path,
    const std::string& history_path
);

// save user profile
void save_user_profile(
    const std::string& profile_path,
    const std::string& topic_path,
    const std::string& history_path,
    const UserProfile& profile
);

}  // namespace scheduler
