#include "definitions.hpp"
#include "memory.hpp"
#include <vector>


class CPU {
    public :
        CPU(std::vector<u8>& ROM_data, Memory& memory) ;
        auto write(u16 adress)-> void;
        auto read(u16 adress) ->u8 ;

    private :
        std::vector<u8> ROM_data ;
        u8 registe_A ;
        u8 registe_B ;
        u8 registe_C ;
        u8 registe_D ;
        u8 registe_E ;
        u8 registe_F ;
        u8 registe_G ;
        u8 registe_H ;
        u8 registe_A ;
        u8 registe_A ;

} ;
