

#ifndef TARS_INTENT_HPP_INCLUDED
#define TARS_INTENT_HPP_INCLUDED

#include <string>

typedef struct {
    char  intent;
    float certainty;
} TARS_Intent;


TARS_Intent TARS_getIntent(std::string raw_audio);


#endif
