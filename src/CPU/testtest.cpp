

#include <iostream>

template <typename T>
void affiche(T x){
    std::cout << x << std::endl;
}

int main(){
    u_int8_t n = 25;
    u_int8_t m = 254;
    u_int8_t somme = n+m;
    affiche(n);
    affiche(m);
    affiche(somme);
    return 0;
}

