#include "problem.h"
#include "user.h"

#include <filesystem>
#include <iostream>
#include <vector>

int main() {
    const std::vector<scheduler::Problem> problems = {
        {"P001", 1000, 20, {"implementation", "math"}},
        {"P002", 1100, 25, {"graphs", "bfs"}},
        {"P003", 1200, 30, {"greedy"}},
        {"P004", 1300, 35, {"dp", "knapsack"}},
        {"P005", 1400, 40, {"binary_search", "prefix_sum"}},
        {"P006", 1500, 45, {"trees", "dfs"}}
    };

    scheduler::UserProfile profile;
    profile.username = "username-example";
    profile.skill_level = 1200;
    profile.topic_proficiency = {
        {"implementation", 0.80},
        {"math", 0.65},
        {"graphs", 0.35},
        {"bfs", 0.40},
        {"greedy", 0.55},
        {"dp", 0.20},
        {"knapsack", 0.15},
        {"binary_search", 0.70},
        {"prefix_sum", 0.75},
        {"trees", 0.45},
        {"dfs", 0.50}
    };
    profile.history = {
        {"P001", true, 18, 0},
        {"P003", false, 28, 3},
        {"P005", false, 50, 4}
    };

    scheduler::save_problems_to_csv("data/problems.csv", problems);

    const std::filesystem::path user_dir = std::filesystem::path("data/users") / profile.username;
    scheduler::save_user_profile(
        (user_dir / "profile.csv").string(),
        (user_dir / "topic_proficiency.csv").string(),
        (user_dir / "user_history.csv").string(),
        profile
    );

    std::cout << "Wrote a deterministic test dataset to data/.\n";
    return 0;
}
