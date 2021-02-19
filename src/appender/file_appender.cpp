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
#include <stdio.h>
#include <time.h>
#include <mutex>
#include "file_appender.h"

namespace logging {

std::ofstream::openmode LogModeStrToOpenMode(const std::string & log_mode_str) {
  if (log_mode_str == "BINARY") return std::ofstream::binary;
  else if (log_mode_str == "ATEND") return std::ofstream::ate;
  else if (log_mode_str == "APPEND") return std::ofstream::app;
  else if (log_mode_str == "TRUNCATE") return std::ofstream::trunc;
  else throw std::invalid_argument("Invalid log mode.")
}

std::string GetTimeStampedFileName(const std::string & file_name_prefix) {
  std::string file_name;
  time_t rawtime;
  struct tm * timeinfo;
  char buffer [80];
  time (&rawtime);
  timeinfo = localtime(&rawtime);
  strftime (buffer, 80, "%y-%m-%d-%H-%M-%S", timeinfo);
  file_name = file_name_prefix + std::string(buffer);
  return file_name;
}

FileAppender::FileAppender(std::unique_ptr<AppenderConfig> appender_config, bool is_closed) :
AppenderBase::AppenderBase(std::move(appender_config), is_closed) {
  FileAppenderConfig * file_appender_config = dynamic_cast<FileAppenderConfig*>(m_appender_config.get());
  std::string file_name;
  if (file_appender_config->m_add_timestamp_to_file_name == "TRUE") {
    file_name = GetTimeStampedFileName(file_appender_config->m_file_name_prefix);
  } else if (file_appender_config->m_add_timestamp_to_file_name == "FALSE"){
	file_name = file_appender_config->m_file_name_prefix;
  } else {
	file_name = file_appender_config->m_file_name_prefix;
  }

  std::string file_name_with_path;

  if (file_appender_config->m_output_file_path.back() != '/') {
    file_name_with_path = file_appender_config->m_output_file_path + '/' + file_name + ".txt";
  } else {
	file_name_with_path = file_appender_config->m_output_file_path + file_name + ".txt";
  }
  std::unique_lock<std::mutex> lock(m_mtx);
  try {
     m_ofs.open(file_name_with_path, std::ofstream::out | LogModeStrToOpenMode(file_appender_config->m_open_mode));
   }
   catch(std::exception& error) {
	 std::cout << "Exception: " << error.what() << std::endl;
     try{ m_ofs.close(); } catch(...){}
   }
   m_message_appender_host.AddHeader(m_ofs);
   lock.unlock();
}

void FileAppender::Close() {
  try{ m_ofs.close(); } catch(...){}
}

void FileAppender::HookedDoSend(const LogEvent & log_event) {
  std::lock_guard<std::mutex> lock(m_mtx);
  m_message_appender_host.SendMessage(m_ofs, log_event);
  Flush();
}

}  // namespace logging




