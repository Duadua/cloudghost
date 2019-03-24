#include "debuger.h"
#include "filehelper.h"

#ifdef C_DEBUG_QT
#include "qtdebugwidget.h"
#endif // C_DEBUG_QT

CDebug* CDebug::instance = nullptr;
CDebug* CDebug::get_instance() {
	if (instance == nullptr) { instance = new CDebug(); }
	return instance;
}
CDebug& c_debug() { return (*CDebug::get_instance()); }

CDebug::CDebug() { 
	data = ""; ss.clear(); 
#ifdef C_DEBUG_QT
	c_debug_qt = new QtDebugWidget();
	c_debug_qt->show();
#endif // C_DEBUG_QT
} 
CDebug::~CDebug() {
#ifdef C_DEBUG_QT
	delete c_debug_qt;
#endif // C_DEBUG_QT
}

void CDebug::log(std::string str) {
#ifdef C_DEBUG
	data += "# =========================\n\t" + str + "\n";
	save("resources/logs/log.txt");
#endif // C_DEBUG

#ifdef C_DEBUG_QT
	if (c_debug_qt != nullptr) { c_debug_qt->append_text("# =========================\n\t" + str + "\n"); }
#endif // C_DEBUG_QT
}

CDebug& CDebug::operator << (const std::string& str) {
#ifdef C_DEBUG
	ss << "# =========================\n\t" + str + "\n";
	data = ss.str();
	save("resources/logs/log.txt");
#endif // C_DEBUG

#ifdef C_DEBUG_QT
	std::string t_str = "# =========================\n\t" + str + "\n";
	if (c_debug_qt != nullptr) { c_debug_qt->append_text(t_str); }
#endif // C_DEBUG_QT

	return (*this);
}
bool CDebug::save(const std::string& path) { return save_txt(path, data); }



