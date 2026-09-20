#pragma once

#include <map>
#include <string>
#include <ostream>
#include <types.hpp>

class IReportFormatter
{
public:
    virtual ~IReportFormatter() = default;

    virtual void format(std::ostream &os, const std::map<std::string, xloc::types::FileStats> &stats) const = 0;
    /*
    Note: "= 0" above means this function is pure virtual. Meaning creating object is not allowed for this class.
    Every class that inherits this class must write the body of this function.
    */
};
