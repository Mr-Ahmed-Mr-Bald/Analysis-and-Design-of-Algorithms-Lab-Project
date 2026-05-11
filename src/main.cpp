#include "display.h"
#include "evaluator.h"
#include "problem.h"
#include "user.h"

#include <filesystem>
#include <iostream>
#include <stdexcept>
#include <string>

namespace {

// All paths are relative to the working directory.
const std::string PROBLEMS_PATH = "data/problems.csv";
const std::string USERS_ROOT = "data/users";
const std::string BENCHMARK_PATH = "data/results/benchmarks.csv";

void print_usage(const char* program_name) {
    std::cerr
        << "Usage: " << program_name
        << " <username>"
        << " <time_budget>\n"
        << '\n'
        << "  username     name of the user under data/users/\n"
        << "  time_budget  session duration in minutes (positive integer)\n"
        << '\n'
        << "Fixed paths:\n"
        << "  problems     " << PROBLEMS_PATH  << '\n'
        << "  users root   " << USERS_ROOT     << "/<username>/\n"
        << "  benchmark    " << BENCHMARK_PATH << '\n';
}

int parse_time_budget(const std::string& raw_value) {
    try {
        const int value = std::stoi(raw_value);
        if (value <= 0) {
            throw std::runtime_error("time_budget must be a positive integer");
        }
        return value;
    } catch (const std::invalid_argument&) {
        throw std::runtime_error("time_budget is not a valid integer: " + raw_value);
    }
}

}  // namespace

int main(int argc, char* argv[]) {

    if (argc < 3) {
        print_usage(argv[0]);
        return 1;
    }

    const std::string username   = argv[1];
    const std::string raw_budget = argv[2];

    // Derive all user-related paths from the username.
    const std::filesystem::path user_dir = std::filesystem::path(USERS_ROOT) / username;
    const std::string profile_path = (user_dir / "profile.csv").generic_string();
    const std::string topic_path = (user_dir / "topic_proficiency.csv").generic_string();
    const std::string history_path = (user_dir / "user_history.csv").generic_string();

    int time_budget = 0;

    try {
        time_budget = parse_time_budget(raw_budget);
    } catch (const std::exception& error) {
        std::cerr << "Error: " << error.what() << '\n';
        return 1;
    }

    scheduler::UserProfile user_profile;
    std::vector<scheduler::Problem> problems;

    try {
        problems = scheduler::load_problems_from_csv(PROBLEMS_PATH);
        user_profile = scheduler::load_user_profile(profile_path, topic_path, history_path);
    } catch (const std::exception& error) {
        std::cerr << "Error loading data: " << error.what() << '\n';
        return 1;
    }

    std::cout << "\n=== Study Session Scheduler ===\n\n";
    std::cout << "Problem set  : " << problems.size() << " problems  ("
              << PROBLEMS_PATH << ")\n";
    std::cout << "Time budget  : " << time_budget << " minutes\n";

    scheduler::print_user_summary(std::cout, user_profile);

    std::vector<scheduler::ScheduleResult> results;

    try {
        results = scheduler::evaluate(
            problems,
            user_profile,
            time_budget,
            BENCHMARK_PATH
        );
    } catch (const std::exception& error) {
        std::cerr << "Error during scheduling: " << error.what() << '\n';
        return 1;
    }

    for (const scheduler::ScheduleResult& result : results) {
        scheduler::print_schedule(std::cout, result);
    }

    scheduler::print_comparison(std::cout, results);

    std::cout << "Benchmark results written to: " << BENCHMARK_PATH << '\n';

    return 0;
}