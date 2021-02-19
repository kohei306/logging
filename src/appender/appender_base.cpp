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

#include "appender_base.h"

namespace logging {

AppenderBase::AppenderBase()
   : m_is_closed(false) {
}

AppenderBase::AppenderBase(std::unique_ptr<AppenderConfig> appender_config, bool is_closed) : m_appender_config (std::move(appender_config)), m_is_closed(is_closed) {

}

void AppenderBase::Send(const LogEvent & log_event) {
   /////////////////////////
   // TODO Add pre filtering/formatting.
   /////////////////////////
   if (m_appender_config->m_level == LogLevel::NOT_SELECTED || m_appender_config->m_level == LogLevel::OFF) {
     return;
   }
   if (m_appender_config->m_level >= log_event.m_log_level) {
     HookedDoSend(log_event);
   }
}

} // namespace logging



