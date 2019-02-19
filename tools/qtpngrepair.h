#pragma once

// ÐÞ¸´ libpng µÄ png warning

#include <string>
#include <vector>

class QtPngRepair {	
public:
	static bool repair_png(std::string& path);
	static bool repair_one_png(std::string& path);

	static void get_all_files_from_dir(std::string& path, std::vector<std::string>& res);
	
	~QtPngRepair() {}
private:
	QtPngRepair() {}

};