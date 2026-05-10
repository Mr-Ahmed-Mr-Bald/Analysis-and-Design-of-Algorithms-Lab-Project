#include "display.h"

#include <algorithm>
#include <iomanip>
#include <map>

namespace scheduler {

namespace {

constexpr int COLUMN_WIDTH = 22;
constexpr int ID_WIDTH = 8;
constexpr int DIFF_WIDTH = 12;
constexpr int TIME_WIDTH = 14;

void print_separator(std::ostream& out, int width = 60) {
    out << std::string(width, '-') << '\n';
}

void print_problem_row(std::ostream& out, const Problem& problem) {
    out << "  "
        << std::left  << std::setw(ID_WIDTH)   << problem.id
        << std::right << std::setw(DIFF_WIDTH)  << problem.difficulty
        << std::right << std::setw(TIME_WIDTH)  << problem.estimated_time
        << "  ";

    for (int i = 0; i < problem.topics.size(); ++i) {
        if (i > 0) out << ", ";
        out << problem.topics[i];
    }
    out << '\n';
}

}  // namespace

void print_user_summary(std::ostream& out, const UserProfile& user_profile) {
    print_separator(out);
    out << "User        : " << user_profile.username << '\n';
    out << "Skill level : " << user_profile.skill_level << '\n';
    out << "History     : " << user_profile.history.size() << " attempt(s)\n";

    if (!user_profile.topic_proficiency.empty()) {
        out << "Topic proficiency:\n";

        // Sort alphabetically for consistent output.
        std::map<std::string, double> sorted(
            user_profile.topic_proficiency.begin(),
            user_profile.topic_proficiency.end()
        );
        for (const auto& [topic, proficiency] : sorted) {
            out << "  " << std::left << std::setw(COLUMN_WIDTH) << topic
                << std::fixed << std::setprecision(2) << proficiency << '\n';
        }
    }
    print_separator(out);
}

void print_schedule(std::ostream& out, const ScheduleResult& result) {
    print_separator(out);
    out << "Algorithm : " << result.algorithm << '\n';
    out << "Problems  : " << result.selected.size() << '\n';
    out << "Total time: " << result.total_time << " min\n";
    out << "Total benefit: " << std::fixed << std::setprecision(4) << result.total_benefit << '\n';
    out << "Runtime   : " << result.runtime_us << " µs\n";

    if (!result.selected.empty()) {
        out << '\n';
        out << "  " << std::left  << std::setw(ID_WIDTH)   << "ID"
            << std::right << std::setw(DIFF_WIDTH)          << "Difficulty"
            << std::right << std::setw(TIME_WIDTH)          << "Est. time"
            << "  Topics\n";
        print_separator(out);

        for (const Problem& problem : result.selected) {
            print_problem_row(out, problem);
        }
    }
    print_separator(out);
}

void print_comparison(std::ostream& out, const std::vector<ScheduleResult>& results) {
    if (results.empty()) {
        return;
    }

    print_separator(out, 70);
    out << "COMPARISON SUMMARY\n";
    print_separator(out, 70);

    // Header
    out << std::left << std::setw(COLUMN_WIDTH) << "Metric";
    for (const ScheduleResult& result : results) {
        out << std::right << std::setw(COLUMN_WIDTH) << result.algorithm;
    }
    out << '\n';
    print_separator(out, 70);

    // Problems selected
    out << std::left << std::setw(COLUMN_WIDTH) << "Problems selected";
    for (const ScheduleResult& result : results) {
        out << std::right << std::setw(COLUMN_WIDTH) << result.selected.size();
    }
    out << '\n';

    // Total time
    out << std::left << std::setw(COLUMN_WIDTH) << "Total time (min)";
    for (const ScheduleResult& result : results) {
        out << std::right << std::setw(COLUMN_WIDTH) << result.total_time;
    }
    out << '\n';

    // Total benefit
    out << std::left << std::setw(COLUMN_WIDTH) << "Total benefit";
    for (const ScheduleResult& result : results) {
        out << std::right << std::setw(COLUMN_WIDTH)
            << std::fixed << std::setprecision(4) << result.total_benefit;
    }
    out << '\n';

    // Runtime
    out << std::left << std::setw(COLUMN_WIDTH) << "Runtime (µs)";
    for (const ScheduleResult& result : results) {
        out << std::right << std::setw(COLUMN_WIDTH) << result.runtime_us;
    }
    out << '\n';

    print_separator(out, 70);

    // Highlight winner
    if (results.size() >= 2) {
        const ScheduleResult* best = &results[0];
        for (const ScheduleResult& result : results) {
            if (result.total_benefit > best->total_benefit) {
                best = &result;
            }
        }
        out << "Higher benefit: " << best->algorithm << '\n';

        const ScheduleResult* fastest = &results[0];
        for (const ScheduleResult& result : results) {
            if (result.runtime_us < fastest->runtime_us) {
                fastest = &result;
            }
        }
        out << "Faster runtime: " << fastest->algorithm << '\n';
    }

    print_separator(out, 70);
}

}  // namespace scheduler