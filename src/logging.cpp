/*
 * logging.cpp - For pretty debugging
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

#include "logging.hpp"

void loginfo(const std::string &message) {
	std::printf("[browniefudge] <INFO> %s\n", message.c_str());
}

void logwarning(const std::string &message) {
	std::printf("[browniefudge] <WARN> %s\n", message.c_str());
}

void logerror(int code, const std::string &message) {
	std::fprintf(stderr, "[browniefudge] <ERR> %s\n", message.c_str());
	exit(code);
}