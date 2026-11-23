#include "trophy_manager.h"
#include <string.h>

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

void tm_stats_calculate(const TmGame* games, size_t game_count, TmStatistics* out) {
    if (!out) return;
    TmStatistics result = {0};
    if (!games) { *out = result; return; }
    for (size_t i = 0; i < game_count; ++i) {
        const TmGame* game = &games[i];
        result.games_found++;
        uint16_t count = game->trophy_count;
        if (count > TM_MAX_TROPHIES_PER_GAME) count = TM_MAX_TROPHIES_PER_GAME;
        for (uint16_t j = 0; j < count; ++j) {
            const TmTrophy* trophy = &game->trophies[j];
            result.trophies_total++;
            if (!trophy->earned) continue;
            result.trophies_earned++;
            switch (trophy->grade) {
                case TM_TROPHY_BRONZE: result.bronze_earned++; break;
                case TM_TROPHY_SILVER: result.silver_earned++; break;
                case TM_TROPHY_GOLD: result.gold_earned++; break;
                case TM_TROPHY_PLATINUM: result.platinum_earned++; break;
                default: break;
            }
        }
    }
    if (result.trophies_total)
        result.completion_percent = (uint8_t)((result.trophies_earned * 100u) / result.trophies_total);
    *out = result;
}

typedef struct { const char* key; const char* en; const char* it; } TmTranslation;
static const TmTranslation k_translations[] = {
    {"library", "TROPHY LIBRARY", "RACCOLTA TROFEI"},
    {"statistics", "STATISTICS", "STATISTICHE"},
    {"backup", "BACKUP DATA", "BACKUP DATI"},
    {"import_export", "IMPORT / EXPORT", "IMPORTA / ESPORTA"},
    {"about", "ABOUT", "INFORMAZIONI"},
    {"games_found", "GAMES FOUND", "GIOCHI TROVATI"},
    {"earned", "EARNED", "OTTENUTI"},
    {"total", "TOTAL", "TOTALE"},
    {"coming_soon", "NOT IMPLEMENTED YET", "NON ANCORA IMPLEMENTATO"}
};

const char* tm_text(TmLanguage language, const char* key) {
    if (!key) return "";
    for (size_t i = 0; i < sizeof(k_translations) / sizeof(k_translations[0]); ++i) {
        if (strcmp(key, k_translations[i].key) == 0)
            return language == TM_LANG_ITALIAN ? k_translations[i].it : k_translations[i].en;
    }
    return key;
}
