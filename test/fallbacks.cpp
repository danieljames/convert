// Boost.Convert test and usage example
// Copyright (c) 2009-2014 Vladimir Batov.
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. See http://www.boost.org/LICENSE_1_0.txt.

#include "./test.hpp"
#include <boost/function.hpp>

namespace { namespace local
{
    bool    called_functor_int;
    bool called_functor_double;
    bool    called_functor_foo;
    bool   called_function_int;
    bool  called_function_long;
}}

struct    functor_int { int    operator() () const { local::   called_functor_int = true; return INT_MAX; }};
struct functor_double { double operator() () const { local::called_functor_double = true; return INT_MAX; }};
struct    functor_foo { int       func (int) const { local::   called_functor_foo = true; return INT_MAX; }};

int   function_int () { local:: called_function_int = true; return INT_MAX; }
long function_long () { local::called_function_long = true; return INT_MAX; }

static
void
fallback_function()
{
    boost::cnv::cstream cnv;
    functor_foo         foo;

    int i01 = boost::convert<int>("uhm", cnv).value_or_eval(functor_int());
    int i02 = boost::convert<int>("uhm", cnv).value_or_eval(functor_double());
    int i03 = boost::convert<int>("uhm", cnv).value_or_eval(boost::bind(&functor_foo::func, foo, 0));
    int i04 = boost::convert<int>("uhm", cnv).value_or_eval(function_int);
    int i05 = boost::convert<int>("uhm", cnv).value_or_eval(function_long);

    BOOST_TEST(local::   called_functor_int && i01 == INT_MAX);
    BOOST_TEST(local::called_functor_double && i02 == INT_MAX);
    BOOST_TEST(local::   called_functor_foo && i03 == INT_MAX);
    BOOST_TEST(local::  called_function_int && i04 == INT_MAX);
    BOOST_TEST(local:: called_function_long && i05 == INT_MAX);

    local::   called_functor_int = false;
    local::called_functor_double = false;
    local::   called_functor_foo = false;
    local::  called_function_int = false;
    local:: called_function_long = false;

    int i11 = boost::convert<int>("uhm", cnv, functor_int());
    int i12 = boost::convert<int>("uhm", cnv, functor_double());
    int i13 = boost::convert<int>("uhm", cnv, boost::bind(&functor_foo::func, foo, 0));
    int i14 = boost::convert<int>("uhm", cnv, function_int);
    int i15 = boost::convert<int>("uhm", cnv, function_long);

    BOOST_TEST(local::   called_functor_int && i01 == INT_MAX);
    BOOST_TEST(local::called_functor_double && i02 == INT_MAX);
    BOOST_TEST(local::   called_functor_foo && i03 == INT_MAX);
    BOOST_TEST(local::  called_function_int && i04 == INT_MAX);
    BOOST_TEST(local:: called_function_long && i05 == INT_MAX);

    try
    {
        int i16 = boost::convert<int>("uhm", cnv, boost::throw_on_failure);
        BOOST_TEST(0);
    }
    catch (boost::bad_optional_access const&)
    {
    }
}

void
test::cnv::fallbacks()
{
    fallback_function();
}
