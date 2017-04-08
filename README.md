# libc8dasm
Disassembler for chip-8 programs.

Chip-8 Instructions table references to:[Cowgod's Chip-8 Technical Reference v1.0](http://devernay.free.fr/hacks/chip8/C8TECH10.HTM#3.1)
## Usage

Simply include the header `libc8dasm.hpp`.

### E.G.
```C++
//File named "UFO"
std::string Prog = libc8ReadFile("UFO"); // This will read the file in binary mode and generates the HEX value string
//The Raw prog hex
std::vector<std::string> Raw = {"A2CD","6938","6A08","D9A3"}; // The first 4 Line of the UFO game

std::vector<std::string> Output;
libc8DASM(Raw,Output);

ofstream OutFile("OUT.asm");
for(auto x : Output)
	OutFile << x << std::endl; //outputs to file
```


Contents of OUT.asm would be:
```asm
LD I, 02CD
LD V9, 38
LD VA, 08
DRW V9, VA, 3
```