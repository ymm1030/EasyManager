#ifndef MYQUICKDATA_H
#define MYQUICKDATA_H

#include <QObject>
#include "basedefine.h"

class Filter
{
public:
    FILTER_STATE    taxfree;
    QString         brand;
    QString         tbId;
    QString         name;
    FILTER_STATE    controlledValue;
    ComparingMethod actualValue;
    FILTER_STATE    freeShipping;
    FILTER_STATE    luluHas;
    QString         barcode;
    ComparingMethod currentStock;
    FILTER_STATE    hasSpAppend;

    Filter()
        : taxfree(FILTER_STATE_NOTCARE)
        , controlledValue(FILTER_STATE_NOTCARE)
        , freeShipping(FILTER_STATE_NOTCARE)
        , luluHas(FILTER_STATE_NOTCARE)
        , hasSpAppend(FILTER_STATE_NOTCARE)
    {
        actualValue.target = 0;
        actualValue.method = COMPARE_METHOD_NOTCARE;

        currentStock.target = 0;
        currentStock.method = COMPARE_METHOD_NOTCARE;
    }

    bool isValid() const
    {
        if (taxfree == FILTER_STATE_NOTCARE &&
            brand.isEmpty() &&
            tbId.isEmpty() &&
            name.isEmpty() &&
            controlledValue == FILTER_STATE_NOTCARE &&
            actualValue.method == COMPARE_METHOD_NOTCARE &&
            freeShipping == FILTER_STATE_NOTCARE &&
            luluHas == FILTER_STATE_NOTCARE &&
            barcode.isEmpty() &&
            currentStock.method == COMPARE_METHOD_NOTCARE &&
            hasSpAppend == FILTER_STATE_NOTCARE) {
            return false;
        }
        return true;
    }
};

class QFile;
class PrivateMyQuickData;
class MyQuickData : public QObject
{
    Q_OBJECT
public:
    MyQuickData(QObject* parent = nullptr);
    ~MyQuickData();

    static MyQuickData* instance() { return s_instance; }

    const QList<NamedDataList>& allData() const;
    QList<NamedDataList> filteredData(const Filter& filter) const;
    QList<NamedDataList> quickFilteredData(const QString& content) const;
    const NamedDataList& lastRecord() const;
    QList<NamedDataList> priceChangedData() const;
    QList<NamedDataList> shippingChangedData() const;
    QList<NamedDataList> withoutBarcodeData() const;

    QList<ColumnDetail> columnDetails() const;
    const QList<int>& columnWidth() const;
    QStringList brands();

    bool open();
    void save();
    void add(const NamedDataList& data);
    bool drop(const NamedDataList& data);
    void update(const NamedDataList& oldData, const NamedDataList& newData);
    void insert(const NamedDataList& current, const NamedDataList& newData);
    void moveBefore(const NamedDataList& source, const NamedDataList& target);
    void moveAfter(const NamedDataList& source, const NamedDataList& target);

    bool exist(const QString& name) const;

    void readStockData();

    const QMap<QString, QString>& getLuluPrice() const;
    void updateLuluPrice(const QMap<QString, QString>& prices, const QList<NamedDataList>& data, const QMap<QString, QString>& changedShippingMap);
    const QMap<QString, QString>& getYesterdayLuluPrice() const;

    void setStockComparingDays(int days);

public slots:
    void renameData(const QString& oldName, const QString& newName);

signals:
    void dataChanged();
    void renameSuccessful(const QString& oldName, const QString& newName);

private:
    void loadAllData(QFile* fi);
    void loadStock();
    void loadLuluPrice();
    QString findStock(const QString& barcode);
    bool checkValid(const NamedDataList& data);
    bool checkDuplicate(const NamedDataList& data);
    void compleData(NamedDataList& data);
    void saveUnchangedPriceMap();

private:
    PrivateMyQuickData*       m_data;
    static MyQuickData*       s_instance;
};

#endif // MYQUICKDATA_H
