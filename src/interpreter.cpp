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
#include <string>
#include <vector>

#include "interpreter.hpp"
#include "logging.hpp"

namespace fs = std::filesystem;

std::string interpret(fs::path path)
{
	char buffer = 0;
	int bufferLength;

	std::ifstream stream(path);
	if (!stream.is_open())
	{
		logerror(1, "Unable to read file " + path.string());
	}
	loginfo("Loaded file " + path.string());

	std::string intermediate;
	char ch;
	std::vector<char> knownInstructions = {'+', '-', '<', '>',
										   '[', ']', ',', '.'};

	while (stream.get(ch))
	{
		if (std::find(knownInstructions.begin(), knownInstructions.end(), ch) ==
			knownInstructions.end())
			continue;
		if (buffer == ch)
		{
			bufferLength++;
			continue;
		}
		if (buffer != 0)
		{
			intermediate += buffer + std::to_string(bufferLength) + ";";
		}
		buffer = ch;
		bufferLength = 1;
	}

	stream.close();

	return intermediate;
}