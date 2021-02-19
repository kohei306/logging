// MIT License

// Copyright (c) 2018 Kohei Otsuka

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include <iostream>
#include <assert.h>
#include "aralog_appender.h"

namespace logging {

std::atomic<bool> AraLogAppender::m_is_aralog_initialized {false};

AraLogAppender::AraLogAppender() {
  if (!m_is_aralog_initialized) {
    // TODO Make it configurable
    //ara_log::InitLogging("TMP", "TEMP", ara_log::LogLevel::kDebug, ara_log::LogMode::kFile, "/home/user/log/");
    m_is_aralog_initialized = true;
  }
}

ara_log::LogMode LogModeStrToEnum(const std::string & log_mode_str) {
  if (log_mode_str == "CONSOLE") return ara_log::LogMode::kConsole;
  if (log_mode_str == "FILE") return ara_log::LogMode::kFile;
  if (log_mode_str == "REMOTE") return ara_log::LogMode::kRemote;
  return ara_log::LogMode::kConsole;
}

AraLogAppender::AraLogAppender(std::unique_ptr<AppenderConfig> appender_config, bool is_closed) :
AppenderBase::AppenderBase(std::move(appender_config), is_closed) {
  if (!m_is_aralog_initialized) {
    // TODO Make it configurable
	AraLogAppenderConfig * ara_log_appender_config = dynamic_cast<AraLogAppenderConfig*>(m_appender_config.get());
    ara_log::InitLogging(ara_log_appender_config->m_app_id, ara_log_appender_config->m_app_description,
    ara_log::LogLevel::kVerbose, LogModeStrToEnum(ara_log_appender_config->m_log_mode), ara_log_appender_config->m_directory_path);
    m_is_aralog_initialized = true;
  }
  AddLogHeader();
}

void AraLogAppender::Close() {
  m_is_closed = true;
}

// Currently using temporary stream object Guaranteed scoped access if required to ensure thread
// safety which enables to log out messages concurrently and have them processed in one piece
// Costs of constructor/destructor depends on their content and is implementation detail of the Logging framework
// - Costs of trivial constructor/destructor (e.g. empty ones)
// is cheap, actually instantiating an object in C++ equals of instantiating a struct in C
void AraLogAppender::HookedDoSend(const LogEvent & log_event) {
  switch (log_event.log_level_) {
    case(LogLevel::FATAL):
	  m_message_appender_host.SendMessage(ara_log::LogFatal(), log_event);
      break;
    case(LogLevel::ERROR):
	  m_message_appender_host.SendMessage(ara_log::LogError() , log_event);
      break;
    case(LogLevel::WARN):
	  m_message_appender_host.SendMessage(ara_log::LogWarn() , log_event);
      break;
    case(LogLevel::INFO):
	  m_message_appender_host.SendMessage(ara_log::LogInfo() , log_event);
      break;
    case(LogLevel::DEBUG):
	  m_message_appender_host.SendMessage(ara_log::LogDebug() , log_event);
      break;
    case(LogLevel::VERBOSE):
	  m_message_appender_host.SendMessage(ara_log::LogVerbose() , log_event);
      break;
    default:
      break;
  }
}

}  // namespace logging





