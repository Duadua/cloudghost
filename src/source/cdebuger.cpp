#include "cdebuger.h"
#include "filehelper.h"

#ifdef C_DEBUG_QT
#include "cdebugerwidget.h"
#endif // C_DEBUG_QT

void CDebuger::_init() {
	data = ""; ss.clear(); 
#ifdef C_DEBUG_QT
	ui_c_debuger = new CDebugerWidget();
	ui_c_debuger->show();
#endif // C_DEBUG_QT

#ifdef C_DEBUG
	(*this) << "[c_debuger][dua][2019.03.16]";
#endif // C_DEBUG
}

void CDebuger::log(std::string str) {
#ifdef C_DEBUG
	data += "# =========================\n\t" + str + "\n";
	save("resources/logs/log.txt");
#endif // C_DEBUG

#ifdef C_DEBUG_QT
	if (ui_c_debuger != nullptr) { ui_c_debuger->append_text("# =========================\n\t" + str + "\n"); }
#endif // C_DEBUG_QT
}

CDebuger& CDebuger::operator << (const std::string& str) {
#ifdef C_DEBUG
	ss << "# =========================\n\t" + str + "\n";
	data = ss.str();
	save("resources/logs/log.txt");
#endif // C_DEBUG

#ifdef C_DEBUG_QT
	std::string t_str = "# =========================\n\t" + str + "\n";
	if (ui_c_debuger != nullptr) { ui_c_debuger->append_text(t_str); }
#endif // C_DEBUG_QT

	return (*this);
}
bool CDebuger::save(const std::string& path) { return FileHelper_ins().save_txt(path, data); }



