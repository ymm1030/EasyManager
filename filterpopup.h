#ifndef FILTERPOPUP_H
#define FILTERPOPUP_H

#include <QDialog>

class Filter;
class PrivateFilterPopUp;
class FilterPopUp : public QDialog
{
    Q_OBJECT
public:
    FilterPopUp(QWidget* parent = nullptr);
    ~FilterPopUp();

    void showPopUp(const Filter& filter, QStringList brands);

signals:
    void requestUpdate(const Filter& filter);

public slots:
    void okClicked();
    void cancelClicked();

private:
    void updateData();

private:
    PrivateFilterPopUp*     m_data;
};

#endif // FILTERPOPUP_H
