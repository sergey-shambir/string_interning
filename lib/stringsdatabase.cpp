#include "stringsdatabase.h"
#include <QMutexLocker>

namespace IndexDb {

StringsDatabase::StringsDatabase()
    : m_lastId(0)
{
#if USE_QT_DATA_STRUCTURES > 0
    m_texts << DbString();
#else
    m_texts.push_back(DbString());
#endif
}

StringsDatabase::~StringsDatabase()
{
}

IndexedString StringsDatabase::findOrInsertString(const DbString &text)
{
    QMutexLocker locker(&m_mutex);

    DbMap::iterator it = m_ids.find(text);
    if (it != m_ids.end()) {
#if USE_QT_DATA_STRUCTURES > 0
        return IndexedString(it.value());
#else
        return IndexedString(it->second);
#endif
    }

    ++m_lastId;
    m_ids[text] = m_lastId;
#if USE_QT_DATA_STRUCTURES > 0
    m_texts << text;
#else
    m_texts.push_back(text);
#endif
    return IndexedString(m_lastId);
}

DbString StringsDatabase::stringText(const IndexedString &string)
{
    QMutexLocker locker(&m_mutex);

    return m_texts[string.m_uniqueId];
}

int StringsDatabase::totalStringsCount() const
{
    return m_lastId;
}

int StringsDatabase::maxStringLength() const
{
    int maxLen = 0;
    for (const DbString &string : m_texts)
        maxLen = qMax<int>(maxLen, string.size());
    return maxLen;
}

int StringsDatabase::briefMemoryUsage() const
{
    int usage = 0;
    for (const DbString &string : m_texts)
        usage += string.capacity() * sizeof(DbString::value_type) + sizeof(DbString) + sizeof(int);

    return usage;
}

double StringsDatabase::averageStringLength() const
{
    qint64 totalLength = 0;
    for (const DbString &string : m_texts)
        totalLength += string.size();
    totalLength *= 10;
    totalLength = totalLength / m_lastId;
    return 0.1 * totalLength;
}

DbString StringsDatabase::stringAt(int index) const
{
    return m_texts[index - 1];
}

} // namespace IndexDb
