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

#ifndef LOGGING_INCLUDE_LOGGING_LOG_LEVEL_H_
#define LOGGING_INCLUDE_LOGGING_LOG_LEVEL_H_

#include <string>
#include <cstdint>

namespace logging {

enum class LogLevel : uint8_t {
  FATAL = 1,
  ERROR = 2,
  WARN = 3,
  INFO = 4,
  DEBUG = 5,
  VERBOSE = 6,
  OFF = 7,
  NOT_SELECTED = 8
};

const inline std::string LogLevellToString(LogLevel log_level) {
  std::string result = "Default";
  if (log_level == LogLevel::FATAL) result = "FATAL";
  else if (log_level == LogLevel::ERROR) result = "ERROR";
  else if (log_level == LogLevel::WARN) result = "WARN";
  else if (log_level == LogLevel::INFO) result = "INFO";
  else if (log_level == LogLevel::DEBUG) result = "DEBUG";
  else if (log_level == LogLevel::VERBOSE) result = "VERBOSE";
  else if (log_level == LogLevel::OFF) result = "OFF";
  else if (log_level == LogLevel::NOT_SELECTED) result = "NOT_SELECTED";
  else throw std::invalid_argument("Invalid log level.");
  return result;
}

const inline LogLevel LogLevellFromString(const std::string log_level) {
  LogLevel result = LogLevel::NOT_SELECTED;
  if (log_level == "FATAL") result = LogLevel::FATAL;
  else if (log_level == "ERROR") result = LogLevel::ERROR;
  else if (log_level == "WARN") result =  LogLevel::WARN;
  else if (log_level == "INFO") result = LogLevel::INFO;
  else if (log_level == "DEBUG") result = LogLevel::DEBUG;
  else if (log_level == "VERBOSE") result = LogLevel::VERBOSE;
  else if (log_level == "OFF") result = LogLevel::OFF;
  else if (log_level == "NOT_SELECTED") result = LogLevel::NOT_SELECTED;
  else throw std::invalid_argument("Invalid log level");
  return result;
}

}  // namespace logging

#endif /* LOGGING_INCLUDE_LOGGING_LOG_LEVEL_H_ */
