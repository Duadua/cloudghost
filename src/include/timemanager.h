#pragma once

#include "cmath.h"
#include <chrono>					// c++11 time 库

using hour_type = std::chrono::duration<ll, std::ratio<60 * 60, 1>>;
using minute_type = std::chrono::duration<ll, std::ratio<60, 1>>;
using second_type = std::chrono::duration<ll, std::ratio<1, 1>>;
using msecnd_type = std::chrono::duration<ll, std::ratio<1, 1000>>;

using system_time_point = std::chrono::system_clock::time_point;
using highrs_time_point = std::chrono::high_resolution_clock::time_point;

struct DateTime {
	int year;
	int month;
	int day;
	int hour;
	int minute;
	int seconds;
	DateTime() {}
	DateTime(int y, int m, int d, int h, int mi, int s) 
		: year(y), month(m), day(d), hour(h), minute(mi), seconds(s) {}

	friend std::ostream& operator << (std::ostream& out, const DateTime& b);
	std::string to_string() const;

};

class TimeManager {
public:
	~TimeManager() {}

	static TimeManager* get_instance();

	DateTime cur_time_data();				// 当前系统时间  -- 输出用
	ll cur_time_msconds();					// 当前系统时间  -- 毫秒
	ll cur_runtime_msconds();				// 当前程序运行时间 -- 毫秒

	void update_tick();
	ll get_delta_tick_msconds() { return delta_tick_msconds; }

private:
	TimeManager() : delta_tick_msconds(0) {}

	static TimeManager* instance;

	static system_time_point time_start_s;	// 程序开始时的系统时间
	static highrs_time_point time_start_h;	// 程序开始时的boot 启动时间 -- 防止运算过程中修改系统时间
	static ll				 time_start_m;	// 程序开始时的系统时间秒数

	ll pre_tick_msconds;					// 上一帧运行的时间
	ll delta_tick_msconds;					// 每两帧之间的时间间隔

};

TimeManager& time_manager();