#ifndef CHECKPRICEPOPUP_H
#define CHECKPRICEPOPUP_H

#include <QDialog>
#include "basedefine.h"

class PrivateCheckPricePopUp;
class CheckPricePopUp : public QDialog
{
    Q_OBJECT
public:
    CheckPricePopUp(QWidget* parent = nullptr);
    ~CheckPricePopUp();

    void showPopUp(const QMap<QString, QString>& prices, const QList<NamedDataList>& data, const QMap<QString, QString>& yesterdayPrices);

signals:
    void requestUpdate(const QMap<QString, QString>&, const QList<NamedDataList>&, const QMap<QString, QString>&);

public slots:
    void requestOpenUrl();
    void prevClicked();
    void nextClicked();
    void ourPriceChanged(const QString& price);
    void shippingChanged(const QString& shipping);
    void clickTimeOut();

private:
    void makeNextFrame();

protected:
    void closeEvent(QCloseEvent *) override;

private:
    PrivateCheckPricePopUp*     m_data;
};

#endif // CHECKPRICEPOPUP_H
