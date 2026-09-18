#pragma once

#include <string>

class GJGameLevel;

namespace StorageExporter
{
    // Creates the public export folders and installs handlers for fatal signals.
    void initialize();

    // Writes a standalone copy of a level to external storage.
    bool backupLevel(const GJGameLevel *level);

    // Copies a regular file from the game's private writable directory to the
    // public GameFiles export directory. Paths and symlinks are deliberately
    // rejected so this cannot be used to read files the game does not own.
    bool copyGameFile(const std::string &fileName);

    // Copies the two Geometry Dash save files and returns the number copied.
    unsigned copySaveFiles();

    // A crash report is kept pending until the player chooses to export it.
    bool hasPendingCrashReport();
    bool exportCrashReport();
}
