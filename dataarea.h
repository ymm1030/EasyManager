#ifndef DATAAREA_H
#define DATAAREA_H

#include <QWidget>
#include "basedefine.h"

class PrivateDataArea;
class DataArea : public QWidget
{
    Q_OBJECT
public:
    explicit DataArea(QWidget *parent = nullptr);
    ~DataArea();
    void save();
    void readStockData();
    void addNew();
    void openFilter();
    void clearFilter();
    void openLuluFilter();
    void exportLuluURL();
    void showPriceChangedData();
    void showShippingChangedData();
    void showWithoutBarcodeData();
    void startCheckPrice();

    void filterLuluPriceSpread(bool over10);
    void openStockComparingSetting();

signals:

public slots:
    void barValueChanged(int hvalue, int vvalue);
    void requestInsert(const NamedDataList& data);
    void mouseWheelValue(int value);

protected:
    void resizeEvent(QResizeEvent *) override;

private:
    PrivateDataArea*    m_data;
};

#endif // DATAAREA_H
