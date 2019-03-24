#include "cdebuger.h"
#include "filehelper.h"

#ifdef C_DEBUG_QT
#include "qtdebugwidget.h"
#endif // C_DEBUG_QT

void CDebuger::_init() {
	data = ""; ss.clear(); 
#ifdef C_DEBUG_QT
	c_debuger_qt = new QtDebugWidget();
	c_debuger_qt->show();
#endif // C_DEBUG_QT
}

void CDebuger::log(std::string str) {
#ifdef C_DEBUG
	data += "# =========================\n\t" + str + "\n";
	save("resources/logs/log.txt");
#endif // C_DEBUG

#ifdef C_DEBUG_QT
	if (c_debuger_qt != nullptr) { c_debuger_qt->append_text("# =========================\n\t" + str + "\n"); }
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
	if (c_debuger_qt != nullptr) { c_debuger_qt->append_text(t_str); }
#endif // C_DEBUG_QT

	return (*this);
}
bool CDebuger::save(const std::string& path) { return FileHelper_ins().save_txt(path, data); }



