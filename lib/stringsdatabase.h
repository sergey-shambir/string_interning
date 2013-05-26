#ifndef INDEXDB_STRINGSDATABASE_H
#define INDEXDB_STRINGSDATABASE_H

#include "indexedstring.h"
#include <QtGlobal>
#include <QMutex>

#if USE_QT_DATA_STRUCTURES > 0
#include <QHash>
#include <QString>
#include <QStringList>

typedef QString DbString;
typedef QStringList DbStringList;
typedef QHash<QString, int> DbMap;

#else
#include <unordered_map>
#include <string>
#include <vector>

typedef std::string DbString;
typedef std::vector<std::string> DbStringList;
typedef std::unordered_map<std::string, int> DbMap;
#endif

namespace IndexDb {

class StringsDatabase
{
    Q_DISABLE_COPY(StringsDatabase)
public:
    StringsDatabase();
    ~StringsDatabase();

    IndexedString findOrInsertString(const DbString &text);

    /// @return cached text for valid indexed string and empty text for invalid.
    DbString stringText(const IndexedString &string);

    /// Debug interface
    int totalStringsCount() const;
    int maxStringLength() const;
    int briefMemoryUsage() const;
    double averageStringLength() const;
    DbString stringAt(int index) const;

private:
    QMutex m_mutex;
    DbMap m_ids;
    DbStringList m_texts;
    int m_lastId;
};

} // namespace IndexDb

#endif // INDEXDB_STRINGSDATABASE_H
