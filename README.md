# Algorithmic Study Session Scheduler

This project implements an algorithmic scheduling engine designed for competitive programming platforms (such as Codeforces and LeetCode) to automatically generate optimal, personalized study sessions based on a user's available time, skill level, and past performance.

## Problem Statement & Context

Competitive programming platforms offer thousands of problems but lack personalized guidance. Users often waste limited study time on problems that are too easy, too difficult, or unrelated to their current weaknesses. This project solves this by selecting a subset of problems that maximizes the educational benefit within a given time constraint.

This is modeled as a variant of the **0-1 Knapsack Problem**, where:
- **Items:** Coding problems (each with a difficulty rating, estimated solving time, and topic tags).
- **Weight/Cost:** Estimated solving time.
- **Value/Benefit:** A dynamically computed score based on user proficiency and problem traits.
- **Capacity:** The user's time budget for the study session.

### The Benefit Function
The educational benefit of a problem for a specific user is calculated using four factors:
1. **Topic Weakness:** Prioritizes topics where the user has lower proficiency.
2. **Difficulty Match:** Peaks when the problem difficulty aligns with the user's skill level (using an exponential decay formula for mismatch).
3. **Recency Penalty:** Penalizes problems attempted recently to avoid repetitive solving.
4. **Unsolved Bonus:** Heavily rewards retrying problems the user previously attempted but failed.

## Algorithmic Approaches

The project implements and compares two algorithmic strategies to solve this optimization problem:

### 1. Greedy Approach (Fractional Knapsack Heuristic)
- **Design:** Sorts problems by their benefit-to-time ratio in descending order, breaking ties by preferring shorter problems. It greedily adds problems to the session until the time budget is exhausted.
- **Complexity:** Time: $\mathcal{O}(n \log n)$ (dominated by sorting), Space: $\mathcal{O}(n)$.
- **Trade-offs:** Extremely fast and memory-efficient. While it does not theoretically guarantee a globally optimal selection (as it can leave unused time gaps), it serves as a highly effective and fast approximation.

### 2. Dynamic Programming (0-1 Knapsack)
- **Design:** Scales floating-point benefit values to integers and populates a 0-1 Knapsack DP table of size $(n+1) \times (T+1)$, where $T$ is the session time budget in minutes. The optimal schedule is recovered by backtracking.
- **Complexity:** Time: $\mathcal{O}(n \cdot T)$, Space: $\mathcal{O}(n \cdot T)$.
- **Trade-offs:** Guarantees a globally optimal subset selection (up to the scaling precision). However, it is slower than the greedy approach and its memory footprint grows linearly with the time budget.

## Situational Evaluation & Performance

Both algorithms present distinct engineering trade-offs depending on the deployment environment:
- **Interactive/Real-time Environments:** The **Greedy** approach is ideal for client-facing apps or real-time regeneration, providing instant responses even for large problem sets. It empirically achieves total benefit values within a few percent (mostly 97%-100%) of the optimal DP solution.
- **Batch Processing:** The **Dynamic Programming** approach is superior for scheduled background jobs (e.g., generating weekly study plans overnight), where runtime is less critical, and guaranteed optimality is required to perfectly pack the time budget.

### Benchmarks
Performance testing with varying problem sizes ($n$) and a fixed time budget ($T = 124$ min) demonstrated:
- The DP approach scales linearly with $n$, running in roughly $\sim 200\mu s$ for $n=50$ and $\sim 700\mu s$ for $n=250$.
- The Greedy approach is faster and scales logarithmically.
- In nearly all tested scenarios, the Greedy algorithm matched the exact optimal DP selection benefit (99\%+ accuracy), proving it to be a highly reliable heuristic for this specific domain where problem times are small compared to the overall budget.

## Challenges & Technical Highlights
- **Handling Floating-Point Values in DP:** Implemented a robust scaling technique (multiplying by a large constant $10^6$ and truncating) to allow the inherently integer-based Knapsack DP to process floating-point benefit scores without losing the relative ordering of problems.
- **System Design:** Includes modules for CSV parsing, data persistence, and distinct data structures mapping users, problems, and attempt histories.
