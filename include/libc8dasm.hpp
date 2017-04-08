#include <fstream>
#include <string>
#include <vector>
#include "./types.hpp"
/*Read programe file and store it into a char array in hex*/
std::string libc8ReadFile(const char* FileName);
std::vector<std::string> libc8SplitLine(const std::string& c8Rawstring,
                                        int HexLength = 4);
bool libc8CheckCorrect(std::vector<std::string> ASM);
std::string ibc8GenAsm(C8Ins Ins, std::string param1 = "",
                       std::string param2 = "", std::string param3 = "");
void libc8DASM(std::vector<std::string> ASM, std::vector<std::string>& Output);