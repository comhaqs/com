#ifndef SOCI_BOOST_POSIX_TIME_PTIME_H_INCLUDED
#define SOCI_BOOST_POSIX_TIME_PTIME_H_INCLUDED

#include <soci/type-conversion-traits.h>
// boost
#include <boost/date_time/posix_time/ptime.hpp>
// std
#include <ctime>

namespace soci
{

template<>
struct type_conversion<boost::posix_time::ptime>
{
    typedef std::tm base_type;

    static void from_base(
        base_type const & in, indicator ind, boost::posix_time::ptime & out)
    {
        if (ind == i_null)
        {
            throw soci_error("Null value not allowed for this type");
        }

        out = boost::posix_time::ptime(boost::gregorian::date(in.tm_year + 1900, in.tm_mon + 1, in.tm_mday), 
            boost::posix_time::time_duration(in.tm_hour, in.tm_min, in.tm_sec));
    }

    static void to_base(
        boost::posix_time::ptime const & in, base_type & out, indicator & ind)
    {
        if(in.is_not_a_date_time()){
            ind = i_null;
        }else{
            auto d = in.date();
            auto t = in.time_of_day();
            out.tm_year = d.year() - 1900;
            out.tm_mon = d.month() - 1;
            out.tm_mday = d.day();
            out.tm_hour = t.hours();
            out.tm_min = t.minutes();
            out.tm_sec = t.seconds();
            ind = i_ok;
        }
    }
};

} // namespace soci

#endif // SOCI_BOOST_POSIX_TIME_PTIME_H_INCLUDED
