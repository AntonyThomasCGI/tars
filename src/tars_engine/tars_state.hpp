#ifndef TARS_STATE_HPP_INCLUDED
#define TARS_STATE_HPP_INCLUDED

enum State {LISTENING, AUDIO_EVENT, SPEAKING, SHUT_DOWN};

State TARS_getState();
void TARS_setState(State newState);


#endif
