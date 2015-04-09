#include "sdregex.h"

#if SDU_WITH_BOOST

#include <boost/xpressive/xpressive.hpp>

namespace SGDP
{



    BOOL SDRegexMatch(const CHAR *str , SMatchResult & result, const CHAR * pattern)
    {
        boost::xpressive::cregex rex = boost::xpressive::cregex::compile( pattern);
        boost::xpressive:: cmatch what;

        bool ret = boost::xpressive::regex_match(str,what, rex);
        for (boost::xpressive::cmatch::iterator itr = what.begin(); itr != what.end(); ++itr)
        {
            result.push_back(itr->str());
        }
        return ret;
    }

    BOOL SDRegexMatch(const std::string &str ,SMatchResult & result,  const std::string & pattern)
    {
        boost::xpressive::sregex rex = boost::xpressive::sregex::compile( pattern);
        boost::xpressive:: smatch what;
        bool ret = boost::xpressive::regex_match(str,what, rex);
        for (boost::xpressive::smatch::iterator itr = what.begin(); itr != what.end(); ++itr)
        {
            result.push_back(itr->str());
        }
        return ret;
    }

    std::string SDRegexReplace(const std::string &  str , const std::string &  pattern ,const std::string &  format)
    {
        boost::xpressive::sregex rex = boost::xpressive::sregex::compile( pattern);
        return  boost::xpressive::regex_replace( str, rex, format );
    }


    std::string SDRegexReplace(const CHAR * str , const CHAR * pattern ,const CHAR * format)
    {
        boost::xpressive::cregex rex = boost::xpressive::cregex::compile( pattern);
        return  boost::xpressive::regex_replace( str, rex, format );
    }


}
#endif  // SDU_WITH_BOOST


