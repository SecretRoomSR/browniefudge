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
#include <vector>

std::string compile(std::vector<intermediate> code)
{
	bool has_input = false;
	bool has_dump = false;
	bool has_io = false;
	int tape_size = 30000; // standard size
	for (intermediate inst : code)
	{
		if (inst.inst == ',')
		{
			has_input = true;
			has_io = true;
			break;
		}
		if (inst.inst == '.')
		{
			has_io = true;
		}
	}
	for (intermediate inst : code)
	{
		if (inst.inst == '$')
		{
			has_dump = true;
			break;
		}
	}
	std::string headers = "#include<stdint.h>\n"
						  "#include<stdlib.h>\n";
	if (has_io || has_dump)
		headers += "#include<stdio.h>\n";
	if (has_input)
	{
		headers +=
			"#ifdef _WIN32\n"
			"#include <conio.h>\n"
			"#define g() _getch()\n"
			"#else\n"
			"#include <termios.h>\n"
			"#include <unistd.h>\n"
			"static inline char g(){char c;struct termios "
			"t,ot;tcgetattr(STDIN_FILENO,&ot);t=ot;t.c_lflag&=~(ICANON|ECHO);"
			"tcsetattr(STDIN_FILENO,TCSANOW,&t);read(STDIN_FILENO,&c,1);"
			"tcsetattr("
			"STDIN_FILENO,TCSANOW,&ot);return c;}\n"
			"#endif\n";
	}
	std::string result = "int main(){uint8_t*p=calloc(" +
						 std::to_string(tape_size) + ",1);int o=0;";
	if (has_dump)
		result += "FILE*f=fopen(\"dump.bin\",\"wb\");";

	for (intermediate inst : code)
	{
		switch (inst.inst)
		{
		case '+':
			if (inst.amount > 1)
			{
				result += "p[o]+=" + std::to_string(inst.amount) + ";";
				break;
			}
			result += "++p[o];";
			break;
		case '-':
			if (inst.amount > 1)
			{
				result += "p[o]-=" + std::to_string(inst.amount) + ";";
				break;
			}
			result += "--p[o];";
			break;
		case '<':
			if (inst.amount > 1)
			{
				result += "o-=" + std::to_string(inst.amount) + ";";
				break;
			}
			result += "--o;";
			break;
		case '>':
			if (inst.amount > 1)
			{
				result += "o+=" + std::to_string(inst.amount) + ";";
				break;
			}
			result += "++o;";
			break;
		case '[':
			for (int i = 0; i < inst.amount; ++i)
				result += "while(p[o]!=0){";
			break;
		case ']':
			result += std::string(inst.amount, '}');
			break;
		case '.':
			if (inst.amount > 1)
			{
				result += "for(int i=0;i<" + std::to_string(inst.amount) +
						  ";++i){printf(\"%c\",p[o]);}";
				break;
			}
			result += "printf(\"%c\",p[o]);";
			break;
		case ',':
			if (inst.amount > 1)
			{
				result += "for(int i=0;i<" + std::to_string(inst.amount) +
						  ";++i){p[o]=g();}";
				break;
			}
			result += "p[o]=g();";
			break;
		case '$':
			result += "fwrite(p,1," + std::to_string(tape_size) + ",f);";
			break;
		}
	}
	if (has_dump)
	{
		result += "fclose(f);";
	}
	result += "return 0;}";

	return headers + result;
}