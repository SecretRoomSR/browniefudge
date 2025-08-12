/*
 * compiler.cpp - Compiles intermediate code into c code
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

#include "interpreter.hpp"
#include <string>
#include <string>
#include <vector>

std::string compile(std::vector<intermediate> code)
{
	int tape_size = 30000; // standard size
	std::string headers = "#include<stdio.h>\n#include<stdint.h>\n#include<stdlib.h>\n";
	std::string result = "int main(){uint8_t *p=calloc(" +
						 std::to_string(tape_size) +
						 ", sizeof(uint8_t));int o=0;";

	for (intermediate inst : code)
	{
		switch (inst.inst)
		{
		case '+':
			result += "p[o]+=" + std::to_string(inst.amount) + ";";
			break;
		case '-':
			result += "p[o]-=" + std::to_string(inst.amount) + ";";
			break;
		case '<':
			result += "o-=" + std::to_string(inst.amount) + ";";
			break;
		case '>':
			result += "o+=" + std::to_string(inst.amount) + ";";
			break;
		case '[':
			result += "while(p[o]!=0){";
			break;
		case ']':
			result += "}";
			break;
		case '$':
			result += "FILE*f=fopen(\"dump.bin\",\"wb\");fwrite(p,sizeof(uint8_t)," + std::to_string(tape_size) + ",f);fclose(f);";
			break;
		}
	}
	result += "return 0;}";

	return headers + result;
}