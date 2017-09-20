#include <ostream>

#ifndef _PONTO_H
#define _PONTO_H

class Ponto {
    public:
        int x, y, z;
        Ponto(int x, int y);
        Ponto(int x, int y, int z);
        friend std::ostream& operator<< (std::ostream& strm, const Ponto& p);

    private:
        bool xyz;
};

#endif // _PONTO_H