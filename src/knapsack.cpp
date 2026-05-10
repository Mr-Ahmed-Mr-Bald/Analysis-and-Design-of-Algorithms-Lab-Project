#include "knapsack.h"
#include "benefit.h"

#include <algorithm>
#include <vector>

namespace scheduler {

std::vector<Problem> knapsack_schedule(
    const std::vector<Problem>& problems,
    const UserProfile& user_profile,
    int time_budget,
    int benefit_scale
) {
    // Build item list. Skip problems that cannot fit.
    struct Item {
        int weight = 0; // estimated_time
        int value  = 0; // benefit * benefit_scale (integer)
        int index = 0; // index into `problems`
    };

    std::vector<Item> items;
    items.reserve(problems.size());

    for (int i = 0; i < problems.size(); ++i) {
        const Problem& problem = problems[i];
        if (problem.estimated_time > time_budget) {
            continue;
        }
        const double benefit = calculate_benefit(problem, user_profile);
        const int scaled_value = (benefit * benefit_scale);
        if (scaled_value <= 0) {
            continue;
        }
        items.push_back({problem.estimated_time, scaled_value, i});
    }

    const int n = items.size();
    if (n == 0) {
        return {};
    }

    const int cols = (time_budget + 1);
    const int rows = (n + 1);

    std::vector<int> table(rows * cols, 0);

    auto cell = [cols](int row, int col) -> int {
        return row * cols + col;
    };

    for (int i = 1; i <= n; ++i) {
        const Item& item = items[(i - 1)];
        for (int t = 0; t <= time_budget; ++t) {
            // Don't take item i
            table[cell(i, t)] = table[cell((i - 1), t)];
            // Take item i if it fits
            if (t >= item.weight) {
                const int prev = (t - item.weight);
                const int with_item = table[cell((i - 1), prev)] + item.value;
                if (with_item > table[cell(i, t)]) {
                    table[cell(i, t)] = with_item;
                }
            }
        }
    }

    // Backtrack through to recover the selected set.
    std::vector<bool> selected(n, false);
    int remaining = time_budget;

    for (int i = n; i >= 1 && remaining > 0; --i) {
        const Item& item = items[(i - 1)];
        // Item i was taken if including it improves over not taking it.
        if (remaining >= item.weight &&
            table[cell(i, remaining)] != table[cell(i - 1, remaining)])
        {
            selected[i - 1] = true;
            remaining -= item.weight;
        }
    }

    std::vector<Problem> result;
    for (int i = 0; i < n; ++i) {
        if (selected[i]) {
            result.push_back(problems[items[i].index]);
        }
    }

    return result;
}

}  // namespace scheduler