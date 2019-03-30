#include "timemanager.h"
#include <sstream>

std::ostream& operator << (std::ostream& out, const DateTime& b) {
	out << "["
		<< b.year << "." 
		<< (b.month < 10 ? "0":"") << b.month << "." 
		<< (b.day < 10 ? "0":"") << b.day << "]["
		<< (b.hour < 10 ? "0":"") << b.hour << ":" 
		<< (b.minute < 10 ? "0":"") << b.minute << ":" 
		<< (b.seconds < 10 ? "0":"") << b.seconds << "]";
	return out;
}
std::string DateTime::to_string() const {
	std::ostringstream oss;
	oss << (*this);
	return oss.str();
}
std::string DateTime::get_date() const {
	std::ostringstream oss;
	oss << "["
		<< year << "."
		<< (month < 10 ? "0" : "") << month << "."
		<< (day < 10 ? "0" : "") << day << "]";
	return oss.str();
}
std::string DateTime::get_time() const {
	std::ostringstream oss;
	oss << "["
		<< (hour < 10 ? "0" : "") << hour << ":"
		<< (minute < 10 ? "0" : "") << minute << ":"
		<< (seconds < 10 ? "0" : "") << seconds << "]";
	return oss.str();
}

// ================================================================================

void TimeManager::_init() {
	time_start_s = std::chrono::time_point_cast<msecnd_type>(std::chrono::system_clock::now());
	time_start_h = std::chrono::time_point_cast<msecnd_type>(std::chrono::high_resolution_clock::now());
	time_start_m = (std::chrono::time_point_cast<msecnd_type>(std::chrono::system_clock::now())).time_since_epoch().count();
	
	pre_tick_msconds = 0;
	delta_tick_msconds = 0;

}

DateTime TimeManager::cur_time_data() {
	auto c_h = std::chrono::time_point_cast<msecnd_type>(std::chrono::high_resolution_clock::now());
	auto c_t = std::chrono::time_point_cast<msecnd_type>(time_start_s + (c_h - time_start_h));
	auto tt = std::chrono::system_clock::to_time_t(c_t);
	struct tm t_tm;
	localtime_s(&t_tm, &tt);
	return DateTime(t_tm.tm_year + 1900, t_tm.tm_mon + 1, t_tm.tm_mday,
		t_tm.tm_hour, t_tm.tm_min, t_tm.tm_sec);
}
ll TimeManager::cur_time_msconds() {
	auto c_h = std::chrono::time_point_cast<msecnd_type>(std::chrono::high_resolution_clock::now());
	auto c_t = std::chrono::time_point_cast<msecnd_type>(time_start_s + (c_h - time_start_h));
	return c_t.time_since_epoch().count();
}

ll TimeManager::cur_runtime_msconds() { return cur_time_msconds() - time_start_m; }

float TimeManager::cur_runtime_seconds() {
	return 0.001f * cur_runtime_msconds();
}

void TimeManager::update_tick() {
	if (pre_tick_msconds == 0) { pre_tick_msconds = cur_runtime_msconds(); }
	delta_tick_msconds = cur_runtime_msconds() - pre_tick_msconds;
	pre_tick_msconds = cur_runtime_msconds();
}

