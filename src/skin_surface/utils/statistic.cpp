#include <utils/statistic.h>
#include <cstdio>

void Statistic::set(const std::string &name_, const std::string &value_, const std::string &unit_)
{
    name = name_;
    value = value_;
    unit = unit_;
}
void Statistic::set(const std::string &name_, size_t value_, const std::string &unit_)
{
    char val[80];
    snprintf(val, 80, "%lu", value_);
    set(name_, val, unit_);
}
void Statistic::set(const std::string &name_, int32_t value_, const std::string &unit_)
{
    char val[80];
    snprintf(val, 80, "%d", value_);
    set(name_, val, unit_);
}
void Statistic::set(const std::string &name_, double value_, const std::string &unit_)
{
    char val[80];
    snprintf(val, 80, "%f", value_);
    set(name_, val, unit_);
}
