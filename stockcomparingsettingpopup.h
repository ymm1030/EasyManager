#ifndef STOCKCOMPARINGSETTINGPOPUP_H
#define STOCKCOMPARINGSETTINGPOPUP_H

#include <QDialog>

class PrivateStockComparingSettingPopUp;
class StockComparingSettingPopUp : public QDialog
{
    Q_OBJECT
public:
    StockComparingSettingPopUp(QWidget* parent = nullptr);
    ~StockComparingSettingPopUp();

signals:
    void comparingStockDays(int, bool);

public slots:
    void btnClicked();

private:
    PrivateStockComparingSettingPopUp* m_data;
};

#endif // STOCKCOMPARINGSETTINGPOPUP_H
