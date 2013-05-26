#include "indexedstring.h"

namespace IndexDb {

IndexedString::IndexedString()
    : m_uniqueId(0)
{
}

bool IndexedString::isValid() const
{
    return m_uniqueId != 0;
}

bool IndexedString::operator ==(const IndexedString &other) const
{
    return m_uniqueId == other.m_uniqueId;
}

bool IndexedString::operator !=(const IndexedString &other) const
{
    return m_uniqueId != other.m_uniqueId;
}

int IndexedString::uniqueId() const
{
    return m_uniqueId;
}

IndexedString::IndexedString(int uniqueId)
    : m_uniqueId(uniqueId)
{
}

} // namespace IndexDb
