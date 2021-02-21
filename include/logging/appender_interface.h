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

#ifndef INCLUDE_LOGGING_APPENDER_INTERFACE_H_
#define INCLUDE_LOGGING_APPENDER_INTERFACE_H_

#include <string>
#include <memory>
#include <vector>

#include "log_event.h"

namespace logging {

class IAppender;

using AppenderUnqPtr = std::unique_ptr<IAppender>;
using AppenderRawPtr = IAppender*;
using AppenderList = std::vector<AppenderUnqPtr>;

class IAppender {
 public:
  virtual ~IAppender() = default;

  virtual void Send(const LogEvent & log_event) = 0;

  virtual AppenderType GetAppenderType() = 0;

  virtual std::string GetAppenderName() = 0;

  virtual LogLevel GetAppenderLogLevel() = 0;

  virtual const AppenderConfig& GetAppenderConfig() const = 0;

  virtual void SetAppenderConfig(std::unique_ptr<AppenderConfig> appender_config) = 0;

  virtual void Close() = 0;
};

}  // namespace logging


#endif  // INCLUDE_LOGGING_APPENDER_INTERFACE_H_
