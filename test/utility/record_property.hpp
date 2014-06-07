// Copyright (c) Benjamin Kietzman (github.com/bkietz)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <gtest/gtest.h>
#include <sstream>

class record_property
{
  typedef std::stringstream stream;
  stream value;
  const char *name;
public:
  record_property(const char *p)
    : name(p)
  {
  }
  template<typename T>
  std::ostream& operator<< (T t)
  {
    return value << t;
  }
  ~record_property()
  {
    ::testing::Test::RecordProperty(name, value.str().c_str());
  }
};
