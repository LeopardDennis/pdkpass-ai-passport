#include <assert.h>
#include "pdkpass_model.h"

int main(void)
{
    pdkpass_state_t state;
    pdkpass_state_init(&state);
    assert(state.page == PDKPASS_PAGE_HOME);
    assert(state.selected_race == 0);

    pdkpass_state_handle(&state, PDKPASS_INPUT_UP, 11, 23);
    assert(state.page == PDKPASS_PAGE_STANDINGS);
    pdkpass_state_handle(&state, PDKPASS_INPUT_UP, 11, 23);
    assert(state.selected_driver == 22);
    pdkpass_state_handle(&state, PDKPASS_INPUT_BACK, 11, 23);
    assert(state.page == PDKPASS_PAGE_HOME);

    pdkpass_state_handle(&state, PDKPASS_INPUT_DOWN, 11, 23);
    assert(state.page == PDKPASS_PAGE_CALENDAR);
    pdkpass_state_handle(&state, PDKPASS_INPUT_DOWN, 11, 23);
    assert(state.selected_race == 1);
    pdkpass_state_handle(&state, PDKPASS_INPUT_UP, 11, 23);
    assert(state.selected_race == 0);
    pdkpass_state_handle(&state, PDKPASS_INPUT_UP, 11, 23);
    assert(state.selected_race == 10);

    pdkpass_state_handle(&state, PDKPASS_INPUT_OK, 11, 23);
    assert(state.page == PDKPASS_PAGE_RACE_DETAIL);
    pdkpass_state_handle(&state, PDKPASS_INPUT_DOWN, 11, 23);
    assert(state.selected_race == 0);
    pdkpass_state_handle(&state, PDKPASS_INPUT_OK, 11, 23);
    assert(state.page == PDKPASS_PAGE_CALENDAR);
    return 0;
}
