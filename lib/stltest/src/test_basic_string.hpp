/*
Copyright (C) 2012 Mihai Vasilian

This program is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the
Free Software Foundation; either version 2 of the License, or (at your
option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along
with this program. If not, see http://www.gnu.org/licenses/.

contact: grayasm@gmail.com
*/



#ifndef __test_basic_string_hpp__
#define __test_basic_string_hpp__


//c++
//....

//CppUnit
#include <cppunit/extensions/HelperMacros.h>



class test_basic_string : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(test_basic_string);
    CPPUNIT_TEST(correctness);
    CPPUNIT_TEST(ctor);
    CPPUNIT_TEST(assign_op);
    CPPUNIT_TEST(size);
    CPPUNIT_TEST(resize);
    CPPUNIT_TEST(reserve);
    CPPUNIT_TEST(clear);
    CPPUNIT_TEST(append_op);
    CPPUNIT_TEST(append);
    CPPUNIT_TEST(push_back);
    CPPUNIT_TEST(assign);
    CPPUNIT_TEST(insert);
    CPPUNIT_TEST(erase);
    CPPUNIT_TEST(replace);
    CPPUNIT_TEST(copy);
    CPPUNIT_TEST(swap);
    CPPUNIT_TEST(find);
    CPPUNIT_TEST(rfind);
    CPPUNIT_TEST(find_first_of);
    CPPUNIT_TEST(find_last_of);
    CPPUNIT_TEST(find_first_not_of);
    CPPUNIT_TEST(find_last_not_of);
    CPPUNIT_TEST(substr);
    CPPUNIT_TEST(compare);
    CPPUNIT_TEST(perf1);    
    CPPUNIT_TEST_SUITE_END();


public:
    void setUp();
    void tearDown();

    void correctness(); // detect allowed types at compile time and trigger error
    void ctor();
    void assign_op();
    void size();    // + length + capacity
    void resize();
    void reserve();
    void clear();   // + empty
    void append_op();
    void append();
    void push_back();
    void assign();
    void insert();
    void erase();
    void replace();
    void copy();
    void swap();
    void find();
    void rfind();
    void find_first_of();
    void find_last_of();
    void find_first_not_of();
    void find_last_not_of();
    void substr();
    void compare();
    void perf1();       // performance test    
};



#endif//__test_basic_string_hpp__
