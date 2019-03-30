#include "cdebuger.h"
#include "filehelper.h"
#include "timemanager.h"

#ifdef C_DEBUG_QT
#include "cdebugerwidget.h"
#endif // C_DEBUG_QT

void CDebuger::_init() {
#ifdef C_DEBUG_QT
	ui_c_debuger = new CDebugerWidget();
	ui_c_debuger->show();
#endif // C_DEBUG_QT

#ifdef C_DEBUG
	data = ""; ss.clear(); 
	(*this) << "[dua][c_debuger] start at " + TimeManager_ins().cur_time_data().get_date();
#endif // C_DEBUGme
}

void CDebuger::log(std::string str) {
#ifdef C_DEBUG
	data += "# =========================\n" + TimeManager_ins().cur_time_data().get_time() + str + "\n";
	save("resources/logs/log.txt");
#endif // C_DEBUG

#ifdef C_DEBUG_QT
	if (ui_c_debuger != nullptr) { ui_c_debuger->append_text("# =========================\n" + TimeManager_ins().cur_time_data().get_time() + str + "\n"); }
#endif // C_DEBUG_QT
}

CDebuger& CDebuger::operator << (const std::string& str) {
#ifdef C_DEBUG
	ss << "# =========================\n" + TimeManager_ins().cur_time_data().get_time() + str + "\n";
	data = ss.str();
	save("resources/logs/log.txt");
#endif // C_DEBUG

#ifdef C_DEBUG_QT
	std::string t_str = "# =========================\n" + TimeManager_ins().cur_time_data().get_time() + str + "\n";
	if (ui_c_debuger != nullptr) { ui_c_debuger->append_text(t_str); }
#endif // C_DEBUG_QT

	return (*this);
}
bool CDebuger::save(const std::string& path) { return FileHelper_ins().save_txt(path, data); }



