#include "../src/trophy_manager.h"
#include <assert.h>
#include <string.h>

int main(void) {
    TmUiState ui;
    tm_ui_init(&ui);
    assert(ui.screen == TM_SCREEN_LIBRARY);
    tm_ui_move(&ui, 1);
    assert(ui.screen == TM_SCREEN_STATISTICS);
    tm_ui_move(&ui, -1);
    assert(ui.screen == TM_SCREEN_LIBRARY);

    TmGame games[1] = {0};
    games[0].trophy_count = 3;
    games[0].trophies[0].grade = TM_TROPHY_BRONZE;
    games[0].trophies[0].earned = 1;
    games[0].trophies[1].grade = TM_TROPHY_GOLD;
    games[0].trophies[1].earned = 1;
    games[0].trophies[2].grade = TM_TROPHY_PLATINUM;
    games[0].trophies[2].earned = 0;

    TmStatistics stats;
    tm_stats_calculate(games, 1, &stats);
    assert(stats.games_found == 1);
    assert(stats.trophies_total == 3);
    assert(stats.trophies_earned == 2);
    assert(stats.bronze_earned == 1);
    assert(stats.gold_earned == 1);
    assert(stats.platinum_earned == 0);
    assert(stats.completion_percent == 66);
    assert(strcmp(tm_text(TM_LANG_ITALIAN, "statistics"), "STATISTICHE") == 0);
    return 0;
}
