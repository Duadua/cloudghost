#include "timemanager.h"
#include <sstream>

std::ostream& operator << (std::ostream& out, const DateTime& b) {
	out << "DataTime ( "
		<< b.year << "." << b.month << "." << b.day << " -- "
		<< b.hour << ":" << b.minute << ":" << b.seconds << " )";
	return out;
}
std::string DateTime::to_string() const {
	std::ostringstream oss;
	oss << (*this);
	return oss.str();
}

// ================================================================================

TimeManager* TimeManager::instance = nullptr;

system_time_point TimeManager::time_start_s = std::chrono::time_point_cast<msecnd_type>(std::chrono::system_clock::now());
highrs_time_point TimeManager::time_start_h = std::chrono::time_point_cast<msecnd_type>(std::chrono::high_resolution_clock::now());
ll				  TimeManager::time_start_m = (std::chrono::time_point_cast<msecnd_type>(std::chrono::system_clock::now())).time_since_epoch().count();
;	

TimeManager* TimeManager::get_instance() {
	if (instance == nullptr) { instance = new TimeManager(); }
	return instance;
}

DateTime TimeManager::cur_time_data() {
	auto c_h = std::chrono::time_point_cast<msecnd_type>(std::chrono::high_resolution_clock::now());
	auto c_t = std::chrono::time_point_cast<msecnd_type>(time_start_s + (c_h - time_start_h));
	auto tt = std::chrono::system_clock::to_time_t(c_t);
	struct tm* t_tm = localtime(&tt);
	return DateTime(t_tm->tm_year + 1900, t_tm->tm_mon + 1, t_tm->tm_mday,
		t_tm->tm_hour, t_tm->tm_min, t_tm->tm_sec);
}
ll TimeManager::cur_time_msconds() {
	auto c_h = std::chrono::time_point_cast<msecnd_type>(std::chrono::high_resolution_clock::now());
	auto c_t = std::chrono::time_point_cast<msecnd_type>(time_start_s + (c_h - time_start_h));
	return c_t.time_since_epoch().count();
}

ll TimeManager::cur_runtime_msconds() { return cur_time_msconds() - time_start_m; }

void TimeManager::update_tick() {
	if (pre_tick_msconds == 0) { pre_tick_msconds = cur_runtime_msconds(); }
	delta_tick_msconds = cur_runtime_msconds() - pre_tick_msconds;
	pre_tick_msconds = cur_runtime_msconds();
}

TimeManager& time_manager() { return (*TimeManager::get_instance()); }
