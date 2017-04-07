# libc8dasm
Disassembler of chip-8 prog.

##Usage
```C++
//File named "UFO"
std::string prog = ReadFile("UFO"); // This reads the file in binary mode and generates the HEX value string
//The Raw prog hex
std::vector<std::string> Raw = {"A2CD","6938","6A08","D9A3"}; // First 4 byte of the UFO game
std::vector<std::string> Output;
Dissassembler(Raw,Output);

ofstream OutFile("OUT.asm");
for(auto x : Output)
	OutFile << x << std::endl; //outputs to file

/*Contents of OUT.asm:
LD I, 02CD
LD V9, 38
LD VA, 08
DRW V9, VA, 3
*/
```

