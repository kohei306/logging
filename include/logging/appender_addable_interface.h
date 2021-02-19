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

#ifndef LOGGING_INCLUDE_LOGGING_APPENDER_ADDABLE_INTERFACE_H_
#define LOGGING_INCLUDE_LOGGING_APPENDER_ADDABLE_INTERFACE_H_

#include <vector>
#include <string>
#include <memory>
#include "appender_interface.h"

namespace logging {

enum class AppenderAddableError {
  NO_ERROR = 0,
  APPENDER_EXIST = 1,
  ADDING_NULL_APPENDER = 2
};

class IAppenderAddable {
   public:
    virtual AppenderAddableError AddAppender(std::unique_ptr<AppenderConfig> new_appender_config) = 0;

    virtual AppenderList & GetAllAppenders() = 0;


    virtual AppenderRawPtr GetAppender(const std::string& name) const = 0;

    virtual bool AppenderExist(const std::string & appender_name) = 0;

    virtual void RemoveAllAppenders() = 0;

    virtual void RemoveAppender(const std::string& name) = 0;

    virtual ~IAppenderAddable() = default;

   private:
    virtual AppenderAddableError AddAppender(AppenderUnqPtr new_appender) = 0;
};

}  // namespace logging



#endif /* LOGGING_INCLUDE_LOGGING_APPENDER_ADDABLE_INTERFACE_H_ */
