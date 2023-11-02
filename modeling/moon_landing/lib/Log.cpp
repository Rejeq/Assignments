#include "Log.h"

#include <memory>

using namespace Log;

static std::unique_ptr<LogBase> s_logger;

LogBase& Log::Get() {
  assert(s_logger != nullptr && "Logger is not created");
  return *s_logger;
}

LogBase* Log::GetPtr() {
  return s_logger.get();
}

LogBase* Log::Create() {
  assert(s_logger == nullptr && "Logger is already defined");

  s_logger = std::make_unique<LogBase>("Moon");
  return s_logger.get();
}

LogBase* Log::CreateDefault() {
  if (Create() == nullptr)
    return nullptr;

  if (s_logger->SetDefault())
    return nullptr;

  return s_logger.get();
}
