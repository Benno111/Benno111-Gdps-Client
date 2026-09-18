#include "NetworkDebug.h"

#include "cocos2d.h"
#include "GDPSManager.h"
#include "Logger.h"
#include "hooking.h"
#include "cocos2dx/extensions/network/HttpClient.h"
#include "cocos2dx/extensions/network/HttpRequest.h"

#include <cstdio>
#include <ctime>
#include <dlfcn.h>
#include <fstream>
#include <string>

namespace
{
typedef void (*SendFunction)(cocos2d::extension::CCHttpClient *,
                             cocos2d::extension::CCHttpRequest *);
SendFunction sSend = 0;

const char *requestMethod(cocos2d::extension::CCHttpRequest::HttpRequestType type)
{
    using cocos2d::extension::CCHttpRequest;
    switch (type)
    {
    case CCHttpRequest::kHttpGet: return "GET";
    case CCHttpRequest::kHttpPost: return "POST";
    case CCHttpRequest::kHttpPut: return "PUT";
    case CCHttpRequest::kHttpDelete: return "DELETE";
    default: return "UNKNOWN";
    }
}

std::string urlWithoutQuery(const char *url)
{
    if (!url)
        return "<null>";

    const std::string value(url);
    const std::string::size_type query = value.find('?');
    return query == std::string::npos ? value : value.substr(0, query) + "?<redacted>";
}

void logRequest(cocos2d::extension::CCHttpRequest *request)
{
    if (!request)
    {
        LOGI("[NetworkDebug] request=<null>");
        return;
    }

    const char *method = requestMethod(request->getRequestType());
    const std::string url = urlWithoutQuery(request->getUrl());
    const int bodySize = request->getRequestDataSize();
    const unsigned headerCount = request->getHeaders().size();

    LOGI("[NetworkDebug] %s %s body_bytes=%d headers=%u",
         method, url.c_str(), bodySize, headerCount);

    std::ofstream output("/sdcard/Benno111GDPS/network-debug.log",
                         std::ios::out | std::ios::app);
    if (!output)
        return;

    output << static_cast<long>(time(0)) << ' ' << method << ' ' << url
           << " body_bytes=" << bodySize << " headers=" << headerCount << '\n';
}

void sendHook(cocos2d::extension::CCHttpClient *client,
              cocos2d::extension::CCHttpRequest *request)
{
    if (GDPS->networkDebug)
        logRequest(request);
    sSend(client, request);
}
} // namespace

void NetworkDebug::install(void *cocosHandle)
{
    if (!cocosHandle)
        return;

    void *send = dlsym(cocosHandle,
        "_ZN7cocos2d9extension12CCHttpClient4sendEPNS0_13CCHttpRequestE");
    if (!send)
    {
        LOGE("Unable to install networking debug hook: %s", dlerror());
        return;
    }

    HookManager::do_hook(send, reinterpret_cast<void *>(&sendHook),
                         reinterpret_cast<void **>(&sSend));
}
