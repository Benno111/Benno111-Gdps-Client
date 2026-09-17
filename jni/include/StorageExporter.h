#pragma once

class GJGameLevel;

namespace StorageExporter
{
    // Creates the public export folders and installs handlers for fatal signals.
    void initialize();

    // Writes a standalone copy of a level to external storage.
    bool backupLevel(const GJGameLevel *level);

    // A crash report is kept pending until the player chooses to export it.
    bool hasPendingCrashReport();
    bool exportCrashReport();
}
