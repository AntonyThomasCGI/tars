#include "tars_state.hpp"

#include <atomic>


// Global to this translation unit.
static std::atomic<State> state;


/* TARS_setState safely sets TARS' state.
 */
void TARS_setState(State newState)
{
    state.store(newState, std::memory_order_release);
};

/* TARS_getState safely accesses the current state of TARS.
 */
State TARS_getState()
{
    State current = state.load(std::memory_order_acquire);

    return current;
};
