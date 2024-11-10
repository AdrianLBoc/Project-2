#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <regex>

// Helper to parse time range in "HH:MM" format to minutes
std::pair<int, int> ParseTimeRange(const std::string& time_range) {
  std::regex time_format(R"(\[(\d{2}):(\d{2}),\s*(\d{2}):(\d{2})\])");
  std::smatch matches;

  if (std::regex_match(time_range, matches, time_format)) {
    int start_hour = std::stoi(matches[1].str());
    int start_minute = std::stoi(matches[2].str());
    int end_hour = std::stoi(matches[3].str());
    int end_minute = std::stoi(matches[4].str());

    int start_time = start_hour * 60 + start_minute; // Convert to minutes
    int end_time = end_hour * 60 + end_minute;

    return {start_time, end_time};
  }
  return {0, 0}; // Return 0 if the time range is invalid
}

// Helper to parse a person's schedule
std::vector<std::pair<int, int>> ParseSchedule(const std::string& schedule_input) {
  std::vector<std::pair<int, int>> schedule;
  std::regex range_format(R"(\[\d{2}:\d{2},\s*\d{2}:\d{2}\])");
  auto ranges_begin = std::sregex_iterator(schedule_input.begin(), schedule_input.end(), range_format);
  auto ranges_end = std::sregex_iterator();

  for (auto i = ranges_begin; i != ranges_end; ++i) {
      schedule.push_back(ParseTimeRange(i->str()));
  }

  return schedule;
}

// Helper to convert minutes to "HH:MM" format
std::string MinutesToTime(int minutes) {
    int hours = minutes / 60;
    minutes %= 60;
    return (hours < 10 ? "0" : "") + std::to_string(hours) + ":" + (minutes < 10 ? "0" : "") + std::to_string(minutes);
}

// Merges overlapping intervals in a schedule
std::vector<std::pair<int, int>> MergeIntervals(std::vector<std::pair<int, int>> intervals) {
    if (intervals.empty()) return {};

    std::sort(intervals.begin(), intervals.end());
    std::vector<std::pair<int, int>> merged;
    merged.push_back(intervals[0]);

    for (size_t i = 1; i < intervals.size(); ++i) {
        if (merged.back().second >= intervals[i].first) {
            merged.back().second = std::max(merged.back().second, intervals[i].second);
        } else {
            merged.push_back(intervals[i]);
        }
    }
    return merged;
}

// Finds available time slots for a meeting
std::vector<std::pair<int, int>> FindAvailableSlots(
    const std::vector<std::vector<std::pair<int, int>>>& schedules,
    const std::vector<std::pair<int, int>>& working_periods,
    int min_duration) {

    // Merge all schedules into one
    std::vector<std::pair<int, int>> all_intervals;
    for (const auto& schedule : schedules) {
        all_intervals.insert(all_intervals.end(), schedule.begin(), schedule.end());
    }
    all_intervals = MergeIntervals(all_intervals);

    std::vector<std::pair<int, int>> available_slots;
    int current_time = working_periods[0].first;  // Start from the earliest working period start

    for (const auto& interval : all_intervals) {
        if (current_time < interval.first) {
            int gap = interval.first - current_time;
            if (gap >= min_duration) {
                available_slots.push_back({current_time, interval.first});
            }
        }
        current_time = std::max(current_time, interval.second);
    }

    // Check if there is a final gap until the end of the last working period
    if (current_time < working_periods[0].second) {
        int gap = working_periods[0].second - current_time;
        if (gap >= min_duration) {
            available_slots.push_back({current_time, working_periods[0].second});
        }
    }

    return available_slots;
}

int main() {
  std::vector<std::vector<std::pair<int, int>>> schedules;
  std::vector<std::pair<int, int>> working_periods;
  
  // Prompt user for input
  std::cout << "\nEnter the schedules of the people you would like to compare in the format '[HH:MM, HH:MM], ...'"
            << "\nExample: [06:00, 08:30], [12:00, 14:00]"
            << "\nThen, enter the working period in the same format."
            << "\nPress enter after the last schedule and working period to finish.\n\n";
  
  int person_count = 0;
  while (true) {
    // Get schedule inputs
    std::string schedule_input;
    std::cout << "Enter Person " << person_count + 1 << "'s schedule: ";
    std::getline(std::cin, schedule_input);
    if (schedule_input.empty()) {break;}
    schedules.push_back(ParseSchedule(schedule_input));
    
    // Get working period inputs
    std::string working_period_input;
    std::cout << "Enter Person " << person_count + 1 << "'s working period: ";
    std::getline(std::cin, working_period_input);
    working_periods.push_back(ParseTimeRange(working_period_input));
    ++person_count;
  }

  // Get meeting duration
  int meeting_duration;
  std::cout << "Enter the duration of the meeting in minutes: ";
  std::cin >> meeting_duration;

  // Find available slots and print them
  std::vector<std::pair<int, int>> available_slots = FindAvailableSlots(schedules, working_periods, meeting_duration);
  std::cout << "\nAvailable time slots:\n";
  for (const auto& slot : available_slots) {
    std::cout << "[" << MinutesToTime(slot.first) << ", " << MinutesToTime(slot.second) << "]\n";
  }

  return 0;
}