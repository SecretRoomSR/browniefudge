/*
 * preference.cpp - Saves preferences set to the program
 * Copyright (C) 2025 Jason Christian
 *
 * This file is part of browniefudge.
 *
 * browniefudge is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * browniefudge is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 */

#include <string>

#include "preference.hpp"

#ifdef _WIN32
#include <windows.h>
#else
#include <filesystem>
#include <fstream>
#endif

Preference::Preference(const std::string &app_name) : app_name(app_name) {}

// Return true on success, false on failure.
bool Preference::set(const std::string &value)
{
#ifdef _WIN32
	HKEY hKey;
	std::string reg_path = "Software\\" + app_name;
	if (RegCreateKeyExA(HKEY_CURRENT_USER, reg_path.c_str(), 0, NULL,
						REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hKey,
						NULL) == ERROR_SUCCESS)
	{
		LONG rc = RegSetValueExA(hKey, "Value", 0, REG_SZ,
								 reinterpret_cast<const BYTE *>(value.c_str()),
								 static_cast<DWORD>(value.size() + 1));
		RegCloseKey(hKey);
		return rc == ERROR_SUCCESS;
	}
	return false;
#else
	std::filesystem::path path = getConfigPath();

	std::error_code ec;
	std::filesystem::create_directories(path.parent_path(), ec);
	if (ec)
	{
		// Could not create directories
		return false;
	}

	// Write atomically: write to temp file then rename
	std::filesystem::path tmp = path;
	tmp += ".tmp";

	{
		std::ofstream ofs(tmp, std::ios::binary | std::ios::trunc);
		if (!ofs.is_open())
			return false;
		ofs << value;
		if (!ofs)
			return false;
		ofs.flush();
		if (!ofs)
			return false;
	}

	// Move into place (rename is typically atomic on same filesystem)
	std::filesystem::rename(tmp, path, ec);
	if (ec)
		return false;

	// Ensure file permissions allow only owner read/write (adds those perms)
	// Note: this is best-effort; semantics vary by platform.
	std::filesystem::permissions(path,
								 std::filesystem::perms::owner_read |
									 std::filesystem::perms::owner_write,
								 std::filesystem::perm_options::add, ec);
	// ignore ec for permissions; still consider write successful
	return true;
#endif
}

// Get stored value or defaultValue if not set or on error
std::string Preference::get(const std::string &default_value) const
{
#ifdef _WIN32
	HKEY hKey;
	char buffer[4096];
	DWORD buffer_size = sizeof(buffer);
	std::string reg_path = "Software\\" + app_name;
	if (RegOpenKeyExA(HKEY_CURRENT_USER, reg_path.c_str(), 0, KEY_READ, &hKey) ==
		ERROR_SUCCESS)
	{
		if (RegGetValueA(hKey, NULL, "Value", RRF_RT_REG_SZ, NULL, buffer,
						 &buffer_size) == ERROR_SUCCESS)
		{
			RegCloseKey(hKey);
			return std::string(buffer);
		}
		RegCloseKey(hKey);
	}
	return default_value;
#else
	std::ifstream ifs(getConfigPath(), std::ios::binary);
	if (!ifs.is_open())
		return default_value;

	// Read entire file into string
	std::string contents;
	ifs.seekg(0, std::ios::end);
	auto size = ifs.tellg();
	if (size > 0)
	{
		contents.resize(static_cast<size_t>(size));
		ifs.seekg(0);
		ifs.read(&contents[0], size);
		if (!ifs)
			return default_value;
	}
	else
	{
		// Empty file or size==0: return empty string (or default?)
		// We'll return empty string to match prior behavior of getline
		// returning "".
	}
	return contents;
#endif
}

#ifndef _WIN32
std::filesystem::path Preference::getConfigPath() const
{
	const char *xdg = std::getenv("XDG_CONFIG_HOME");
	const char *home = std::getenv("HOME");
	if (!home && !xdg)
	{
		// No HOME nor XDG_CONFIG_HOME: fallback to current directory's .config
		// (best-effort)
		return std::filesystem::path(".") / ".config" / app_name / "compiler";
	}

	std::filesystem::path base;
	if (xdg && *xdg)
	{
		base = xdg;
	}
	else
	{
		base = std::filesystem::path(home) / ".config";
	}
	return base / app_name / "compiler";
}
#endif