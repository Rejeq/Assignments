#include "LogBase.h"

#include <spdlog/sinks/stdout_color_sinks.h>

using namespace Log;

const char* s_defaultPattern = "[%H:%M:%S.%e] [%n] [%^%l%$] %v";

bool LogBase::SetDefault() {
  bool err = false;

  err = SetDefaultSinks();
  SetDefaultPattern();
  SetDefaultLevel();
  return err;
}

bool LogBase::SetDefaultSinks() {
  bool err = false;
  err |= AddSink<spdlog::sinks::stdout_color_sink_mt>();
  return err;
}

void LogBase::SetDefaultPattern() {
  p_logger.set_pattern(s_defaultPattern);
}

const char* LogBase::GetCurrentPattern() {
  return s_defaultPattern;
}

void LogBase::SetLevel(LogBase::Level level) {
  p_logger.set_level((spdlog::level::level_enum)level);
}

void LogBase::SetDefaultLevel() {
  SetLevel(GetDefaultLevel());
}

LogBase::Level LogBase::GetDefaultLevel() {
  return Level::Info;
}

LogBase::Level LogBase::GetCurrentLevel() {
  return (Level)p_logger.level();
}

spdlog::logger* LogBase::GetLogger() {
  return &p_logger;
}
