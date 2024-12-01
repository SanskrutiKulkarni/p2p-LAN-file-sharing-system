#ifndef UTIL_H
#define UTIL_H

#include <QString>
#include <QDir>
#include <QVector>
#include <QPair>

class Util
{
public:
    static QString sizeToString(qint64 size);
    static QVector< QPair<QString, QString> >
        getInnerDirNameAndFullFilePath(const QDir& startingDir, const QString& innerDirName);

    static QString parseAppVersion(bool onlyVerNum = true);

    static QString getUniqueFileName(const QString& fileName, const QString& folderPath);
};

#endif // UTIL_H
