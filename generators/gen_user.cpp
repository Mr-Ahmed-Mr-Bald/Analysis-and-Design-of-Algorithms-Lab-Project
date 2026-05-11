#include "problem.h"
#include "user.h"

#include <algorithm>
#include <filesystem>
#include <iostream>
#include <random>
#include <string>
#include <unordered_set>
#include <vector>

namespace {

std::unordered_set<std::string> collect_topics(const std::vector<scheduler::Problem>& problems) {
    std::unordered_set<std::string> topics;
    for (const scheduler::Problem& problem : problems) {
        for (const std::string& topic : problem.topics) {
            topics.insert(topic);
        }
    }
    return topics;
}

}  // namespace

int main(int argc, char* argv[]) {
    const std::string username = argc > 1 ? argv[1] : "username-1";
    const std::string problem_path = argc > 2 ? argv[2] : "data/problems.csv";
    const std::string user_root = argc > 3 ? argv[3] : "data/users";
    const unsigned int seed = argc > 4 ? static_cast<unsigned int>(std::stoul(argv[4])) : 123U;

    const auto problems = scheduler::load_problems_from_csv(problem_path);
    if (problems.empty()) {
        std::cerr << "Cannot generate a user without problems.\n";
        return 1;
    }

    std::mt19937 engine(seed);
    std::uniform_int_distribution<int> skill_dist(900, 1800);
    std::uniform_real_distribution<double> proficiency_dist(0.15, 0.95);
    const int max_history_count = static_cast<int>(problems.size());
    const int min_history_count = std::min(5, max_history_count);
    std::uniform_int_distribution<int> history_count_dist(min_history_count, max_history_count);
    std::uniform_int_distribution<int> solved_dist(0, 1);
    std::uniform_int_distribution<int> wrong_dist(0, 6);
    std::uniform_int_distribution<int> time_delta_dist(-10, 25);

    scheduler::UserProfile profile;
    profile.username = username;
    profile.skill_level = (skill_dist(engine) / 100) * 100;

    for (const std::string& topic : collect_topics(problems)) {
        profile.topic_proficiency[topic] = proficiency_dist(engine);
    }

    std::vector<scheduler::Problem> shuffled = problems;
    std::shuffle(shuffled.begin(), shuffled.end(), engine);
    shuffled.resize(static_cast<std::size_t>(history_count_dist(engine)));

    for (const scheduler::Problem& problem : shuffled) {
        scheduler::AttemptRecord attempt;
        attempt.problem_id = problem.id;
        attempt.solved = solved_dist(engine) == 1;
        attempt.time_taken = std::max(5, problem.estimated_time + time_delta_dist(engine));
        attempt.wrong_submissions = attempt.solved ? wrong_dist(engine) / 2 : wrong_dist(engine);
        profile.history.push_back(std::move(attempt));
    }

    const std::filesystem::path user_dir = std::filesystem::path(user_root) / username;
    scheduler::save_user_profile(
        (user_dir / "profile.csv").string(),
        (user_dir / "topic_proficiency.csv").string(),
        (user_dir / "user_history.csv").string(),
        profile
    );

    std::cout << "Generated user data for " << username << " in " << user_dir.string() << '\n';
    return 0;
}
