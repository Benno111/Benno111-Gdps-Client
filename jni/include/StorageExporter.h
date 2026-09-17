#pragma once

class GJGameLevel;

namespace StorageExporter
{
    // Creates the public export folders and installs handlers for fatal signals.
    void initialize();

    // Writes a standalone copy of a level to external storage.
    bool backupLevel(const GJGameLevel *level);
}
