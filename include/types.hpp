/*Here are some self-named types,for my personal purpose*/
typedef char byte;
typedef uint16_t PLine;  // A Program line in hex.E.G. 00EE
typedef uint8_t BIT8;    // 8BIT data
typedef uint16_t BIT16;  // 16BIT data

/*Chip-8 Instructions*/
enum C8Ins {
  // A - Address | R - 8-bit Registers | I - The 16-bit register I | K - 8-bit
  // value | N - 4-bit value
  // V0 ~ VF stands for the especial register
  // ST - Sound Timer | DT - Delay Timer
  // DK - Key
  SYS_A,  // 0nnn|Jump to a machine code routine at nnn.This instruction is only
          // used on the old computers on which Chip-8 was originally
          // implemented. It is ignored by modern interpreters.
  CLS,    // 00E0|Clear the display
  RET,    // 00EE|Return from a subroutine.The interpreter sets the program
  // counter to the address at the top of the stack, then subtracts 1 from
  // the stack pointer.
  JP_A,    // 1nnn|Jump to location nnn.The interpreter sets the program counter
           // to nnn.
  CALL_A,  // 2nnn|Call subroutine at nnn.The interpreter increments the stack
           // pointer, then puts the current PC on the top of the stack. The PC
           // is then set to nnn.
  SE_RK,   // 3xkk|Skip next instruction if Vx = kk.The interpreter compares
           // register Vx to kk, and if they are equal, increments the program
           // counter by 2.
  SNE_RK,  // 4xkk|Skip next instruction if Vx != kk.The interpreter compares
           // register Vx to kk, and if they are not equal, increments the
           // program counter by 2.
  SE_RR,   // 5xy0|Skip next instruction if Vx = Vy.The interpreter compares
           // register Vx to register Vy, and if they are equal, increments the
           // program counter by 2.
  LD_RK,   // 6xkk|Set Vx = kk.The interpreter puts the value kk into register
           // Vx.
  ADD_RK,  // 7xkk|Set Vx = Vx + kk.Adds the value kk to the value of register
           // Vx, then stores the result in Vx.
  LD_RR,   // 8xy0|Set Vx = Vy.Stores the value of register Vy in regsiter Vx.
  OR_RR,   // 8xy1|Set Vx = Vx OR Vy.Performs a bitwise OR on the values of Vx
           // and Vy,
           // then stores the result in Vx.
  AND_RR,  // 8xy2|Set Vx = Vx AND Vy.
  XOR_RR,  // 8xy3|Set Vx = Vx XOR Vy
  ADD_RR,  // 8xy4|Set Vx = Vx + Vy,set VF = carry.If the result is greater than
           // 8 bits (i.e., > 255,) VF is set to 1,otherwise 0.
  SUB_RR,  // 8xy5|Set Vx = Vx - Vy, set VF = NOT borrow.If Vx > Vy, then VF is
           // set to 1, otherwise 0.Then Vy is subtracted from Vx, and the
           // results stored in Vx.
  SHR_RR,  // 8xy6|Set Vx = Vx SHR 1.If the least-significant bit of Vx is 1,
           // then VF is set to 1, otherwise 0. Then Vx is divided by 2.
  SUBN_RR,  // 8xy7|Set Vx = Vy - Vx, set VF = NOT borrow.If Vy > Vx, then VF is
            // set to 1, otherwise 0. Then Vx is subtracted from Vy, and the
            // results stored in Vx.
  SHL_RR,   // 8xyE|Set Vx = Vx SHL 1.If the most-significant bit of Vx is 1,
            // then VF is set to 1, otherwise to 0. Then Vx is multiplied by 2.

  SNE_RR,  // 9xy0|Skip next instruction if Vx != Vy.The values of Vx and Vy are
           // compared, and if they are not equal, the program counter is
           // increased by 2.
  LD_IA,   // Annn|Set I = nnn.
  JP_V0A,  // Bnnn|Jump to location nnn + V0.The program counter is set to nnn
           // plus the value of V0.
  RND_RK,  // Cxkk|Set Vx = random byte AND kk.The interpreter generates a
           // random number from 0 to 255, which is then ANDed with the value
           // kk. The results are stored in Vx. See instruction 8xy2 for more
           // information on AND.
  DRW_RRN,  // Dxyn|Display n-byte sprite starting at memory location I at
            // (Vx,Vy), set VF = collision.The interpreter reads n bytes from
            // memory, starting at the address stored in I. These bytes are then
            // displayed as sprites on screen at coordinates (Vx, Vy). Sprites
            // are XORed onto the existing screen. If this causes any pixels to
            // be erased, VF is set to 1, otherwise it is set to 0. If the
            // sprite is positioned so part of it is outside the coordinates of
            // the display, it wraps around to the opposite side of the screen.
            // See instruction 8xy3 for more information on XOR, and section
            // 2.4, Display, for more information on the Chip-8 screen and
            // sprites.
  SKP_R,    // Ex9E|Skip next instruction if key with the value of Vx is
            // pressed.Checks the keyboard, and if the key corresponding to the
            // value of Vx is currently in the down position, PC is increased by
            // 2.
  SKNP_R,   // ExA1|Skip next instruction if key with the value of Vx is not
            // pressed.Checks the keyboard, and if the key corresponding to the
  // value of Vx is currently in the up position, PC is increased by 2.
  LD_RDT,  // Fx07|Set Vx = delay timer value.The value of DT is placed into Vx.
  LD_RDK,  // Fx0A|Wait for a key press, store the value of the key in Vx.All
           // execution stops until a key is pressed, then the value of that key
           // is stored in Vx.
  LD_DTR,  // Fx15|Set delay timer = Vx.
  LD_STR,  // Fx18|Set sound timer = Vx.
  ADD_IR,  // Fx1E|Set I = I + Vx.
  LD_FR,   // Fx29|Set I = location of sprite for digit Vx.
  LD_BR,  // Fx33|Store BCD representation of Vx in memory locations I, I+1, and
          // I+2.
  LD_IR,  // Fx55|Store registers V0 through Vx in memory starting at location
          // I.
  LD_RI   // Fx65|Read registers V0 through Vx from memory starting at location
          // I.
};