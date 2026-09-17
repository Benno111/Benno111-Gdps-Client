#include "StorageExporter.h"

#include "GJGameLevel.h"

#include <csignal>
#include <cstdio>
#include <cstring>
#include <fcntl.h>
#include <fstream>
#include <sys/stat.h>
#include <sys/time.h>
#include <unistd.h>

namespace
{
const char *const kExportRoot = "/sdcard/Benno111GDPS";
const char *const kBackupDirectory = "/sdcard/Benno111GDPS/LevelBackups";
const char *const kCrashDirectory = "/sdcard/Benno111GDPS/Crashes";

volatile sig_atomic_t sHandlingCrash = 0;

void createExportDirectories()
{
    // EEXIST is expected after the first launch. Any other error is handled by
    // the later file open, so startup should never fail because storage is not
    // currently mounted or permission has not yet been granted.
    mkdir(kExportRoot, 0775);
    mkdir(kBackupDirectory, 0775);
    mkdir(kCrashDirectory, 0775);
}

void appendText(char *buffer, size_t capacity, size_t &length, const char *text)
{
    while (*text && length + 1 < capacity)
        buffer[length++] = *text++;
}

void appendUnsigned(char *buffer, size_t capacity, size_t &length, unsigned long value, unsigned base)
{
    char digits[2 * sizeof(value) + 1];
    size_t count = 0;
    do
    {
        const unsigned digit = value % base;
        digits[count++] = static_cast<char>(digit < 10 ? '0' + digit : 'a' + digit - 10);
        value /= base;
    } while (value && count < sizeof(digits));

    while (count && length + 1 < capacity)
        buffer[length++] = digits[--count];
}

const char *signalName(int signalNumber)
{
    switch (signalNumber)
    {
    case SIGABRT: return "SIGABRT";
    case SIGBUS: return "SIGBUS";
    case SIGFPE: return "SIGFPE";
    case SIGILL: return "SIGILL";
    case SIGSEGV: return "SIGSEGV";
    default: return "UNKNOWN";
    }
}

void crashHandler(int signalNumber, siginfo_t *info, void *)
{
    if (sHandlingCrash)
        _exit(128 + signalNumber);
    sHandlingCrash = 1;

    char report[512];
    size_t length = 0;
    appendText(report, sizeof(report), length, "Benno111 GDPS native crash report\nsignal: ");
    appendText(report, sizeof(report), length, signalName(signalNumber));
    appendText(report, sizeof(report), length, " (");
    appendUnsigned(report, sizeof(report), length, static_cast<unsigned long>(signalNumber), 10);
    appendText(report, sizeof(report), length, ")\nprocess: ");
    appendUnsigned(report, sizeof(report), length, static_cast<unsigned long>(getpid()), 10);
    appendText(report, sizeof(report), length, "\nfault_address: 0x");
    appendUnsigned(report, sizeof(report), length,
                   reinterpret_cast<unsigned long>(info ? info->si_addr : 0), 16);
    appendText(report, sizeof(report), length,
               "\n\nThe Android system tombstone/logcat may contain a full native stack trace.\n");

    const int file = open("/sdcard/Benno111GDPS/Crashes/latest-crash.txt",
                          O_WRONLY | O_CREAT | O_TRUNC, 0664);
    if (file >= 0)
    {
        // Ignore a short write: there is no safe recovery path inside a fatal
        // signal handler, and preserving even part of the report is useful.
        write(file, report, length);
        close(file);
    }

    signal(signalNumber, SIG_DFL);
    raise(signalNumber);
    _exit(128 + signalNumber);
}

void installCrashHandler(int signalNumber)
{
    struct sigaction action;
    memset(&action, 0, sizeof(action));
    sigemptyset(&action.sa_mask);
    action.sa_sigaction = crashHandler;
    action.sa_flags = SA_SIGINFO | SA_RESETHAND;
    sigaction(signalNumber, &action, 0);
}

std::string safeFilename(const std::string &name)
{
    std::string result;
    for (std::string::const_iterator it = name.begin(); it != name.end() && result.size() < 48; ++it)
    {
        const unsigned char character = static_cast<unsigned char>(*it);
        result += ((character >= 'a' && character <= 'z') ||
                   (character >= 'A' && character <= 'Z') ||
                   (character >= '0' && character <= '9') || character == '-' || character == '_')
                      ? static_cast<char>(character)
                      : '_';
    }
    return result.empty() ? "unnamed" : result;
}
} // namespace

void StorageExporter::initialize()
{
    createExportDirectories();
    installCrashHandler(SIGABRT);
    installCrashHandler(SIGBUS);
    installCrashHandler(SIGFPE);
    installCrashHandler(SIGILL);
    installCrashHandler(SIGSEGV);
}

bool StorageExporter::backupLevel(const GJGameLevel *level)
{
    if (!level)
        return false;

    createExportDirectories();
    struct timeval now;
    gettimeofday(&now, 0);

    char path[384];
    snprintf(path, sizeof(path), "%s/%d-%s-%ld-%ld.gmd", kBackupDirectory,
             level->levelID, safeFilename(level->levelName).c_str(),
             static_cast<long>(now.tv_sec), static_cast<long>(now.tv_usec));

    std::ofstream output(path, std::ios::out | std::ios::binary | std::ios::trunc);
    if (!output)
        return false;

    output << "Benno111 GDPS Level Backup\n"
           << "format=1\n"
           << "level_id=" << level->levelID << "\n"
           << "level_name=" << level->levelName << "\n"
           << "level_data_length=" << level->levelString.size() << "\n"
           << "level_data_begin\n";
    output.write(level->levelString.data(), level->levelString.size());
    output << "\nlevel_data_end\n";
    output.close();
    return output.good();
}
