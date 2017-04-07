#include "c8dasm.hpp"
#include <iomanip>
#include <iterator>
#include <sstream>

/*Read programe file and store it into a char array in hex*/
std::string ReadFile(const char* FileName) {
  // Since the Chip-8 program usually won't be greater than 1KB,so I choose
  // string for temperary storage just in mind.
  std::ifstream File(FileName, std::ios::binary);
  std::vector<BIT8> data((std::istreambuf_iterator<char>(File)),
                          std::istreambuf_iterator<char>());
  std::stringstream ss;
  for (auto i : data) {
    ss << std::setw(2) << std::setfill('0') << std::hex << std::uppercase
       << int(i);
  }
  return ss.str();
}

/* Split the raw program string into a 4-hex-num based vector*/
std::vector<std::string> PSplit(const std::string& Raw, int HexLength) {
  int NumPLine = Raw.length() / HexLength;  // The number of Program Line
  std::vector<std::string> ASM;  // Yes,it's for produce the asm source code.

  for (auto i = 0; i < NumPLine; i++) {
    ASM.push_back(Raw.substr(i * HexLength, HexLength));
  }

  // If there are leftover characters, create a shorter item at the end.
  if (Raw.length() % HexLength != 0) {
    ASM.push_back(Raw.substr(HexLength * NumPLine));
  }
  return ASM;
}

/*Check if the program is correct*/
bool CheckCorrect(std::vector<std::string> ASM) {
	if (ASM.empty()) return false;
  for (auto x : ASM) {
    std::stringstream ss;
    if (x.length() != 4) return false;  // Uncomplete program?
    ss << x;
    uint16_t temp;
    ss >> std::hex >> temp;
    if (temp < 0x0000 || temp > 0xFFFF) return false;  // out of range?
  }
  return true;
}
std::string ASMGen(Instructions Ins, std::string param1, std::string param2,
                   std::string param3) {
  std::stringstream ss;
  switch (Ins) {
    case CLS:
      ss << "CLS";
      break;
    case RET:
      ss << "RET";
      break;
    case SYS_A:
      ss << "SYS " << std::setfill('0') << std::setw(4) << param1;
      break;
    case JP_A:
      ss << "JP " << std::setfill('0') << std::setw(4) << param1;
      break;
    case CALL_A:
      ss << "CALL " << std::setfill('0') << std::setw(4) << param1;
      break;
    case SE_RK:
		ss << "SE V" << param1 << "," <<std::setfill('0')<<std::setw(4)<< param2;
		break;
    case SE_RR:
      ss << "SE V" << param1 << ", V" << param2;
      break;
    case SNE_RK:
		ss << "SNE V" << param1 << ", " << std::setfill('0') << std::setw(4) << param2;
		break;
    case SNE_RR:
      ss << "SNE V" << param1 << ", V" << param2;
	  break;
    case LD_RK:
		ss << "LD V" << param1 << ", " << std::setfill('0') << std::setw(2) << param2;
		break;
    case LD_RR:
      ss << "LD V" << param1 << ", V" << param2;
      break;
    case ADD_RK:
		ss << "ADD V" << param1 << ", " << param2;
		break;
    case ADD_RR:
      ss << "ADD V" << param1 << ", V" << param2;
      break;
    case OR_RR:
      ss << "OR V" << param1 << ", V" << param2;
      break;
    case AND_RR:
      ss << "AND V" << param1 << ", V" << param2;
      break;
    case XOR_RR:
      ss << "XOR V" << param1 << ", V" << param2;
      break;
    case SUB_RR:
      ss << "SUB V" << param1 << ", V" << param2;
      break;
    case SHR_RR:
      ss << "SHR V" << param1 << "{, V" << param2 << "}";
      break;
    case SUBN_RR:
      ss << "SUBN V" << param1 << ", V" << param2;
      break;
    case SHL_RR:
      ss << "SHL V" << param1 << "{, V" << param2 << "}";
      break;
    case LD_IA:
      ss << "LD I, " << std::setfill('0') << std::setw(4) << param1;
      break;
    case JP_V0A:
      ss << "JP V0, " <<std::setfill('0')<<std::setw(4) <<param1;
      break;
    case RND_RK:
      ss << "RND V" << param1 << ", " << param2;
      break;
    case DRW_RRN:
      ss << "DRW V" << param1 << ", V" << param2 << ", " << param3;
      break;
    case SKP_R:
      ss << "SKP V" << param1;
      break;
    case SKNP_R:
      ss << "SKNP V" << param1;
      break;
    case LD_RDT:
      ss << "LD V" << param1 << ", DT";
      break;
    case LD_RDK:
      ss << "LD V" << param1 << ", K";
      break;
    case LD_DTR:
      ss << "LD DT, V" << param1;
      break;
    case LD_STR:
      ss << "LD ST, V" << param1;
      break;
    case ADD_IR:
      ss << "ADD I, V" << param1;
      break;
    case LD_FR:
      ss << "LD F, V" << param1;
      break;
    case LD_BR:
      ss << "LD B, V" << param1;
      break;
    case LD_IR:
      ss << "LD I, V" << param1;
      break;
    case LD_RI:
      ss << "LD V" << param1 << ", I";
      break;
  }
  return ss.str();
}
/*Program Disassembler,if uncorrect code appears or failed,return false*/
void Dissassembler(std::vector<std::string> ASM,
                   std::vector<std::string>& DASM) {
	if (CheckCorrect(ASM)) {
		std::stringstream ss;
		for (auto &Line : ASM) {
			switch (Line[0]) {
			case '0':
				if (Line == "00E0") {
					DASM.push_back(ASMGen(CLS));
					break;
				}
				if (Line == "00EE") {
					DASM.push_back(ASMGen(RET));
					break;
				}
			case '1':
				DASM.push_back(ASMGen(JP_A, Line.substr(1, 3)));
				break;
			case '2':
				DASM.push_back(ASMGen(CALL_A, Line.substr(1, 3)));
				break;
			case '3':
				DASM.push_back(ASMGen(SE_RK, std::string(1,Line[1]), Line.substr(2, 2)));
				break;
			case '4':
				DASM.push_back(ASMGen(SNE_RK, std::string(1,Line[1]), Line.substr(2, 2)));
				break;
			case '5':
				DASM.push_back(ASMGen(SE_RR, std::string(1,Line[1]), std::string(1,Line[2])));
				break;
			case '6':
				DASM.push_back(ASMGen(LD_RK, std::string(1,Line[1]) , Line.substr(2, 2)));
				break;
			case '7':
				DASM.push_back(ASMGen(ADD_RK, std::string(1,Line[1]), Line.substr(2, 2)));
				break;
			case '8':  // Insturctions refer to calc
				switch (Line[3]) {
				case '0':
					DASM.push_back(ASMGen(LD_RR, std::string(1,Line[1]), std::string(1,Line[2])));
					break;
				case '1':
					DASM.push_back(ASMGen(OR_RR, std::string(1,Line[1]), std::string(1,Line[2])));
					break;
				case '2':
					DASM.push_back(ASMGen(AND_RR, std::string(1,Line[1]), std::string(1,Line[2])));
					break;
				case '3':
					DASM.push_back(ASMGen(XOR_RR, std::string(1,Line[1]), std::string(1,Line[2])));
					break;
				case '4':
					DASM.push_back(ASMGen(ADD_RR, std::string(1,Line[1]), std::string(1,Line[2])));
					break;
				case '5':
					DASM.push_back(ASMGen(SUB_RR, std::string(1,Line[1]), std::string(1,Line[2])));
					break;
				case '6':
					DASM.push_back(ASMGen(SHR_RR, std::string(1,Line[1]), std::string(1,Line[2])));
					break;
				case '7':
					DASM.push_back(ASMGen(SUBN_RR, std::string(1,Line[1]), std::string(1,Line[2])));
					break;
				case 'E':
					DASM.push_back(ASMGen(SHL_RR, std::string(1,Line[1]), std::string(1,Line[2])));
				}
				break;
			case '9':
				DASM.push_back(ASMGen(SNE_RR, std::string(1,Line[1]), std::string(1,Line[2])));
				break;
			case 'A':
				DASM.push_back(ASMGen(LD_IA, Line.substr(1, 3)));
				break;
			case 'B':
				DASM.push_back(ASMGen(JP_V0A, Line.substr(1, 3)));
			case 'C':
				DASM.push_back(ASMGen(RND_RK, std::string(1,Line[1]), Line.substr(2, 2)));
				break;
			case 'D':
				DASM.push_back(ASMGen(DRW_RRN, std::string(1,Line[1]), std::string(1,Line[2]),
					std::string(1, Line[3])));
				break;
			case 'E':
				if (Line.substr(2, 2) == "9E")
					DASM.push_back(ASMGen(SKP_R, std::string(1,Line[1])));
				else if (Line.substr(2, 2) == "A1")
					DASM.push_back(ASMGen(SKNP_R, std::string(1,Line[1])));
				break;
			case 'F':
				switch (Line[2]) {
				case '0':
					if (Line[3] == '7')
						DASM.push_back(ASMGen(LD_RDT, std::string(1,Line[1])));
					else if (Line[3] == 'A')
						DASM.push_back(ASMGen(LD_RDK, std::string(1,Line[1])));
					break;
				case '1':
					switch (Line[3]) {
					case '5':
						DASM.push_back(ASMGen(LD_DTR, std::string(1,Line[1])));
						break;
					case '8':
						DASM.push_back(ASMGen(LD_STR, std::string(1,Line[1])));
						break;
					case 'E':
						DASM.push_back(ASMGen(ADD_IR, std::string(1,Line[1])));
						break;
					}
					break;
				case '2':
					if (Line[3] == '9')
						DASM.push_back(ASMGen(LD_FR, std::string(1,Line[1])));
					break;
				case '3':
					if (Line[3] == '3')
						DASM.push_back(ASMGen(LD_BR, std::string(1,Line[1])));
					break;
				case '5':
					if (Line[3] == '5')
						DASM.push_back(ASMGen(LD_IR, std::string(1,Line[1])));
					break;
				case '6':
					if (Line[3] == '5')
						DASM.push_back(ASMGen(LD_RI, std::string(1,Line[1])));
					break;
				}
				break;
			}
		}
	}
}