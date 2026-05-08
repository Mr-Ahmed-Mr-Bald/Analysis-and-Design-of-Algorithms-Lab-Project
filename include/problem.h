#pragma once

#include <string>
#include <vector>

namespace scheduler {

struct Problem {
    std::string id;
    int difficulty = 0;
    int estimated_time = 0;
    std::vector<std::string> topics;
};

std::vector<std::string> split_topics(const std::string& raw_topics);
std::string join_topics(const std::vector<std::string>& topics);

std::vector<Problem> load_problems_from_csv(const std::string& path);
void save_problems_to_csv(const std::string& path, const std::vector<Problem>& problems);

}