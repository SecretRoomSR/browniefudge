#include <cstdio>
#include <iterator>
#include <string>
#include <algorithm>
#include <vector>
#include <stdio.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <filesystem>
#include <fstream>
#endif

class Preference
{
  public:
	Preference(const std::string &appName) : appName(appName) {}

	// Return true on success, false on failure.
	bool set(const std::string &value)
	{
#ifdef _WIN32
		HKEY hKey;
		std::string regPath = "Software\\" + appName;
		if (RegCreateKeyExA(HKEY_CURRENT_USER, regPath.c_str(), 0, NULL,
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
			if (!ofs.is_open()) return false;
			ofs << value;
			if (!ofs) return false;
			ofs.flush();
			if (!ofs) return false;
		}

		// Move into place (rename is typically atomic on same filesystem)
		std::filesystem::rename(tmp, path, ec);
		if (ec) return false;

		// Ensure file permissions allow only owner read/write (adds those perms)
		// Note: this is best-effort; semantics vary by platform.
		std::filesystem::permissions(
			path,
			std::filesystem::perms::owner_read | std::filesystem::perms::owner_write,
			std::filesystem::perm_options::add,
			ec);
		// ignore ec for permissions; still consider write successful
		return true;
#endif
	}

	// Get stored value or defaultValue if not set or on error
	std::string get(const std::string &defaultValue = "") const
	{
#ifdef _WIN32
		HKEY hKey;
		char buffer[4096];
		DWORD bufferSize = sizeof(buffer);
		std::string regPath = "Software\\" + appName;
		if (RegOpenKeyExA(HKEY_CURRENT_USER, regPath.c_str(), 0, KEY_READ,
						  &hKey) == ERROR_SUCCESS)
		{
			if (RegGetValueA(hKey, NULL, "Value", RRF_RT_REG_SZ, NULL, buffer,
							 &bufferSize) == ERROR_SUCCESS)
			{
				RegCloseKey(hKey);
				return std::string(buffer);
			}
			RegCloseKey(hKey);
		}
		return defaultValue;
#else
		std::ifstream ifs(getConfigPath(), std::ios::binary);
		if (!ifs.is_open()) return defaultValue;

		// Read entire file into string
		std::string contents;
		ifs.seekg(0, std::ios::end);
		auto size = ifs.tellg();
		if (size > 0)
		{
			contents.resize(static_cast<size_t>(size));
			ifs.seekg(0);
			ifs.read(&contents[0], size);
			if (!ifs) return defaultValue;
		}
		else
		{
			// Empty file or size==0: return empty string (or default?)
			// We'll return empty string to match prior behavior of getline returning "".
		}
		return contents;
#endif
	}

  private:
#ifndef _WIN32
	std::filesystem::path getConfigPath() const
	{
		const char *xdg = std::getenv("XDG_CONFIG_HOME");
		const char *home = std::getenv("HOME");
		if (!home && !xdg)
		{
			// No HOME nor XDG_CONFIG_HOME: fallback to current directory's .config (best-effort)
			return std::filesystem::path(".") / ".config" / appName / "compiler";
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
		return base / appName / "compiler";
	}
#endif

	std::string appName;
};

void logerror(const std::string &message, char *argv[])
{
	std::fprintf(stderr, "%s: %s", argv[0], message.c_str());
}

int main(int argc, char *argv[])
{
	Preference pref("browniefudge");
	std::vector<std::string> args;
	args.reserve(argc);
	for (int i = 0; i < argc; ++i) args.emplace_back(argv[i] ? argv[i] : "");

	if (argc > 1)
	{
		auto pos = std::find(args.begin(), args.end(), "--set-compiler");
		if (pos != args.end())
		{
			auto next = std::next(pos);
			if (next != args.end())
			{
				std::string compiler = *next;
				if (!pref.set(compiler))
				{
					logerror("Failed to save preference\n", argv);
					return 2;
				}
			}
			else
			{
				logerror("--set-compiler given but no value provided\n", argv);
				return 3;
			}
		}
	}

	std::string current = pref.get("clang++");

	return 0;
}
