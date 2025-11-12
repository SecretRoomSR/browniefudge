/*
 * interpreter.cpp - Interpretation to optimize the source code
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

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <vector>

#include "interpreter.hpp"
#include "logging.hpp"

namespace fs = std::filesystem;

std::vector<intermediate> interpret(fs::path path) {
	char buffer = 0;
	int buffer_length;

	std::ifstream stream(path);
	if (!stream.is_open()) {
		logerror(1, "Unable to read file " + path.string());
	}
	loginfo("Loaded file " + path.string());

	std::vector<intermediate> inter;
	char ch;
	std::vector<char> known_instructions = {'+', '-', '<', '>', '$',
											'[', ']', ',', '.', ','};

	while (stream.get(ch)) {
		if (std::find(known_instructions.begin(), known_instructions.end(), ch) ==
			known_instructions.end())
			continue;
		if (buffer == ch) {
			buffer_length++;
			continue;
		}
		if (buffer != 0) {
			inter.push_back(intermediate(buffer_length, buffer));
		}
		buffer = ch;
		buffer_length = 1;
	}
	if (buffer != 0) {
		inter.push_back(intermediate(buffer_length, buffer));
	}

	stream.close();

	return inter;
}