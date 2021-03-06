#include "./test.hpp"

static
bool
my_cypher(std::string const& value_in, boost::optional<std::string>& value_out)
{
    size_t const cypher = 'A' - '1';

    value_out = value_in;

    for (std::string::iterator it = value_out->begin(); it != value_out->end(); ++it)
        (*it < 'A') ? (*it += cypher) : (*it -= cypher);

    return true;
}

void
test::cnv::encryption()
{
    ////////////////////////////////////////////////////////////////////////////
    // Testing custom converter.
    ////////////////////////////////////////////////////////////////////////////

    std::string encrypted = boost::convert<std::string>("ABC", my_cypher).value();
    std::string decrypted = boost::convert<std::string>(encrypted, my_cypher).value();

    BOOST_TEST(encrypted == "123");
    BOOST_TEST(decrypted == "ABC");
}
