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
#include "logging/logger.h"
#include "logging/log_manager.h"

TEST(LogManagerTest, GetLogger) {
 std::string logger_a_name("LoggerA");
 ASSERT_EQ(logging::LogManager::GetInstance().GetNumLoggers(), 0);
 ASSERT_EQ(logging::LogManager::GetInstance().IsLoggerExists(logger_a_name), false);
 logging::Logger& logger_a (logging::LogManager::GetInstance().GetLogger(logger_a_name));
 ASSERT_EQ(logging::LogManager::GetInstance().GetNumLoggers(), 1);
 ASSERT_EQ(logging::LogManager::GetInstance().IsLoggerExists(logger_a_name), true);
 ASSERT_EQ(logger_a.GetName(), "LoggerA");
 ASSERT_EQ(logger_a.GetLogLevel(), logging::LogLevel::NOT_SELECTED);
 ASSERT_EQ(logger_a.GetUseDefaultAppender(), true);
 ASSERT_EQ(logger_a.GetNumAppenders(), 0);
 logging::LogManager::GetInstance().Shutdown();
 ASSERT_EQ(logging::LogManager::GetInstance().GetNumLoggers(), 0);
 ASSERT_EQ(logging::LogManager::GetInstance().IsLoggerExists(logger_a_name), false);
}

TEST(LogManagerTest, GetLogger_MultipleTimes) {
 std::string logger_a_name("LoggerA");
 std::string logger_b_name("LoggerB");
 std::string logger_c_name("LoggerC");
 ASSERT_EQ(logging::LogManager::GetInstance().GetNumLoggers(), 0);
 ASSERT_EQ(logging::LogManager::GetInstance().IsLoggerExists(logger_a_name), false);
 logging::Logger& logger_a (logging::LogManager::GetInstance().GetLogger(logger_a_name));
 ASSERT_EQ(logging::LogManager::GetInstance().IsLoggerExists(logger_a_name), true);
 ASSERT_EQ(logging::LogManager::GetInstance().GetNumLoggers(), 1);
 logging::Logger& logger_b (logging::LogManager::GetInstance().GetLogger(logger_b_name));
 ASSERT_EQ(logging::LogManager::GetInstance().IsLoggerExists(logger_b_name), true);
 ASSERT_EQ(logging::LogManager::GetInstance().GetNumLoggers(), 2);
 logging::Logger& logger_c (logging::LogManager::GetInstance().GetLogger(logger_c_name));
 ASSERT_EQ(logging::LogManager::GetInstance().IsLoggerExists(logger_c_name), true);
 ASSERT_EQ(logging::LogManager::GetInstance().GetNumLoggers(), 3);
 ASSERT_EQ(logging::LogManager::GetInstance().IsLoggerExists(logger_c_name), true);
 ASSERT_EQ(logger_a.GetName(), "LoggerA");
 ASSERT_EQ(logger_a.GetLogLevel(), logging::LogLevel::NOT_SELECTED);
 ASSERT_EQ(logger_a.GetUseDefaultAppender(), true);
 ASSERT_EQ(logger_b.GetName(), "LoggerB");
 ASSERT_EQ(logger_b.GetLogLevel(), logging::LogLevel::NOT_SELECTED);
 ASSERT_EQ(logger_b.GetUseDefaultAppender(), true);
 ASSERT_EQ(logger_c.GetName(), "LoggerC");
 ASSERT_EQ(logger_c.GetLogLevel(), logging::LogLevel::NOT_SELECTED);
 ASSERT_EQ(logger_c.GetUseDefaultAppender(), true);
 logging::LogManager::GetInstance().Shutdown();
 ASSERT_EQ(logging::LogManager::GetInstance().GetNumLoggers(), 0);
 ASSERT_EQ(logging::LogManager::GetInstance().IsLoggerExists(logger_a_name), false);
 ASSERT_EQ(logging::LogManager::GetInstance().IsLoggerExists(logger_b_name), false);
 ASSERT_EQ(logging::LogManager::GetInstance().IsLoggerExists(logger_c_name), false);
}

TEST(LogManagerTest, AddRemoveAppender) {
  std::unique_ptr<logging::AppenderConfig> appender_config_console = std::make_unique<logging::AppenderConfig>(logging::AppenderType::CONSOLE, "DefaultConsoleAppender");
  std::unique_ptr<logging::AppenderConfig> appender_config_file = std::make_unique<logging::FileAppenderConfig>(logging::AppenderType::FILE, "DefaultFileAppender");
//  std::unique_ptr<logging::AppenderConfig> appender_config_aralog = std::make_unique<logging::AraLogAppenderConfig>(logging::AppenderType::ARALOG, "AraLogAppenderDefaultName", "DFT", "Default App description", "ARA_CONSOLE", "/tmp/");
//  std::unique_ptr<logging::AppenderConfig> appender_config_syslog = std::make_unique<logging::AppenderConfig>(logging::AppenderType::SYSLOG, "DefaultSYSLOGAppender");
//  std::unique_ptr<logging::AppenderConfig> appender_config_net = std::make_unique<logging::AppenderConfig>(logging::AppenderType::NET, "DefaultNETAppender");

  ASSERT_EQ(logging::LogManager::GetInstance().GetNumDefaultAppenders(), 0);
  ASSERT_EQ(logging::LogManager::GetInstance().AddDefaultAppender(std::move(appender_config_console)), logging::AppenderAddableError::NO_ERROR);
  std::unique_ptr<logging::AppenderConfig> appender_config_console_clone = std::make_unique<logging::AppenderConfig>(logging::AppenderType::CONSOLE, "DefaultConsoleAppender");
  ASSERT_EQ(logging::LogManager::GetInstance().IsDefaultAppenderExist("DefaultConsoleAppender"), true);
  ASSERT_EQ(logging::LogManager::GetInstance().GetNumDefaultAppenders(), 1);
  ASSERT_EQ(logging::LogManager::GetInstance().AddDefaultAppender(std::move(appender_config_console_clone)), logging::AppenderAddableError::APPENDER_EXIST);
  ASSERT_EQ(logging::LogManager::GetInstance().GetNumDefaultAppenders(), 1);

  ASSERT_EQ(logging::LogManager::GetInstance().AddDefaultAppender(std::move(appender_config_file)), logging::AppenderAddableError::NO_ERROR);
  std::unique_ptr<logging::AppenderConfig> appender_config_file_clone = std::make_unique<logging::FileAppenderConfig>(logging::AppenderType::FILE, "DefaultFileAppender");
  ASSERT_EQ(logging::LogManager::GetInstance().IsDefaultAppenderExist("DefaultFileAppender"), true);
  ASSERT_EQ(logging::LogManager::GetInstance().GetNumDefaultAppenders(), 2);
  ASSERT_EQ(logging::LogManager::GetInstance().AddDefaultAppender(std::move(appender_config_file_clone)), logging::AppenderAddableError::APPENDER_EXIST);
  ASSERT_EQ(logging::LogManager::GetInstance().GetNumDefaultAppenders(), 2);


  logging::LogManager::GetInstance().RemoveDefaultAppender("DefaultConsoleAppender");
  ASSERT_EQ(logging::LogManager::GetInstance().GetNumDefaultAppenders(), 1);
  ASSERT_EQ(logging::LogManager::GetInstance().IsDefaultAppenderExist("DefaultConsoleAppender"), false);

  std::unique_ptr<logging::AppenderConfig> appender_config_console_2 = std::make_unique<logging::AppenderConfig>(logging::AppenderType::CONSOLE, "DefaultConsoleAppender2");
  ASSERT_EQ(logging::LogManager::GetInstance().AddDefaultAppender(std::move(appender_config_console_2)), logging::AppenderAddableError::NO_ERROR);
  ASSERT_EQ(logging::LogManager::GetInstance().GetNumDefaultAppenders(), 2);
  logging::LogManager::GetInstance().RemoveAllDefaultAppenders();
  ASSERT_EQ(logging::LogManager::GetInstance().GetNumDefaultAppenders(), 0);

// !!! Need Ara::log installed. !!!
//  logging::LogManager::GetInstance().AddDefaultAppender(std::move(appender_config_aralog));
//  std::unique_ptr<logging::AppenderConfig> appender_config_aralog_clone = std::make_unique<logging::AraLogAppenderConfig>(logging::AppenderType::ARALOG, "AraLogAppenderDefaultName", "DFT", "Default App description", "ARA_CONSOLE", "/tmp/");
//  std::cout << "82" << std::endl;
//  ASSERT_EQ(logging::LogManager::GetInstance().AppenderExist(appender_config_aralog_clone), true);
//  ASSERT_EQ(logging::LogManager::GetInstance().GetNumDefaultAppenders(), 3);

// !!! Not implemented yet. !!!
//  logging::LogManager::GetInstance().AddDefaultAppender(std::move(appender_config_syslog));
//  std::unique_ptr<logging::AppenderConfig> appender_config_syslog_clone = std::make_unique<logging::AppenderConfig>(logging::AppenderType::SYSLOG, "DefaultSYSLOGAppender");
//  ASSERT_EQ(logging::LogManager::GetInstance().AppenderExist(appender_config_syslog_clone), true);
//  ASSERT_EQ(logging::LogManager::GetInstance().GetNumDefaultAppenders(), 3);
// !!! Not implemented yet. !!!
//  logging::LogManager::GetInstance().AddDefaultAppender(std::move(appender_config_net));
//  std::unique_ptr<logging::AppenderConfig> appender_config_net_clone = std::make_unique<logging::AppenderConfig>(logging::AppenderType::NET, "DefaultNETAppender");
//  ASSERT_EQ(logging::LogManager::GetInstance().AppenderExist(appender_config_net_clone), true);
//  ASSERT_EQ(logging::LogManager::GetInstance().GetNumDefaultAppenders(), 4);

  logging::LogManager::GetInstance().Shutdown();
  ASSERT_EQ(logging::LogManager::GetInstance().GetNumDefaultAppenders(), 0);
  ASSERT_EQ(logging::LogManager::GetInstance().GetNumLoggers(), 0);
  ASSERT_EQ(logging::LogManager::GetInstance().IsDefaultAppenderExist("DefaultConsoleAppender"), false);
  ASSERT_EQ(logging::LogManager::GetInstance().IsDefaultAppenderExist("DefaultFileAppender"), false);
//  ASSERT_EQ(logging::LogManager::GetInstance().AppenderExist(appender_config_aralog_clone), false);
//  ASSERT_EQ(logging::LogManager::GetInstance().AppenderExist(appender_config_syslog_clone), false);
//  ASSERT_EQ(logging::LogManager::GetInstance().AppenderExist(appender_config_net_clone), false);
}

TEST(LogManagerTest, InitFromLogConfigFile) {
  std::string logger_a_name("SubModuleA_Logger");
  logging::Logger& logger_a (logging::LogManager::GetInstance().GetLogger(logger_a_name));
  LOG_FATAL(logger_a, "Example Fatal level message");
  LOG_ERROR(logger_a, "Example Error level message");
  LOG_WARN(logger_a, "Example Warn level message");
  LOG_INFO(logger_a, "Example Info level message");
  LOG_DEBUG(logger_a, "Example Debug level message");
  LOG_VERBOSE(logger_a, "Example Verbose level message");

  logging::LogManager::GetInstance().InitFromLogConfigFile("./example_logging_config.txt");
  logging::LogManager::GetInstance().PrintSummaryOfLogConfig();

  std::string logger_d_name("SubModuleD_Logger");
  logging::Logger& x_logger (logging::LogManager::GetInstance().GetLogger(logger_d_name));
  LOG_FATAL(x_logger, "Example Fatal level message");
  LOG_ERROR(x_logger, "Example Error level message");
  LOG_WARN(x_logger, "Example Warn level message");
  LOG_INFO(x_logger, "Example Info level message");
  LOG_DEBUG(x_logger, "Example Debug level message");
  LOG_VERBOSE(x_logger, "Example Verbose level message");
  logging::LogManager::GetInstance().Shutdown();
  ASSERT_EQ(logging::LogManager::GetInstance().GetNumDefaultAppenders(), 0);
  ASSERT_EQ(logging::LogManager::GetInstance().GetNumLoggers(), 0);
}

