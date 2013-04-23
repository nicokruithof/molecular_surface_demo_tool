#ifndef MODEL_CACHED_CLASS_H_
#define MODEL_CACHED_CLASS_H_

#include <map>

template <class T>
class CachedClass {
public:
    typedef T value_type;

    CachedClass(const std::string &name, const T &initial_value = T())
    : m_data(initial_value)
    , m_name(name)
    , m_version(0)
    {
    }

    void clear() {
        ++m_version;
        m_data.clear();
    }
    const T &data() const {
        return m_data;
    }
    void set_data(const T &data) {
        m_data = data;
    }
    template <class T2>
    bool is_up_to_date(const CachedClass<T2> &other) const {
        std::map<std::string, size_t>::const_iterator it = other.m_dependencies.find(m_name);
        if (it != other.end())
            return (it->second == m_version);
        return false;
    }
    template <class T2>
    void make_up_to_date(CachedClass<T2> &other) const {
        other.m_dependencies[m_name] = m_version;
    }

public:
    T m_data;

private:
    std::string m_name;
    size_t m_version;
    std::map<std::string, size_t> m_dependencies;
};
#endif /* MODEL_CACHED_CLASS_H_ */
