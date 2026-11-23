#pragma once
#include "trophy_manager.h"
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Versioned, portable text backup. The caller owns FILE and game buffers.
 * Import only populates caller-provided memory; it never touches PSN or system trophy files. */
int tm_backup_write(FILE* stream, const TmGame* games, size_t game_count);
int tm_backup_read(FILE* stream, TmGame* games, size_t game_capacity, size_t* games_read);

#ifdef __cplusplus
}
#endif
