#include "problem.h"

#include <algorithm>
#include <iostream>
#include <random>
#include <string>
#include <vector>

namespace {

std::vector<std::string> pick_topics(
    std::mt19937& engine,
    const std::vector<std::string>& pool
) {
    std::uniform_int_distribution<int> topic_count_dist(1, 4);
    std::vector<std::string> shuffled = pool;
    std::shuffle(shuffled.begin(), shuffled.end(), engine);
    shuffled.resize(static_cast<std::size_t>(topic_count_dist(engine)));
    return shuffled;
}

}  // namespace

int main(int argc, char* argv[]) {
    const std::string output_path = argc > 1 ? argv[1] : "data/problems.csv";
    const int count = argc > 2 ? std::stoi(argv[2]) : 250;
    const unsigned int seed = argc > 3 ? static_cast<unsigned int>(std::stoul(argv[3])) : 42U;

    if (count <= 0) {
        std::cerr << "Problem count must be positive.\n";
        return 1;
    }

    const std::vector<std::string> topic_pool = {
        "graphs", "trees", "dp", "greedy", "math", "number_theory",
        "two_pointers", "binary_search", "strings", "implementation",
        "prefix_sum", "bitmasks", "dfs", "bfs", "shortest_paths",
        "segment_tree", "fenwick_tree", "union_find", "backtracking",
        "combinatorics", "geometry", "sorting", "brute_force", "hashing"
    };

    std::mt19937 engine(seed);
    std::uniform_int_distribution<int> difficulty_dist(800, 2200);
    std::uniform_int_distribution<int> time_dist(10, 150);

    std::vector<scheduler::Problem> problems;
    problems.reserve(static_cast<std::size_t>(count));

    for (int index = 1; index <= count; ++index) {
        scheduler::Problem problem;
        problem.id = "P" + std::to_string(index);
        problem.difficulty = (difficulty_dist(engine) / 100) * 100;
        problem.estimated_time = time_dist(engine);
        problem.topics = pick_topics(engine, topic_pool);
        problems.push_back(std::move(problem));
    }

    scheduler::save_problems_to_csv(output_path, problems);
    std::cout << "Generated " << count << " problems at " << output_path << '\n';
    return 0;
}
