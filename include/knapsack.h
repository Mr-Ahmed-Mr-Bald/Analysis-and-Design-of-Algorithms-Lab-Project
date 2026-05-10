#pragma once

#include "problem.h"
#include "user.h"

#include <vector>

namespace scheduler {

// Selects problems using knapsack DP to maximise total benefit
// within the session time budget.
std::vector<Problem> knapsack_schedule(
    const std::vector<Problem>& problems,
    const UserProfile& user_profile,
    int time_budget,
    int benefit_scale = 10000
);

}  // namespace scheduler