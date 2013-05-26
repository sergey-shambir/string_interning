#include <QDebug>
#include "lib/stringsdatabase.h"
#include <QDir>
#include <QTime>

typedef QList<IndexDb::IndexedString> ReferencesList;

static void visitFile(const QString &path, IndexDb::StringsDatabase &db, ReferencesList &refs);
static void visitDirectory(const QString &path, IndexDb::StringsDatabase &db, ReferencesList &refs);

void visitDirectory(const QString &path, IndexDb::StringsDatabase &db, ReferencesList &refs)
{
    QDir dir(path);
    if (!dir.exists())
        return;

    foreach (const QString &name, dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot))
        visitDirectory(dir.absoluteFilePath(name), db, refs);
    foreach (const QString &name, dir.entryList(QDir::Files | QDir::NoSymLinks))
        visitFile(dir.absoluteFilePath(name), db, refs);
}

/// Insert all identifers and file path to @a db.
void visitFile(const QString &path, IndexDb::StringsDatabase &db, ReferencesList &refs)
{
    if (!path.endsWith(QLatin1String(".cpp")) && !path.endsWith(QLatin1String(".h")))
        return;
    QFile file(path);
    if (!file.exists())
        return;
    file.open(QFile::ReadOnly);
    QString data = QString::fromUtf8(file.readAll());
    file.close();

    bool inIdentifier = false;
    QString identifier;
    for (int i = 0; i < data.size(); ++i) {
        if (data[i].isLetterOrNumber() || data[i] == QLatin1Char('_')) {
            if (inIdentifier) {
                identifier += data[i];
            } else {
                identifier.clear();
                identifier += data[i];
                inIdentifier = true;
            }
        } else if (inIdentifier) {
#if USE_QT_DATA_STRUCTURES > 0
            refs << db.findOrInsertString(identifier);
#else
            refs << db.findOrInsertString(identifier.toStdString());
#endif
            inIdentifier = false;
        }
    }

#if USE_QT_DATA_STRUCTURES > 0
    refs << db.findOrInsertString(path);
#else
    refs << db.findOrInsertString(path.toStdString());
#endif
}

int main(int argc, char *argv[])
{
    if (argc != 2) {
        qDebug() << "This is utility to test strings database creation perfomance.\n"
                    "It scans whole project and creates an index with all identifiers and numbers.\n"
                    "\n"
                    "One argument: path to directory with big C++ project.";
        return 0;
    }

    const QString projectPath = QString::fromUtf8(argv[1]);

    QTime time;
    time.start();

    {
        IndexDb::StringsDatabase db;
        ReferencesList refs;
        visitDirectory(projectPath, db, refs);
        qDebug() << "Access count:" << refs.size() << "strings";
        qDebug() << "Indexed strings count:" << db.totalStringsCount();
        qDebug() << "Maximum string length:" << db.maxStringLength() << "chars";
        qDebug() << "Average string length:" << db.averageStringLength() << "chars";
        qDebug() << "Brief memory usage:" << db.briefMemoryUsage() << "bytes";
        qDebug() << "Time elapsed (reading & db creation):" << (0.001 * time.elapsed()) << "seconds";

        {
            qDebug() << "\n\nMeasuring find & insertion time by copying data to other StringDatabase...";
            QTime timer;
            timer.start();
            IndexDb::StringsDatabase db2;
            foreach (const IndexDb::IndexedString &s, refs)
                db2.findOrInsertString(db.stringText(s));
            qDebug() << "Elapsed time:" << (0.001 * timer.elapsed()) << "milliseconds.";
            qDebug() << "Access count:" << refs.size() << "strings";

            ReferencesList empty;
            refs.swap(empty);
        }
    }

    return 0;
}

