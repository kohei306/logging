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
#include "gmock/gmock.h"
#include "logging/logger.h"
#include "logging/log_manager.h"
#include "logging/appender_interface.h"
#include "logging/log_event.h"


TEST(LoggerTest, Constructor) {
 std::string logger_name("LoggerA");
 logging::Logger& logger_a (logging::LogManager::GetInstance().GetLogger(logger_name));
 ASSERT_EQ(logger_a.GetName(), logger_name);
 ASSERT_EQ(logger_a.GetLogLevel(), logging::LogLevel::NOT_SELECTED);
 ASSERT_EQ(logger_a.GetUseDefaultAppender(), true);
 ASSERT_EQ(logger_a.IsVerboseEnabled(), true);  // Specification.1 default level (NOT_SELECTED) is log level lower than Verbose.
 ASSERT_EQ(logger_a.IsDebugEnabled(), true);
 ASSERT_EQ(logger_a.IsInfoEnabled(), true);
 ASSERT_EQ(logger_a.IsWarnEnabled(), true);
 ASSERT_EQ(logger_a.IsErrorEnabled(), true);
 ASSERT_EQ(logger_a.IsFatalEnabled(), true);
 ASSERT_EQ(logger_a.GetUseDefaultAppender(), true); // Specification.2 By default, logger use default appender.
 ASSERT_EQ(logger_a.GetNumAppenders(), 0);

 logging::LogManager::GetInstance().Shutdown();
 ASSERT_EQ(logging::LogManager::GetInstance().GetNumDefaultAppenders(), 0);
 ASSERT_EQ(logging::LogManager::GetInstance().GetNumLoggers(), 0);
}

TEST(LoggerTest, Setter) {
 std::string logger_name("LoggerA");
 logging::Logger& logger (logging::LogManager::GetInstance().GetLogger(logger_name));
 ASSERT_EQ(logger.GetName(), logger_name);

 ASSERT_EQ(logger.GetLogLevel(), logging::LogLevel::NOT_SELECTED);
 logger.SetLogLevel(logging::LogLevel::DEBUG);
 ASSERT_EQ(logger.GetLogLevel(), logging::LogLevel::DEBUG);


 std::string logger_a_name("LoggerA");
 logging::Logger& logger_2 (logging::LogManager::GetInstance().GetLogger(logger_a_name));
 ASSERT_EQ(logging::LogManager::GetInstance().GetNumLoggers(), 1);
 ASSERT_EQ(logging::LogManager::GetInstance().IsLoggerExists(logger_a_name), true);

 ASSERT_EQ(logger_2.GetLogLevel(), logging::LogLevel::DEBUG);

 logging::LogManager::GetInstance().Shutdown();
 ASSERT_EQ(logging::LogManager::GetInstance().GetNumDefaultAppenders(), 0);
 ASSERT_EQ(logging::LogManager::GetInstance().GetNumLoggers(), 0);
}

TEST(LoggerTest, AddRemoveAppender) {
 std::string logger_name("LoggerA");
 logging::Logger& logger (logging::LogManager::GetInstance().GetLogger(logger_name));
 ASSERT_EQ(logger.GetName(), logger_name);

 ASSERT_EQ(logger.GetNumAppenders(), 0);

 std::unique_ptr<logging::AppenderConfig> appender_config_console =
		 std::make_unique<logging::AppenderConfig>(logging::AppenderType::CONSOLE, "LoggerA_ConsoleAppender");
 std::unique_ptr<logging::AppenderConfig> appender_config_console_clone =
		 std::make_unique<logging::AppenderConfig>(logging::AppenderType::CONSOLE, "LoggerA_ConsoleAppender");
 std::unique_ptr<logging::AppenderConfig> appender_config_console_clone_clone =
		 std::make_unique<logging::AppenderConfig>(logging::AppenderType::CONSOLE, "LoggerA_ConsoleAppender");
 std::unique_ptr<logging::AppenderConfig> appender_config_file =
		 std::make_unique<logging::FileAppenderConfig>(logging::AppenderType::FILE, "LoggerA_FileAppender");
 std::unique_ptr<logging::AppenderConfig> appender_config_file_clone =
		 std::make_unique<logging::FileAppenderConfig>(logging::AppenderType::FILE, "LoggerA_FileAppender");

 ASSERT_EQ(logger.AddAppender(std::move(appender_config_console)), logging::AppenderAddableError::NO_ERROR);
 ASSERT_EQ(logger.GetNumAppenders(), 1);
 ASSERT_EQ(logger.AppenderExist("LoggerA_ConsoleAppender"), true);
 ASSERT_EQ(logger.AddAppender(std::move(appender_config_console_clone)), logging::AppenderAddableError::APPENDER_EXIST);
 ASSERT_EQ(logger.GetNumAppenders(), 1);

 ASSERT_EQ(logger.AddAppender(std::move(appender_config_file)), logging::AppenderAddableError::NO_ERROR);
 ASSERT_EQ(logger.GetNumAppenders(), 2);
 ASSERT_EQ(logger.AppenderExist("LoggerA_FileAppender"), true);
 ASSERT_EQ(logger.AddAppender(std::move(appender_config_file_clone)), logging::AppenderAddableError::APPENDER_EXIST);
 ASSERT_EQ(logger.GetNumAppenders(), 2);

 logger.RemoveAppender("LoggerA_ConsoleAppender");
 ASSERT_EQ(logger.GetNumAppenders(), 1);
 ASSERT_EQ(logger.AppenderExist("LoggerA_ConsoleAppender"), false);

 ASSERT_EQ(logger.AddAppender(std::move(appender_config_console_clone_clone)), logging::AppenderAddableError::NO_ERROR);
 ASSERT_EQ(logger.GetNumAppenders(), 2);

 logger.RemoveAllAppenders();
 ASSERT_EQ(logger.GetNumAppenders(), 0);

 logging::LogManager::GetInstance().Shutdown();
 ASSERT_EQ(logging::LogManager::GetInstance().GetNumDefaultAppenders(), 0);
 ASSERT_EQ(logging::LogManager::GetInstance().GetNumLoggers(), 0);
}