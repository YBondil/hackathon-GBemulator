#include "definitions.hpp"
#include "registers.hpp"
#include "memory.hpp"
#include <vector>


class CPU {
    public :
        CPU(std::vector<u8>& ROM_data, Memory& memory) ;
        auto write(u16 adress)-> void;
        auto read(u16 adress) -> u8 ;

        auto run();

    private :
        Memory& memory ;
        auto write_register(char* lettre)->void;
        auto read_register(char* lettre);

        std::vector<u8> ROM_data ;
        u16 PC ;//Program Counter
        u16 SP ; //Stack Counter

        //registers
        Register A,B,C,D,E,H,L ;
        FlagRegister F;
        RegisterPair AF, BD, DE, HL;


        // clock ?


        //OPCodes 

        auto opcode_adc(u8 value)->void;

} ;
