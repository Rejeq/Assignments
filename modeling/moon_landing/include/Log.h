#pragma once

#include "LogBase.h"

// User must implement this
namespace Log {

// TODO: Add support for shared libraries
LogBase& Get();
LogBase* GetPtr();
LogBase* Create();
LogBase* CreateDefault();

}  // namespace Log

#define _LOGGER ::Log::Get()
#define LOG_CRITICAL(...) LOGBASE_C(_LOGGER, __VA_ARGS__)
#define LOG_ERROR(...) LOGBASE_E(_LOGGER, __VA_ARGS__)
#define LOG_WARN(...) LOGBASE_W(_LOGGER, __VA_ARGS__)
#define LOG_INFO(...) LOGBASE_I(_LOGGER, __VA_ARGS__)
#define LOG_DEBUG(...) LOGBASE_D(_LOGGER, __VA_ARGS__)
#define LOG_TRACE(...) LOGBASE_T(_LOGGER, __VA_ARGS__)

