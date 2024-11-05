#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <regex>

std::pair<int, int> ParseTimeRange(const std::string& timeRange) {
  std::regex timeFormat(R"(\[(\d{2}):(\d{2}),\s*(\d{2}):(\d{2})\])");
  std::smatch matches;

  if (std::regex_match(timeRange, matches, timeFormat)) {
    int startHour = std::stoi(matches[1].str());
    int startMinute = std::stoi(matches[2].str());
    int endHour = std::stoi(matches[3].str());
    int endMinute = std::stoi(matches[4].str());

    int startTime = startHour * 60 + startMinute; // Convert to minutes
    int endTime = endHour * 60 + endMinute;

    return {startTime, endTime};
  }
}

std::vector<std::pair<int, int>> ParseSchedule(const std::string& scheduleInput) {
  std::vector<std::pair<int, int>> schedule;
  std::regex rangeFormat(R"(\[\d{2}:\d{2},\s*\d{2}:\d{2}\])");
  auto rangesBegin = std::sregex_iterator(scheduleInput.begin(), scheduleInput.end(), rangeFormat);
  auto rangesEnd = std::sregex_iterator();

  for (auto i = rangesBegin; i != rangesEnd; ++i) {
      schedule.push_back(ParseTimeRange(i->str()));
  }

  return schedule;
}

int main() {
  std::vector<std::vector<std::pair<int, int>>> schedules;
  std::vector<std::pair<int, int>> working_periods;
  
  std::cout << "\nEnter the schedules of the people you would like to compare in the format '[HH:MM, HH:MM], ...'"
            << "\nExample: [06:00, 08:30], [12:00, 14:00]"
            << "\nThen, enter the working period in the same format."
            << "\nPress enter after the last schedule and working period to finish.\n\n";
  
  int person_count = 0;
  while (true) {
    person_count++;
    
    std::string schedule_input;
    std::cout << "Enter Person " << person_count << "'s schedule: ";
    std::getline(std::cin, schedule_input);
    if (schedule_input.empty()) {break;}
    schedules.push_back(ParseSchedule(schedule_input));
    
    std::string working_period_input;
    std::cout << "Enter Person " << person_count << "'s working period: ";
    std::getline(std::cin, working_period_input);
    working_periods.push_back(ParseTimeRange(working_period_input));
  }
  
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