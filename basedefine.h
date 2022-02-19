#ifndef BASEDEFINE_H
#define BASEDEFINE_H

#include <QString>
#include <QDateTime>
#include <QVariant>

struct NamedData {
    QVariant        data;
    int             priority;

    NamedData() : priority(0) {}
};

struct StockData {
    QString         barcode;
    QString         number;
};

typedef QList<NamedData> NamedDataList;

struct ColumnDetail
{
    QString     name;
    int         width;
};

enum COMPARE_METHOD
{
    COMPARE_METHOD_NOTCARE = 0,
    COMPARE_METHOD_GREATER,
    COMPARE_METHOD_GREATER_EQUAL,
    COMPARE_METHOD_EQUAL,
    COMPARE_METHOD_LESS,
    COMPARE_METHOD_LESS_EQUAL
};

enum FILTER_STATE
{
    FILTER_STATE_NOTCARE = 0,
    FILTER_STATE_YES,
    FILTER_STATE_NO
};

struct ComparingMethod
{
    int             target;
    COMPARE_METHOD  method;
};

#endif // BASEDEFINE_H
