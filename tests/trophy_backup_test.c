#include "../src/trophy_backup.h"
#include <assert.h>
#include <string.h>
#include <stdio.h>

static TmGame source_games[1];
static TmGame restored_games[1];

int main(void) {
    memset(source_games, 0, sizeof source_games);
    strcpy(source_games[0].title_id, "CUSA12345");
    strcpy(source_games[0].title, "Example Game");
    source_games[0].trophy_count = 1;
    strcpy(source_games[0].trophies[0].id, "trophy01");
    strcpy(source_games[0].trophies[0].name, "First Trophy");
    source_games[0].trophies[0].grade = TM_TROPHY_GOLD;
    source_games[0].trophies[0].earned = 1;

    FILE* f = tmpfile();
    assert(f);
    assert(tm_backup_write(f, source_games, 1));
    rewind(f);
    size_t count = 0;
    assert(tm_backup_read(f, restored_games, 1, &count));
    assert(count == 1);
    assert(strcmp(restored_games[0].title, "Example Game") == 0);
    assert(strcmp(restored_games[0].trophies[0].name, "First Trophy") == 0);
    assert(restored_games[0].earned_count == 1);
    fclose(f);
    return 0;
}
