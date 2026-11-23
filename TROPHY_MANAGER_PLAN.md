# PS4 Trophy Manager

A local-first PS4 homebrew project focused on viewing and backing up trophy data.

## Project status

**Prototype planning stage.** The repository currently contains the original ShadCube4 renderer template. The Trophy Manager interface and trophy-data features are not implemented yet. Do not treat the current PKG as a Trophy Manager build.

## Goals

- Read and display trophy information available locally on the console.
- Organize a local trophy library and show completion statistics.
- Export and restore local backups where the PS4 APIs and permissions allow it.
- Provide import/export workflows for supported backup formats.
- Use a dark black/red visual style and support DualShock 4 navigation.
- Keep all operations local and transparent.

## Out of scope

- Creating, unlocking, editing, or fabricating trophy records.
- Changing PSN account/server records or synchronizing artificial trophies.
- Bypassing PlayStation account or server-side trophy validation.

## Planned screens

1. **Trophy Library** — list locally available games and trophy groups.
2. **Statistics** — totals and completion percentages calculated from readable local data.
3. **Backup Data** — create, inspect, and restore supported local backups.
4. **Import / Export** — handle documented, compatible local backup formats.
5. **About** — version, compatibility notes, and diagnostics.

## Implementation milestones

- [ ] Replace the ShadCube4 demo renderer with a stable Trophy Manager application shell.
- [ ] Add controller input and focus/navigation handling.
- [ ] Implement local trophy-data discovery and parsing after confirming the available PS4 APIs and file formats.
- [ ] Add read-only library and statistics views.
- [ ] Implement backup/export and safe restore with validation and confirmation.
- [ ] Add localization and test the package on supported firmware.

## Build and compatibility notes

The existing OpenOrbis build setup is retained for now. No compatibility with a particular firmware version is claimed until the application is built and tested on that firmware. Features must be marked implemented only after they have been verified on-console.
