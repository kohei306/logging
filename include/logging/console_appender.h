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

#ifndef LOGGING_SRC_APPENDER_CONSOLE_APPENDER_H_
#define LOGGING_SRC_APPENDER_CONSOLE_APPENDER_H_

#include "appender_base.h"
#include "message_appender.h"
#include "default_format_policy_with_newline.h"

namespace logging {

class ConsoleAppender : public AppenderBase {
 public:
  using MessageAppenderHost = MessageAppender<DefaultFormatPolicyWithNewLine>;
  ConsoleAppender();
  explicit ConsoleAppender(std::unique_ptr<AppenderConfig> appender_config, bool is_closed = false);
  void Close() override;

 protected:
  void HookedDoSend(const LogEvent & log_event) final;
  void Flush() const { std::cout << std::flush; };

 private:
   MessageAppenderHost m_message_appender_host;
};

}  // namespace logging

#endif  // LOGGING_SRC_APPENDER_CONSOLE_APPENDER_H_
