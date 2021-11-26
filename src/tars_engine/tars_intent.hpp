

#ifndef TARS_INTENT_HPP_INCLUDED
#define TARS_INTENT_HPP_INCLUDED

#include <cstddef>
#include <string>

typedef struct {
    std::string text; 
    std::string intent;
    float        confidence;
} TARS_Intent;


TARS_Intent TARS_getIntent(const char *audioBinary, std::size_t binaryLen);


#endif
