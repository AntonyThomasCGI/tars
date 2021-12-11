

#ifndef TARS_INTENT_HPP_INCLUDED
#define TARS_INTENT_HPP_INCLUDED

#include <cstddef>
#include <string>
#include <vector>


typedef struct {
    std::string type;
    std::string value;
    float        confidence;

} TARS_Entity;


typedef struct {
    std::string text; 
    std::string intent;
    float        confidence;
    std::vector<TARS_Entity> entities;
} TARS_Intent;


TARS_Intent TARS_getIntent(const char *audioBinary, std::size_t binaryLen);


#endif
