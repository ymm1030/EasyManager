#ifndef MODIFYPOPUP_H
#define MODIFYPOPUP_H

#include <QDialog>
#include "basedefine.h"

class PrivateModifyPopUp;
class ModifyPopUp : public QDialog
{
    Q_OBJECT
public:
    ModifyPopUp(QWidget* parent = nullptr);
    ~ModifyPopUp();

    void showPopUp(QStringList brands, const NamedDataList& lastRecord);
    void showPopUp(const NamedDataList& data, QStringList brands);

signals:
    void requestUpdate(const NamedDataList& data);
    void requestRename(const QString& oldName, const QString& newName);

public slots:
    void okClicked();
    void cancelClicked();
    void copyLastClicked();
    void renameClicked();
    void renameSuccessful(const QString& oldName, const QString& newName);

private:
    void updateData();

private:
    PrivateModifyPopUp*     m_data;
};

#endif // MODIFYPOPUP_H
