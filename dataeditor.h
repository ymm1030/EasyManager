#ifndef DATAEDITOR_H
#define DATAEDITOR_H

#include <QWidget>
#include "basedefine.h"

class Filter;
class PrivateDataEditor;
class DataEditor : public QWidget
{
    Q_OBJECT
public:
    explicit DataEditor(QWidget *parent = nullptr);
    ~DataEditor();

    void add(const NamedDataList& data);
    QList<ColumnDetail> columnDetails() const;

    void save();
    void readStockData();
    void addNew(bool isBlank = true);
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

    void showQuickFiltered(const QString& content);

signals:
    void barValueChanged(int hvalue, int vvalue);
    void mouseWheelValue(int value);
    void updateColumnDetails(QList<ColumnDetail>);
    void clearQuickFilter();

public slots:
    void dataChanged();
    void hbarValueChanged(int value);
    void vbarValueChanged(int value);
    void menuTriggered(QAction* action);
    void requestUpdate(const NamedDataList& data);
    void requestAddNew(const NamedDataList& data);
    void requestFilterRefresh(const Filter& filter);
    void requestUpdateLuluPrice(const QMap<QString, QString>& prices, const QList<NamedDataList>& data, const QMap<QString, QString>& changedShippingMap);
    void setStockComparingDays(int days, bool redBrushForUnchangedStock);
    void renameSuccessful(const QString& oldName, const QString& newName);

protected:
    void paintEvent(QPaintEvent *) override;
    void resizeEvent(QResizeEvent *) override;
    void mousePressEvent(QMouseEvent *e) override;
    void wheelEvent(QWheelEvent* e) override;
    void contextMenuEvent(QContextMenuEvent *e) override;
    void mouseDoubleClickEvent(QMouseEvent *e) override;
    void mouseMoveEvent(QMouseEvent *e) override;
    void mouseReleaseEvent(QMouseEvent *e) override;

    bool event(QEvent *event) override;

private:
    void resetBarValues();
    void openModifyPopUp();
    void refreshShownData();

private:
    PrivateDataEditor*      m_data;
};

#endif // DATAEDITOR_H
