#include "trophy_manager.h"

static const char* const k_screen_titles[TM_SCREEN_COUNT] = {"TROPHY LIBRARY", "STATISTICS", "BACKUP DATA", "IMPORT / EXPORT", "ABOUT"};

void tm_ui_init(TmUiState* state) {
    if (!state) return;
    state->screen = TM_SCREEN_LIBRARY;
    state->selected = 0;
    state->dialog_open = 0;
}

void tm_ui_move(TmUiState* state, int direction) {
    if (!state || state->dialog_open || direction == 0) return;
    int next = (int)state->screen + (direction > 0 ? 1 : -1);
    if (next < 0) next = TM_SCREEN_COUNT - 1;
    if (next >= TM_SCREEN_COUNT) next = 0;
    state->screen = (TmScreen)next;
    state->selected = (uint8_t)next;
}

void tm_ui_confirm(TmUiState* state) { if (state) state->dialog_open = 0; }
void tm_ui_back(TmUiState* state) { if (state) state->dialog_open = 0; }

const char* tm_screen_title(TmScreen screen) {
    if ((unsigned)screen >= TM_SCREEN_COUNT) return "PS4 TROPHY MANAGER";
    return k_screen_titles[screen];
}
