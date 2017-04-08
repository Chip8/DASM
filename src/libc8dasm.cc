#include "../include/libc8dasm.hpp"
#include <iomanip>
#include <iterator>
#include <sstream>

/*Read programe file and store it into a char array in hex*/
std::string libc8ReadFile(const char* c8ProgramFile) {
  // Since the Chip-8 program usually won't be greater than 1KB,so I choose
  // string for temperary storage just in mind.
  std::ifstream c8Prog(c8ProgramFile, std::ios::binary);
  std::vector<PLine> data((std::istreambuf_iterator<byte>(c8Prog)),
                          std::istreambuf_iterator<byte>());
  std::stringstream c8RawStream;
  for (auto i : data) {
    c8RawStream << std::setw(2) << std::setfill('0') << std::hex
                << std::uppercase << int(i);
  }
  return c8RawStream.str();
}

/* Split the raw program string into a 4-hex-num based vector*/
std::vector<std::string> libc8SplitLine(const std::string& c8RawString,
                                        int HexLength) {
  int NumPLine =
      c8RawString.length() / HexLength;  // The number of Program Line
  std::vector<std::string> c8ASM;  // Yes,it's for produce the asm source code.

  for (auto i = 0; i < NumPLine; i++) {
    c8ASM.push_back(c8RawString.substr(i * HexLength, HexLength));
  }

  // If there are leftover characters, create a shorter item at the end.
  if (c8RawString.length() % HexLength != 0) {
    c8ASM.push_back(c8RawString.substr(HexLength * NumPLine));
  }
  return c8ASM;
}

/*Check if the program is correct*/
bool libc8CheckCorrect(std::vector<std::string> c8RawString) {
  if (c8RawString.empty()) return false;
  for (auto x : c8RawString) {
    std::stringstream c8RawStream;
    if (x.length() != 4) return false;  // Uncomplete program?
    c8RawStream << x;
    PLine temp;
    c8RawStream >> std::hex >> temp;
    if (temp < 0x0000 || temp > 0xFFFF) return false;  // out of range?
  }
  return true;
}
std::string libc8GenAsm(C8Ins Ins, std::string param1 = "",
                        std::string param2 = "", std::string param3 = "") {
  std::stringstream c8AsmOutStream;
  switch (Ins) {
    case CLS:
      c8AsmOutStream << "CLS";
      break;
    case RET:
      c8AsmOutStream << "RET";
      break;
    case SYS_A:
      c8AsmOutStream << "SYS " << std::setfill('0') << std::setw(4) << param1;
      break;
    case JP_A:
      c8AsmOutStream << "JP " << std::setfill('0') << std::setw(4) << param1;
      break;
    case CALL_A:
      c8AsmOutStream << "CALL " << std::setfill('0') << std::setw(4) << param1;
      break;
    case SE_RK:
      c8AsmOutStream << "SE V" << param1 << "," << std::setfill('0')
                     << std::setw(4) << param2;
      break;
    case SE_RR:
      c8AsmOutStream << "SE V" << param1 << ", V" << param2;
      break;
    case SNE_RK:
      c8AsmOutStream << "SNE V" << param1 << ", " << std::setfill('0')
                     << std::setw(4) << param2;
      break;
    case SNE_RR:
      c8AsmOutStream << "SNE V" << param1 << ", V" << param2;
      break;
    case LD_RK:
      c8AsmOutStream << "LD V" << param1 << ", " << std::setfill('0')
                     << std::setw(2) << param2;
      break;
    case LD_RR:
      c8AsmOutStream << "LD V" << param1 << ", V" << param2;
      break;
    case ADD_RK:
      c8AsmOutStream << "ADD V" << param1 << ", " << param2;
      break;
    case ADD_RR:
      c8AsmOutStream << "ADD V" << param1 << ", V" << param2;
      break;
    case OR_RR:
      c8AsmOutStream << "OR V" << param1 << ", V" << param2;
      break;
    case AND_RR:
      c8AsmOutStream << "AND V" << param1 << ", V" << param2;
      break;
    case XOR_RR:
      c8AsmOutStream << "XOR V" << param1 << ", V" << param2;
      break;
    case SUB_RR:
      c8AsmOutStream << "SUB V" << param1 << ", V" << param2;
      break;
    case SHR_RR:
      c8AsmOutStream << "SHR V" << param1 << "{, V" << param2 << "}";
      break;
    case SUBN_RR:
      c8AsmOutStream << "SUBN V" << param1 << ", V" << param2;
      break;
    case SHL_RR:
      c8AsmOutStream << "SHL V" << param1 << "{, V" << param2 << "}";
      break;
    case LD_IA:
      c8AsmOutStream << "LD I, " << std::setfill('0') << std::setw(4) << param1;
      break;
    case JP_V0A:
      c8AsmOutStream << "JP V0, " << std::setfill('0') << std::setw(4)
                     << param1;
      break;
    case RND_RK:
      c8AsmOutStream << "RND V" << param1 << ", " << param2;
      break;
    case DRW_RRN:
      c8AsmOutStream << "DRW V" << param1 << ", V" << param2 << ", " << param3;
      break;
    case SKP_R:
      c8AsmOutStream << "SKP V" << param1;
      break;
    case SKNP_R:
      c8AsmOutStream << "SKNP V" << param1;
      break;
    case LD_RDT:
      c8AsmOutStream << "LD V" << param1 << ", DT";
      break;
    case LD_RDK:
      c8AsmOutStream << "LD V" << param1 << ", K";
      break;
    case LD_DTR:
      c8AsmOutStream << "LD DT, V" << param1;
      break;
    case LD_STR:
      c8AsmOutStream << "LD ST, V" << param1;
      break;
    case ADD_IR:
      c8AsmOutStream << "ADD I, V" << param1;
      break;
    case LD_FR:
      c8AsmOutStream << "LD F, V" << param1;
      break;
    case LD_BR:
      c8AsmOutStream << "LD B, V" << param1;
      break;
    case LD_IR:
      c8AsmOutStream << "LD I, V" << param1;
      break;
    case LD_RI:
      c8AsmOutStream << "LD V" << param1 << ", I";
      break;
  }
  return c8AsmOutStream.str();
}
/*Program libcC8DASM,if uncorrect code appears or failed,return false*/
void libc8DASM(std::vector<std::string> ASM, std::vector<std::string>& DASM) {
  std::stringstream ss;
  for (auto& Line : ASM) {
    switch (Line[0]) {
      case '0':
        if (Line == "00E0") {
          DASM.push_back(libc8GenAsm(CLS));
          break;
        }
        if (Line == "00EE") {
          DASM.push_back(libc8GenAsm(RET));
          break;
        }
      case '1':
        DASM.push_back(libc8GenAsm(JP_A, Line.substr(1, 3)));
        break;
      case '2':
        DASM.push_back(libc8GenAsm(CALL_A, Line.substr(1, 3)));
        break;
      case '3':
        DASM.push_back(
            libc8GenAsm(SE_RK, std::string(1, Line[1]), Line.substr(2, 2)));
        break;
      case '4':
        DASM.push_back(
            libc8GenAsm(SNE_RK, std::string(1, Line[1]), Line.substr(2, 2)));
        break;
      case '5':
        DASM.push_back(libc8GenAsm(SE_RR, std::string(1, Line[1]),
                                   std::string(1, Line[2])));
        break;
      case '6':
        DASM.push_back(
            libc8GenAsm(LD_RK, std::string(1, Line[1]), Line.substr(2, 2)));
        break;
      case '7':
        DASM.push_back(
            libc8GenAsm(ADD_RK, std::string(1, Line[1]), Line.substr(2, 2)));
        break;
      case '8':  // Insturctions refer to calc
        switch (Line[3]) {
          case '0':
            DASM.push_back(libc8GenAsm(LD_RR, std::string(1, Line[1]),
                                       std::string(1, Line[2])));
            break;
          case '1':
            DASM.push_back(libc8GenAsm(OR_RR, std::string(1, Line[1]),
                                       std::string(1, Line[2])));
            break;
          case '2':
            DASM.push_back(libc8GenAsm(AND_RR, std::string(1, Line[1]),
                                       std::string(1, Line[2])));
            break;
          case '3':
            DASM.push_back(libc8GenAsm(XOR_RR, std::string(1, Line[1]),
                                       std::string(1, Line[2])));
            break;
          case '4':
            DASM.push_back(libc8GenAsm(ADD_RR, std::string(1, Line[1]),
                                       std::string(1, Line[2])));
            break;
          case '5':
            DASM.push_back(libc8GenAsm(SUB_RR, std::string(1, Line[1]),
                                       std::string(1, Line[2])));
            break;
          case '6':
            DASM.push_back(libc8GenAsm(SHR_RR, std::string(1, Line[1]),
                                       std::string(1, Line[2])));
            break;
          case '7':
            DASM.push_back(libc8GenAsm(SUBN_RR, std::string(1, Line[1]),
                                       std::string(1, Line[2])));
            break;
          case 'E':
            DASM.push_back(libc8GenAsm(SHL_RR, std::string(1, Line[1]),
                                       std::string(1, Line[2])));
        }
        break;
      case '9':
        DASM.push_back(libc8GenAsm(SNE_RR, std::string(1, Line[1]),
                                   std::string(1, Line[2])));
        break;
      case 'A':
        DASM.push_back(libc8GenAsm(LD_IA, Line.substr(1, 3)));
        break;
      case 'B':
        DASM.push_back(libc8GenAsm(JP_V0A, Line.substr(1, 3)));
      case 'C':
        DASM.push_back(
            libc8GenAsm(RND_RK, std::string(1, Line[1]), Line.substr(2, 2)));
        break;
      case 'D':
        DASM.push_back(libc8GenAsm(DRW_RRN, std::string(1, Line[1]),
                                   std::string(1, Line[2]),
                                   std::string(1, Line[3])));
        break;
      case 'E':
        if (Line.substr(2, 2) == "9E")
          DASM.push_back(libc8GenAsm(SKP_R, std::string(1, Line[1])));
        else if (Line.substr(2, 2) == "A1")
          DASM.push_back(libc8GenAsm(SKNP_R, std::string(1, Line[1])));
        break;
      case 'F':
        switch (Line[2]) {
          case '0':
            if (Line[3] == '7')
              DASM.push_back(libc8GenAsm(LD_RDT, std::string(1, Line[1])));
            else if (Line[3] == 'A')
              DASM.push_back(libc8GenAsm(LD_RDK, std::string(1, Line[1])));
            break;
          case '1':
            switch (Line[3]) {
              case '5':
                DASM.push_back(libc8GenAsm(LD_DTR, std::string(1, Line[1])));
                break;
              case '8':
                DASM.push_back(libc8GenAsm(LD_STR, std::string(1, Line[1])));
                break;
              case 'E':
                DASM.push_back(libc8GenAsm(ADD_IR, std::string(1, Line[1])));
                break;
            }
            break;
          case '2':
            if (Line[3] == '9')
              DASM.push_back(libc8GenAsm(LD_FR, std::string(1, Line[1])));
            break;
          case '3':
            if (Line[3] == '3')
              DASM.push_back(libc8GenAsm(LD_BR, std::string(1, Line[1])));
            break;
          case '5':
            if (Line[3] == '5')
              DASM.push_back(libc8GenAsm(LD_IR, std::string(1, Line[1])));
            break;
          case '6':
            if (Line[3] == '5')
              DASM.push_back(libc8GenAsm(LD_RI, std::string(1, Line[1])));
            break;
        }
        break;
    }
  }
}