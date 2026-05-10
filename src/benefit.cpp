#include "benefit.h"
#include <algorithm>
#include <cmath>

namespace scheduler {

    namespace {

        // How weak the user is across the problem's topics
        static double topic_weakness_weight(
            const Problem& problem,
            const UserProfile& user_profile
        ) {
            if (problem.topics.empty()) return 0.5; // neutral if no topics
    
            double total = 0.0;
            for (const std::string& topic : problem.topics) {
                double proficiency = user_profile.get_topic_proficiency(topic);
                total += (1.0 - proficiency);
            }
            return total / problem.topics.size();
        }
    
        // How well the problem difficulty matches the user's skill level
        static double difficulty_match_score(
            const Problem& problem,
            const UserProfile& user_profile
        ) {
            int gap = std::abs(problem.difficulty - user_profile.skill_level);
            return std::exp(-0.5 * gap);
        }
    
        // Penalizes problems the user has recently attempted to encourage variety
        static double recency_penalty(
            const Problem& problem,
            const UserProfile& user_profile
        ) {
            if (!user_profile.has_attempted(problem.id)) return 1.0;
    
            // count how many times the problem appears in history
            int attempts = 0;
            for (const AttemptRecord& record : user_profile.history) {
                if (record.problem_id == problem.id) attempts++;
            }
    
            return 1.0 / (1.0 + attempts);
        }
    
        // Boosts problems the user has tried but failed to solve
        static double unsolved_bonus(
            const Problem& problem,
            const UserProfile& user_profile
        ) {
            if (user_profile.has_attempted(problem.id)) {
                if (!user_profile.has_solved(problem.id)) {
                    return 1.5;
                } else {
                    return 0.5;
                }
            }
            return 1.0;
        }
    }

    double calculate_benefit(
        const Problem& problem,
        const UserProfile& user_profile
    ) {
        double weakness = topic_weakness_weight(problem, user_profile);
        double match = difficulty_match_score(problem, user_profile);
        double recency = recency_penalty(problem, user_profile);
        double unsolved = unsolved_bonus(problem, user_profile);
        return weakness * match * recency * unsolved;
    }

} // namespace scheduler