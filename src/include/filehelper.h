#pragma once

#include "singleton.h"
#include <string>
#include <vector>

// files helper
SINGLETON_CLASS(FileHelper) {
	SINGLETON(FileHelper)

public:
	std::string get_suff_of_file(const std::string& path);
	std::string get_path_of_file(const std::string& path);
	std::string get_name_of_file(const std::string& path);
	void get_all_files_from_dir(const std::string& path, std::vector<std::string>& res);

	std::string load_txt(const std::string& path);
	bool		save_txt(const std::string& path, const std::string& source);

private:

};
SINGLETON_X(FileHelper)


