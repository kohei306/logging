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

#ifndef LOGGING_INCLUDE_LOGGING_MESSAGE_APPENDER_H_
#define LOGGING_INCLUDE_LOGGING_MESSAGE_APPENDER_H_


#include <string>
#include <utility>

#include "log_event.h"
#include "log_header.h"

#ifdef	ARALOG
#include <ara/logging.hpp>
#endif

namespace logging {

template <class FormatPolicy, class LogHeader = DefaultLogHeader>
class MessageAppender: public FormatPolicy, LogHeader
{
  public:
	template <class OutPutChannel>
	void SendMessage(OutPutChannel && output_channel, const LogEvent & event) {
		std::string message {FormatPolicy::FormatMessage(event)};
		std::forward<OutPutChannel>(output_channel) << message;
	};

#ifdef	ARALOG
	// Ara::log overload
	void SendMessage(ara_log::LogStream && output_channel, const LogEvent & event) {
		std::string message {FormatPolicy::FormatMessage(event)};
		std::forward<ara_log::LogStream>(output_channel) << message;
	};
#endif

	template <class OutPutChannel>
	void AddHeader(OutPutChannel && output_channel) {
		std::string header {LogHeader::GetHeader()};
		std::forward<OutPutChannel>(output_channel) << header;
	};
};

}  // namespace logging


#endif /* LOGGING_INCLUDE_LOGGING_MESSAGE_APPENDER_H_ */
