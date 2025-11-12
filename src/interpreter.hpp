/*
 * interpreter.hpp - Header file for interpreter.cpp
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

#pragma once
#include <filesystem>
#include <vector>

namespace fs = std::filesystem;

typedef struct intermediate {
public:
	int amount;
	char inst;
	intermediate(int amount, char inst) : amount(amount), inst(inst) {}
} intermediate;
std::vector<intermediate> interpret(fs::path path);