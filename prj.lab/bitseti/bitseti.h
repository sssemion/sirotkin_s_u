//
// Created by sssemion on 27.04.2022.
//

#ifndef SIROTKIN_S_U_BITSETI_H
#define SIROTKIN_S_U_BITSETI_H

#include <iosfwd>
class BitsetI {
    virtual std::istream& readFrom(std::istream &istrm)=0;
    virtual std::ostream& writeTo(std::ostream &ostrm) const=0;
};

#endif //SIROTKIN_S_U_BITSETI_H
