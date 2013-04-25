#ifndef MODEL_CACHED_CLASS_H_
#define MODEL_CACHED_CLASS_H_

#include <map>

template<class T>
class CachedClass
{
public:
    typedef T value_type;

    CachedClass(const std::string &name, const T &initial_value = T())
                    : m_data(initial_value), m_name(name), m_version(0)
    {
    }

    const std::string &name() const
    {
        return m_name;
    }
    size_t version() const
    {
        return m_version;
    }
    const T &data() const
    {
        return m_data;
    }
    T &data_non_const()
    {
        return m_data;
    }
    T &modify_data()
    {
        ++m_version;
        return m_data;
    }
    void set_data(const T &data)
    {
        ++m_version;
        m_data = data;
    }
    void swap_data(T &data)
    {
        ++m_version;
        m_data.swap(data);
    }
    void clear()
    {
        ++m_version;
        m_data.clear();
    }

    template<class T2>
    bool is_up_to_date(const CachedClass<T2> &other) const
    {
        std::map<std::string, size_t>::const_iterator it = m_dependencies.find(other.name());
        if (it != m_dependencies.end()) return (it->second == other.version());
        return false;
    }
    template<class T2>
    void make_up_to_date(const CachedClass<T2> &other)
    {
        m_dependencies[other.name()] = other.version();
    }

public:
    T m_data;

private:
    std::string m_name;
    size_t m_version;
    std::map<std::string, size_t> m_dependencies;
};
#endif /* MODEL_CACHED_CLASS_H_ */
