#include <algorithm>
#include <cstring>
#include <dirent.h>
#include <vector>
#include "DirectoryListing.hpp"

// Must function as operator<
static bool sort_dirent(struct dirent i, struct dirent j)
{
	if (i.d_type == DT_DIR)
	{
		if (j.d_type == DT_DIR)
			return std::strcmp(i.d_name, j.d_name) <= 0;
		else
			return true;
	}
	else if (j.d_type == DT_DIR)
		return false;
	else
		return std::strcmp(i.d_name, j.d_name) <= 0;
}

std::string createDirectoryListingBody(const std::string &dir_path, const std::string &dir_root)
{
	DIR *dir;
	struct dirent *ent;
	std::string html("<!DOCTYPE html>\r\n"
			"<html lang=\"en\">\r\n"
			"<head>\r\n"
			"</head>\r\n"
			"<body>\r\n"
			"<h1>Index of " + dir_path + " </h1>\r\n"
			"<hr>\r\n");

	std::vector<struct dirent> entries;

	dir = opendir(dir_root.c_str());
	if (!dir)
		return "";
	while ((ent = readdir (dir)) != NULL)
	{
		entries.push_back(*ent);
	}
	closedir(dir);

	std::sort(entries.begin(), entries.end(), sort_dirent);

	for (std::vector<struct dirent>::iterator it = entries.begin(); it != entries.end(); it++)
	{
		std::string name(it->d_name);
		std::string line = "<a href=\"";
		line += dir_path;
		if (dir_path[dir_path.size() -1] != '/')
			line += '/';
		line += it->d_name;
		line += "\">";
		line += it->d_name;
		line += "</a></br>\r\n";
		html += line;
	}
	html += "</body>\r\n";

	return html;
}
//*/
