#ifndef INDEXDB_INDEXEDSTRING_H
#define INDEXDB_INDEXEDSTRING_H

namespace IndexDb {

class StringsDatabase;

/// Copyable string, unique in single index database.
class IndexedString
{
public:
    IndexedString();

    bool isValid() const;
    bool operator ==(const IndexedString &other) const;
    bool operator !=(const IndexedString &other) const;

    int uniqueId() const;

private:
    friend class StringsDatabase;

    IndexedString(int uniqueId);
    int m_uniqueId;
};

} // namespace IndexDb

#endif // INDEXDB_INDEXEDSTRING_H
