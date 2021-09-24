

#ifndef TARS_INTENT_HPP_INCLUDED
#define TARS_INTENT_HPP_INCLUDED

#include <cstddef>

typedef struct {
    char  intent;
    float certainty;
} TARS_Intent;


TARS_Intent TARS_getIntent(const char *audioBinary, std::size_t binaryLen);


#endif
