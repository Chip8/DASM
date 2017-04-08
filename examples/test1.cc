#include <iostream>
#include "../include/libc8dasm.hpp"
int main(){
    std::string Pong = libc8ReadFile("./games/PONG");
    try{
        std::vector<std::string> GameData = libc8SplitLine(Pong);
        if(!libc8CheckCorrect(GameData))
         throw(1);
        std::ofstream DASMOutPutFile("PONG.asm");
        std::vector<std::string> DASM;
        libc8DASM(GameData,DASM);
        for(auto ProgLine : DASM)
            DASMOutPutFile<<ProgLine<<std::endl;
        std::cout<<"Successed!"<<std::endl;
        return 0;
    }
    catch(int){
        std::cerr<<"Erro!Program incorrect!"<<std::endl;
        return -1;
    }
}
