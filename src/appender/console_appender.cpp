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
#include "console_appender.h"

namespace logging {

ConsoleAppender::ConsoleAppender() : AppenderBase::AppenderBase(std::make_unique<AppenderConfig>(AppenderType::CONSOLE, "ConcoleAppender")), m_message_appender_host() {
}

ConsoleAppender::ConsoleAppender(std::unique_ptr<AppenderConfig> appender_config, bool is_closed) :
AppenderBase::AppenderBase(std::move(appender_config), is_closed){
  m_message_appender_host.AddHeader(std::cout);
}

void ConsoleAppender::Close() {
  m_is_closed = true;
}

void ConsoleAppender::HookedDoSend(const LogEvent & log_event) {
  m_message_appender_host.SendMessage(std::cout, log_event);
  Flush();
}

}  // namespace logging
