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
    person_count;
    
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
  }

  // Get meeting duration
  int meeting_duration;
  std::cout << "Enter the duration of the meeting in minutes: ";
  std::cin >> meeting_duration;


  
//   // Optional: Print parsed schedules and working periods
//   for (int i = 0; i < schedules.size(); ++i) {
//     std::cout << "\nPerson " << i + 1 << "'s Schedule:\n";
//     for (const auto& time_range : schedules[i]) {
//       std::cout << "[" << time_range.first / 60 << ":" << (time_range.first % 60 < 10 ? "0" : "") << time_range.first % 60 
//                 << ", " << time_range.second / 60 << ":" << (time_range.second % 60 < 10 ? "0" : "") << time_range.second % 60 
//                 << "]\n";
//     }
//     std::cout << "Working Period: [" << working_periods[i].first / 60 << ":" << (working_periods[i].first % 60 < 10 ? "0" : "") 
//               << working_periods[i].first % 60 << ", " << working_periods[i].second / 60 << ":" 
//               << (working_periods[i].second % 60 < 10 ? "0" : "") << working_periods[i].second % 60 << "]\n";
//     }
  return 0;
}