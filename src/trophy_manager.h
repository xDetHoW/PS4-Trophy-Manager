#pragma once
#include <stdint.h>
#include <stddef.h>

/* Platform-independent Trophy Manager data model and menu state.
 * This module never writes trophy records or communicates with PSN. */
#ifdef __cplusplus
extern "C" {
#endif
#define TM_MAX_GAMES 256
#define TM_MAX_TROPHIES_PER_GAME 256
#define TM_TITLE_MAX 128
#define TM_ID_MAX 16

typedef enum { TM_TROPHY_BRONZE=0, TM_TROPHY_SILVER=1, TM_TROPHY_GOLD=2, TM_TROPHY_PLATINUM=3 } TmTrophyGrade;
typedef struct { char id[TM_ID_MAX]; char name[TM_TITLE_MAX]; TmTrophyGrade grade; uint8_t earned; uint8_t hidden; } TmTrophy;
typedef struct { char title_id[TM_ID_MAX]; char title[TM_TITLE_MAX]; uint16_t trophy_count; uint16_t earned_count; TmTrophy trophies[TM_MAX_TROPHIES_PER_GAME]; } TmGame;
typedef enum { TM_SCREEN_LIBRARY=0, TM_SCREEN_STATISTICS, TM_SCREEN_BACKUP, TM_SCREEN_IMPORT_EXPORT, TM_SCREEN_ABOUT, TM_SCREEN_COUNT } TmScreen;
typedef struct { TmScreen screen; uint8_t selected; uint8_t dialog_open; } TmUiState;
void tm_ui_init(TmUiState* state);
void tm_ui_move(TmUiState* state, int direction);
void tm_ui_confirm(TmUiState* state);
void tm_ui_back(TmUiState* state);
const char* tm_screen_title(TmScreen screen);
#ifdef __cplusplus
}
#endif
