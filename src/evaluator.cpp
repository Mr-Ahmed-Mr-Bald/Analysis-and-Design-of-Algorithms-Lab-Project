#include "evaluator.h"
#include "benefit.h"
#include "csv_utils.h"
#include "greedy.h"
#include "knapsack.h"

#include <chrono>
#include <fstream>
#include <numeric>

namespace scheduler {

namespace {

// Compute the aggregate metrics for a list of selected problems.
ScheduleResult build_result(
    const std::string& algorithm,
    std::vector<Problem> selected,
    const UserProfile& user_profile,
    long long runtime_us
) {
    ScheduleResult result;
    result.algorithm = algorithm;
    result.selected = std::move(selected);
    result.runtime_us = runtime_us;

    for (const Problem& problem : result.selected) {
        result.total_benefit += calculate_benefit(problem, user_profile);
        result.total_time += problem.estimated_time;
    }

    return result;
}

// Write a CSV row per algorithm to the given path.
void write_csv(
    const std::string& path,
    const std::vector<ScheduleResult>& results,
    int time_budget
) {
    ensure_parent_directory(path);
    std::ofstream output(path);
    if (!output) {
        return; // evaluation output is still returned to caller
    }

    output << "algorithm,time_budget,problems_selected,total_time,total_benefit,runtime_us\n";
    for (const ScheduleResult& result : results) {
        output
            << escape_csv(result.algorithm) << ','
            << time_budget << ','
            << result.selected.size() << ','
            << result.total_time << ','
            << result.total_benefit << ','
            << result.runtime_us << '\n';
    }
}

}  // namespace

std::vector<ScheduleResult> evaluate(
    const std::vector<Problem>& problems,
    const UserProfile& user_profile,
    int time_budget,
    const std::string& csv_output_path
) {
    std::vector<ScheduleResult> results;

    //  Greedy 
    {
        const auto start = std::chrono::high_resolution_clock::now();
        std::vector<Problem> selected = greedy_schedule(problems, user_profile, time_budget);
        const auto end = std::chrono::high_resolution_clock::now();
        const long long us = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
        results.push_back(build_result("greedy", std::move(selected), user_profile, us));
    }

    //  Knapsack 
    {
        const auto start = std::chrono::high_resolution_clock::now();
        std::vector<Problem> selected = knapsack_schedule(problems, user_profile, time_budget);
        const auto end = std::chrono::high_resolution_clock::now();
        const long long us = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
        results.push_back(build_result("knapsack", std::move(selected), user_profile, us));
    }

    if (!csv_output_path.empty()) {
        write_csv(csv_output_path, results, time_budget);
    }

    return results;
}

}  // namespace scheduler