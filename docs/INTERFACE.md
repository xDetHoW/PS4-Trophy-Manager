# PS4 Trophy Manager — interface specification

## Visual direction
- 1920×1080 target layout, dark charcoal/black background.
- Deep red accent for selection, progress and section markers.
- High-contrast white primary text and muted grey secondary text.
- Controller-first layout; important actions shown as button hints.

## Main navigation
1. Trophy Library
2. Statistics
3. Backup Data
4. Import / Export
5. About

D-pad Up/Down changes the selected section. Cross confirms. Circle backs out of a dialog or detail view. Options opens the app menu. Navigation wraps at the ends.

## Screen layouts

### Trophy Library
Left column: game list with title, title ID and earned/total count. Right panel: selected game's trophy list, grade icon, trophy name and earned state. Filters: All, Earned, Unearned, Hidden.

### Statistics
Summary tiles: games found, trophies earned, total trophies, completion percentage. Grade breakdown: platinum, gold, silver and bronze. Figures are calculated from locally readable data only.

### Backup Data
List local backups with timestamp and size. Actions: create, inspect, restore and delete. Restore and delete require confirmation; never overwrite source data silently.

### Import / Export
Show supported local backup formats and validation result before importing. Export writes a separate backup file and never modifies trophy records.

### About
Application version, firmware/build information, data-source status and diagnostics.

## Interaction and safety
- First release is read-only for trophy data.
- Backup/restore is limited to supported local files and must validate paths and contents.
- No trophy unlocking, fabrication, editing, PSN synchronization or account/server modification.
- Empty states and unsupported APIs must be explicit; never display invented trophy data.

## Current implementation status
The platform-independent menu state and trophy data structures are added in src/trophy_manager.h and src/trophy_manager.c. The native screen renderer, PS4 trophy-data discovery, statistics aggregation, backup implementation, localization and on-console testing remain unimplemented.
