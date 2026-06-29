#include "definitions.hpp"
#include "memory.hpp"
#include <vector>


class CPU {
    public :

        CPU(std::vector<u8>& ROM_data, Memory& memory) ;
        auto write(u16 adress)-> void;
        auto read(u16 adress) ->u8 ;

    private :

        auto write_register(char* lettre)->void;
        auto read_register(char* lettre);

        std::vector<u8> ROM_data ;
        u16 PC ;//Program Counter
        u16 SP ; //Stack Counter
        u8 register_A ;
        u8 register_B ;
        u8 register_C ;
        u8 register_D ;
        u8 register_E ;
        u8 register_H ;
        u8 register_L ;
        u8 flags ;


} ;
