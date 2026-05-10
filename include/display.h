#pragma once

#include "evaluator.h"
#include "problem.h"
#include "user.h"

#include <ostream>
#include <vector>

namespace scheduler {

// Print a summary of a single scheduling result.
void print_schedule(
    std::ostream& out,
    const ScheduleResult& result
);

// Print a side-by-side comparison of two or more scheduling results.
void print_comparison(
    std::ostream& out,
    const std::vector<ScheduleResult>& results
);

// Print an overview of the user profile.
void print_user_summary(
    std::ostream& out,
    const UserProfile& user_profile
);

}  // namespace scheduler