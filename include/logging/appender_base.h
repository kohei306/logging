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

#ifndef LOGGING_INCLUDE_LOGGING_APPENDER_BASE_H_
#define LOGGING_INCLUDE_LOGGING_APPENDER_BASE_H_

#include <atomic>
#include <string>
#include <memory>
#include "appender_interface.h"
#include "appender_config.h"

namespace logging {

class AppenderBase : public IAppender {
 public:
  AppenderBase();
  explicit AppenderBase(std::unique_ptr<AppenderConfig> appender_config, bool is_closed = false);

  void Send(const LogEvent & log_event) override;

  const AppenderConfig& GetAppenderConfig() const override {return *m_appender_config.get();};

  void SetAppenderConfig(std::unique_ptr<AppenderConfig> appender_config) override { m_appender_config = std::move(appender_config);};

  AppenderType GetAppenderType() override { return m_appender_config->m_appender_type; }

  std::string GetAppenderName() override { return m_appender_config->m_name; }

  LogLevel GetAppenderLogLevel() { return m_appender_config->m_level; };

 protected:
    std::unique_ptr<AppenderConfig> m_appender_config;
    std::atomic<bool> m_is_closed;
    virtual void HookedDoSend(const LogEvent & log_event) = 0;
};

}  // namespace logging

#endif  // LOGGING_INCLUDE_LOGGING_APPENDER_BASE_H_
