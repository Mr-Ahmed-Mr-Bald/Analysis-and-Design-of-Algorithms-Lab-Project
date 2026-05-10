#include "greedy.h"
#include "benefit.h"

#include <algorithm>
#include <vector>

namespace scheduler {

std::vector<Problem> greedy_schedule(
    const std::vector<Problem>& problems,
    const UserProfile& user_profile,
    int time_budget
) {
    // Build (ratio, index) pairs.
    struct RatedProblem {
        double ratio = 0.0;
        std::size_t index = 0;
    };

    std::vector<RatedProblem> rated;
    rated.reserve(problems.size());

    for (std::size_t i = 0; i < problems.size(); ++i) {
        const Problem& problem = problems[i];
        const double benefit = calculate_benefit(problem, user_profile);
        rated.push_back({benefit / problem.estimated_time, i});
    }

    // Sort by ratio descending; break ties by lower estimated_time first.
    std::sort(rated.begin(), rated.end(),
        [&problems](const RatedProblem& a, const RatedProblem& b) {
            if (a.ratio != b.ratio) {
                return a.ratio > b.ratio;
            }
            return problems[a.index].estimated_time < problems[b.index].estimated_time;
        }
    );

    std::vector<Problem> selected;
    int remaining = time_budget;

    for (const RatedProblem& rated_problem : rated) {
        const Problem& problem = problems[rated_problem.index];
        if (problem.estimated_time <= remaining) {
            selected.push_back(problem);
            remaining -= problem.estimated_time;
        }
    }

    return selected;
}

}  // namespace scheduler