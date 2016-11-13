#ifndef GLOBALS_H
#define GLOBALS_H

#include <sstream>

const std::string NORGTAGLIBNAME = "NoRGTagLib";
const std::string NORGTAGREADGROUPNAME = "NoRGTagGroup";

template <typename Iter>
std::string join(Iter begin, Iter end, std::string const& separator)
{
  std::ostringstream result;
  if (begin != end)
    result << *begin++;
  while (begin != end)
    result << separator << *begin++;
  return result.str();
}

#endif // GLOBALS_H
