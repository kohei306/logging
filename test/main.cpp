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

#include <gtest/gtest.h>

int main(int argc, char* argv[])
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

//int main(int argc, char ** argv) {
//	logging::Logger& m_logger (logging::LogManager::GetInstance().GetLogger("SubModuleA_Logger"));
//	LOG_FATAL(m_logger, "Example Fatal level message");
//	LOG_ERROR(m_logger, "Example Error level message");
//	LOG_WARN(m_logger, "Example Warn level message");
//	LOG_INFO(m_logger, "Example Info level message");
//	LOG_DEBUG(m_logger, "Example Debug level message");
//	LOG_VERBOSE(m_logger, "Example Verbose level message");
//
//	logging::LogManager::GetInstance().InitFromLogConfigFile("./example_logging_config.txt");
//	logging::LogManager::GetInstance().PrintSummaryOfLogConfig();
//
//	logging::Logger& x_logger (logging::LogManager::GetInstance().GetLogger("SubModuleD_Logger"));
//	LOG_FATAL(x_logger, "Example Fatal level message");
//	LOG_ERROR(x_logger, "Example Error level message");
//	LOG_WARN(x_logger, "Example Warn level message");
//	LOG_INFO(x_logger, "Example Info level message");
//	LOG_DEBUG(x_logger, "Example Debug level message");
//	LOG_VERBOSE(x_logger, "Example Verbose level message");
//  return 0;
//}
//Logger::Logger(const std::string name, LogLevel level, bool use_default_appender)
//  : m_name(name), m_level(level), m_use_default_appender(use_default_appender), m_mtx() {
//}
//
//LoggerRef LoggerMap::GetLogger(const std::string& name) {
//  std::lock_guard<std::recursive_mutex> lock(m_rmtx);
//  LoggerPtrMap::iterator it = m_loggers->find(name);
//  if (it != m_loggers->end()) {
//    return *(it->second.get());
//  } else {
//    m_loggers->insert(LoggerPtrMap::value_type(name, std::unique_ptr<Logger>(new Logger(name, LogLevel::NOT_SELECTED))));
//    return GetLogger(name);
//  }
//}
