Study Session Scheduler for Competitive Programming

Problem Definition

Modern competitive programming platforms provide vast problem sets, but they lack personalized guidance on what to solve next. Users often waste time selecting problems that are either too easy, too difficult, or not aligned with their weaknesses.



This project proposes an algorithmic feature that automatically generates an optimal study session for a user based on their available time, skill level, and past performance.



Input

Problem Set

A collection of problems where each problem is defined by:

Unique ID

Difficulty level

Estimated solving time

One or more topic tags (e.g., DP, graphs, greedy)

User Profile

Current skill level

Topic-wise proficiency/weakness

User History

Previously attempted problems

Outcome (solved/unsolved)

Time taken

Number of wrong submissions

Session Constraints

Total available time for the study sessio

Output

A subset of problems selected from the problem set such that:

The total estimated solving time does not exceed the available session time

The selected problems maximize a defined learning benefit function

Algorithmic Aspect

The project will explore and compare at least two approaches:



A greedy approach based on benefit-to-time ratio

A dynamic programming approach (knapsack-style optimization)

