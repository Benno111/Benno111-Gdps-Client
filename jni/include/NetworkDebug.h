#pragma once

namespace NetworkDebug
{
    // Installs the HTTP request hook. Logging remains dormant until enabled.
    void install(void *cocosHandle);
}
