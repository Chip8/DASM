#include <fstream>
#include <string>
#include <vector>
#include "types.hpp"
// These defines functions refer to the system-io such as disk,keyborad,etc.
/*Read programe file and store it into a char array in hex*/
std::string ReadFile(const char* FileName);
std::vector<std::string> PSplit(const std::string& Raw, int HexLength = 4);
bool CheckCorrect(std::vector<std::string> ASM);
std::string ASMGen(Instructions Ins, std::string param1 = "",
                   std::string param2 = "", std::string param3 = "");
void Dissassembler(std::vector<std::string> ASM,
	std::vector<std::string>& Output);