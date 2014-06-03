// Copyright (c) 2009-2014 Vladimir Batov.
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. See http://www.boost.org/LICENSE_1_0.txt.

#ifndef BOOST_CONVERT_PRINTF_CONVERTER_HPP
#define BOOST_CONVERT_PRINTF_CONVERTER_HPP

#include <boost/convert/base.hpp>
#include <stdio.h>

namespace boost 
{
    struct printf_converter;
}

struct boost::printf_converter : public boost::converter_base
{
    typedef boost::printf_converter this_type;
    typedef boost::converter_base   base_type;

    CONVERT_FUNC_SET_BASE;
    CONVERT_FUNC_SET_PRECISION;
    CONVERT_FUNC_SET_UPPERCASE;

    template<typename TypeIn>
    typename boost::disable_if<conversion::is_any_string<TypeIn>, bool>::type
    operator()(TypeIn const& value_in, std::string& result_out) const
    {
        int const    bufsz = 256;
        char     buf[bufsz];
        int       num_chars = snprintf(buf, bufsz, format(pos<TypeIn>()), value_in);
        bool        success = num_chars < bufsz;

        if (success) result_out = buf;

        return success;
    }
    template<typename TypeOut>
    typename boost::disable_if<conversion::is_any_string<TypeOut>, bool>::type
    operator()(std::string const& value_in, TypeOut& result_out) const
    {
        return this_type::operator()(value_in.c_str(), result_out);
    }
    template<typename TypeOut>
    typename boost::disable_if<conversion::is_any_string<TypeOut>, bool>::type
    operator()(char const* value_in, TypeOut& result_out) const
    {
        int num_read = sscanf(value_in, format(pos<TypeOut>()), &result_out);

        return num_read == 1;
    }

    private:

    template<typename Type> int pos() const
    {
        typedef boost::mpl::vector<double,
                                   int, unsigned int,
                                   short int, unsigned short int,
                                   long int, unsigned long int
                                   > managed_types;

        typedef typename boost::mpl::find<managed_types, Type>::type type_iterator;
        typedef typename type_iterator::pos                               type_pos;

        return type_pos::value;
    }

    char const* format(int pos) const
    {
        char const* d_format[] = { "%f", "%d", "%u", "%hd", "%hu", "%ld", "%lu" }; // Must match managed_types
        char const* x_format[] = { "%f", "%x", "%x", "%hx", "%hx", "%lx", "%lx" }; // Must match managed_types
        char const* o_format[] = { "%f", "%o", "%o", "%ho", "%ho", "%lo", "%lo" }; // Must match managed_types
        char const*     format = base_ == 10 ? d_format[pos]
                               : base_ == 16 ? x_format[pos]
                               : base_ ==  8 ? o_format[pos]
                               : (BOOST_ASSERT(0), (char const*) 0);

        return format;
    }
};

#endif // BOOST_CONVERT_PRINTF_CONVERTER_HPP