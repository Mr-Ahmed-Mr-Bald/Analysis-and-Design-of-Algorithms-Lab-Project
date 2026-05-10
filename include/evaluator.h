#pragma once

#include "problem.h"
#include "user.h"

#include <string>
#include <vector>

namespace scheduler {

struct ScheduleResult {
    std::string algorithm; // "greedy" or "knapsack"
    std::vector<Problem> selected; // problems chosen for the session
    double total_benefit = 0.0; // sum of calculate_benefit() for each selected problem
    int total_time = 0; // sum of estimated_time for each selected problem
    long long runtime_us = 0; // wall-clock time the algorithm took (microseconds)
};

// Run both algorithms and return their results.
std::vector<ScheduleResult> evaluate(
    const std::vector<Problem>& problems,
    const UserProfile& user_profile,
    int time_budget,
    const std::string& csv_output_path = ""
);

}  // namespace scheduler