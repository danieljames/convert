#include "./example.hpp"
#include <boost/convert.hpp>
#include <boost/convert/stream.hpp>
#include <boost/lexical_cast.hpp>

using std::string;

static void process_failure() {}
static void log(...) {}
static int fallback_function() { return -1; }

static
void
example1()
{
    boost::cnv::cstream  cnv;
    std::string const    str = "123";
    std::string const   str1 = "123";
    std::string const   str2 = "123";
    std::string const   str3 = "123";
    int const fallback_value = -1;

    {
        //[getting_serious_example1
        int i2 = boost::convert<int>("not an int", cnv).value_or(-1); // after the call i2 == -1

        if (i2 == -1) process_failure();
        //]
    }
    {
        //[getting_serious_example2
        try
        {
            int i1 = boost::lexical_cast<int>(str); // Throws if the conversion fails
            int i2 = boost::convert<int>(str, cnv).value(); // Throws if the conversion fails
        }
        catch (...)
        {
            process_failure();
        }
        //]
    }
    {
        //[getting_serious_example3
        boost::optional<int> r1 = boost::convert<int>(str1, cnv); // Does not throw on conversion failure
        boost::optional<int> r2 = boost::convert<int>(str2, cnv); // Does not throw on conversion failure
        // ...
        try // Delayed processing of potential exceptions
        {
            int i1 = r1.value(); // Will throw if conversion failed
            int i2 = r2.value(); // Will throw if conversion failed
        }
        catch (boost::bad_optional_access const&)
        {
            // Handle failed conversion
        }

        // Exceptions are avoided altogether
        int i1 = r1 ? r1.value() : fallback_value;
        int i2 = r2.value_or(fallback_value);
        int i3 = boost::convert<int>(str3, cnv).value_or(fallback_value);
        int i4 = boost::convert<int>(str3, cnv).value_or_eval(fallback_function);
        //]
    }
}

//[getting_serious_example5
struct fallback_func
{
    int operator()() const { log("Failed to convert"); return INT_MAX; }
};
//]

static
void
example4()
{
    boost::cnv::cstream  cnv;
    std::string const    str = "123";
    int const fallback_value = -1;
    //[getting_serious_example4
    boost::optional<int> res = boost::convert<int>(str, cnv);

    if (!res) log("str conversion failed");

    int i1 = res.value_or(fallback_value);

    // ...proceed
    //]
    //[getting_serious_example6
    int i2 = boost::convert<int>(str, cnv).value_or_eval(fallback_func()); // Fallback functor is called if failed
    int i3 = boost::convert<int>(str, cnv, fallback_func()); // Fallback functor is called if failed
    //]
}
static
void
example5()
{
    boost::cnv::cstream  cnv;
    std::string const    str = "123";
    int const fallback_value = -1;
    //[getting_serious_example7
    // Error-processing behavior are specified clearly and uniformly.
    // a) i1: Returning the provided fallback value;
    // b) i2: Calling the provided failure-processing function;
    // c) i3: Throwing an exception.

    int i1 = boost::convert<int>(str, cnv, fallback_value); // Fallback value used if failed
    int i2 = boost::convert<int>(str, cnv, fallback_func()); // Fallback functor is called if failed

    try
    {
        // Throwing behavior specified explicitly rather than implied
        int i3 = boost::convert<int>(str, cnv, boost::throw_on_failure);
    }
    catch (boost::bad_optional_access const&)
    {
        // Handle failed conversion
    }
    //]
}

void
example::getting_serious()
{
    example1();
    example4();
    example5();
}
