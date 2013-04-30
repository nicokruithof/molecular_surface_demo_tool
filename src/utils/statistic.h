#ifndef UTILS_STATISTIC_H_
#define UTILS_STATISTIC_H_

#include <string>
#include <boost/integer.hpp>

struct Statistic {
    void set(const std::string &name_, const std::string &value_, const std::string &unit_ = "");
    void set(const std::string &name_, size_t value_, const std::string &unit_ = "");
    void set(const std::string &name_, int32_t value_, const std::string &unit_ = "");
    void set(const std::string &name_, double value_, const std::string &unit_ = "");

    std::string category;
    std::string name;
    std::string value;
    std::string unit;
};

#endif /* MODEL_MODEL_H_ */
