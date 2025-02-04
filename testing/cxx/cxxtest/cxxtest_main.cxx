//***************************************************************************
// apps/testing/cxx/cxxtest/cxxtest_main.cxx
//
// SPDX-License-Identifier: Apache-2.0
//
// Licensed to the Apache Software Foundation (ASF) under one or more
// contributor license agreements.  See the NOTICE file distributed with
// this work for additional information regarding copyright ownership.  The
// ASF licenses this file to you under the Apache License, Version 2.0 (the
// "License"); you may not use this file except in compliance with the
// License.  You may obtain a copy of the License at
//
//   http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
// WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  See the
// License for the specific language governing permissions and limitations
// under the License.
//
//***************************************************************************

//***************************************************************************
// Included Files
//***************************************************************************

#include <nuttx/config.h>

#include <cstdio>
#include <cassert>
#include <cstdio>

#ifdef CONFIG_CXX_LOCALIZATION
#include <fstream>
#include <iostream>
#endif
#include <map>
#include <string>
#include <vector>
#ifdef CONFIG_CXX_EXCEPTION
#include <stdexcept>
#endif
#if defined(CONFIG_LIBCXX) && __cplusplus >= 201703L
#include <array>
#include <utility>
#endif

//***************************************************************************
// Private Classes
//***************************************************************************

class Base
{
public:
  virtual void printBase(void) {}
  virtual ~Base() {}
};

class Extend : public Base
{
public:
  void printExtend(void)
  {
    std::printf("extend\n");
  }
};

#if defined(CONFIG_LIBCXX) && __cplusplus >= 201703L
class File
{
public:
  static auto open(std::string_view path) noexcept -> std::optional<File>
  {
    auto fd = ::fopen(path.data(), "r");
    if (fd != nullptr)
      {
        return std::make_optional<File>(fd);
      }
    else
      {
        return std::nullopt;
      }
  }

  File(FILE* file) : file_(file) {}
  ~File()
  {
    if (file_ != nullptr)
      {
        ::fclose(file_);
      }
  }

private:
  FILE* file_ = nullptr;
};
#endif

//***************************************************************************
// Private Data
//***************************************************************************

//***************************************************************************
// Public Functions
//***************************************************************************

//***************************************************************************
// Name: test_ostream
//***************************************************************************/

#ifdef CONFIG_CXX_LOCALIZATION
static void test_ofstream()
{
  std::ofstream ttyOut;

  std::cout << "Test ofstream ================================" << std::endl;
  std::printf("printf: Starting test_ostream\n");
  ttyOut.open ("/dev/console");
  if (!ttyOut.good())
    {
      std::printf("printf: Failed opening /dev/console\n");
      std::cout << "cout: Failed opening /dev/console" << std::endl;
      std::cout << " good()=" << ttyOut.good();
      std::cout << " eof()=" << ttyOut.eof();
      std::cout << " fail()=" << ttyOut.fail();
      std::cout << " bad()=" << ttyOut.bad() << std::endl;
    }
  else
    {
      std::printf("printf: Successfully opened /dev/console\n");
      std::cout << "cout: Successfully opened /dev/console" << std::endl;
      ttyOut << "Writing this to /dev/console\n";
      ttyOut.close();
    }
}
#endif

//***************************************************************************
// Name: test_iostream
//***************************************************************************/

#ifdef CONFIG_CXX_LOCALIZATION
static void test_iostream()
{
  std::cout << "Test iostream ================================" << std::endl;
  std::cout << "Hello, this is only a test" << std::endl;
  std::cout << "Print an int: "  <<  190  <<  std::endl;
  std::cout << "Print a char: "  <<  'd'  <<  std::endl;
}
#endif

//***************************************************************************
// Name: test_stl
//***************************************************************************/

static void test_stl()
{
  std::printf("Test std::vector =============================\n");

  std::vector<int> v1;
  assert(v1.empty());

  v1.push_back(1);
  assert(!v1.empty());

  v1.push_back(2);
  v1.push_back(3);
  v1.push_back(4);
  assert(v1.size() == 4);

  v1.pop_back();
  assert(v1.size() == 3);

  std::printf("v1=%d %d %d\n", v1[0], v1[1], v1[2]);
  assert(v1[2] == 3);

  static const std::string s1 = "Hello, World!";
  std::printf("s1=%s\n", s1.c_str());
  assert(s1 == "Hello, World!");

  std::vector<int> v2 = v1;
  assert(v2 == v1);

  std::string words[4] = {"Hello", "World", "Good", "Luck"};
  std::vector<std::string> v3(words, words + 4);
  std::vector<std::string>::iterator it;
  for (it = v3.begin(); it != v3.end(); ++it)
    {
      std::printf("%s ", it->c_str());
    }

  std::printf("\n");
  assert(v3[1] == "World");

  std::printf("Test std::map ===============================\n");

  std::map<int, std::string> m1;
  m1[12] = "Hello";
  m1[24] = "World";
  assert(m1.size() == 2);
  assert(m1[24] == "World");
}

//***************************************************************************
// Name: test_stl2
//***************************************************************************/

#if defined(CONFIG_LIBCXX) && __cplusplus >= 201703L
auto test_stl2() -> void
{
  std::printf("Test C++17 features ==========================\n");

  auto check = [](auto&& path)
    {
      if (File::open(path))
        {
          std::printf("File %s exists!\n", path.data());
        }
      else
        {
          std::printf("File %s does not exist!\n", path.data());
        }
    };
  std::array<std::string_view, 3> path_list{
    "/proc/meminfo", "/invalid", "/proc/version"
  };

  std::for_each(path_list.cbegin(), path_list.cend(), check);
}
#endif

//***************************************************************************
// Name: test_rtti
//***************************************************************************/

#ifdef CONFIG_CXX_RTTI
static void test_rtti()
{
  printf("Test RTTI =============================\n");
  Base *a = new Base();
  Base *b = new Extend();
  assert(a);
  assert(b);

  Extend *t = dynamic_cast<Extend *>(a);
  assert(t == NULL);

  t = dynamic_cast<Extend *>(b);
  assert(t);
  t->printExtend();

  delete a;
  delete b;
}
#endif

//***************************************************************************
// Name: test_exception
//***************************************************************************/

#ifdef CONFIG_CXX_EXCEPTION
static void test_exception()
{
  std::printf("Test Exception ===============================\n");
  try
    {
      throw std::runtime_error("runtime error");
    }
  catch (std::runtime_error &e)
    {
      std::printf("Catch Exception: %s\n", e.what());
    }
}
#endif

//***************************************************************************
// Public Functions
//***************************************************************************

//***************************************************************************
// Name: cxxtest_main
//***************************************************************************/

extern "C" int main(int argc, char *argv[])
{
#ifdef CONFIG_CXX_LOCALIZATION
  test_ofstream();
  test_iostream();
#endif
  test_stl();
#if defined(CONFIG_LIBCXX) && __cplusplus >= 201703L
  test_stl2();
#endif
#ifdef CONFIG_CXX_RTTI
  test_rtti();
#endif
#ifdef CONFIG_CXX_EXCEPTION
  test_exception();
#endif

  return 0;
}
