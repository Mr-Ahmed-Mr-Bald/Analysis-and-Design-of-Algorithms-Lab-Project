#pragma once

#include "problem.h"
#include "user.h"

#include <vector>

namespace scheduler {

// Selects problems greedily by descending benefit-to-time ratio
// until the session time budget is exhausted.
std::vector<Problem> greedy_schedule(
    const std::vector<Problem>& problems,
    const UserProfile& user_profile,
    int time_budget
);

}  // namespace scheduler