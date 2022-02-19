#include <QPainter>
#include <QMessageBox>
#include <QDebug>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QMenu>
#include <QAction>
#include <QApplication>
#include <QClipboard>
#include <QGestureEvent>
#include "dataeditor.h"
#include "myquickdata.h"
#include "modifypopup.h"
#include "filterpopup.h"
#include "checkpricepopup.h"
#include "stockcomparingsettingpopup.h"

static const int cell_height = 30;

enum PrimeFilterType
{
    ShowLuluPriceLessThan10 = 1,
    ShowLuluPriceGreaterThan10,
    ShowChangedPrice,
    ShowChangedShipping,
    ShowNoBarCode
};

class PrivateDataEditor
{
public:
    MyQuickData         data;
    QList<NamedDataList>    dataShown;
    int                 h_position;
    int                 v_position;
    int                 currentSelected;
    int                 currentInserting;
    QString             currentCellString;
    QMenu*              contextMenu;

    Filter              filter;
    int                 primeFilterType;

    bool                redBrushForUnchangedStock;
    bool                dragStart;

    int                 dragSourceIndex;
    int                 dragTargetIndex;
};

DataEditor::DataEditor(QWidget *parent)
    : QWidget(parent)
    , m_data(new PrivateDataEditor)
{
    m_data->data.open();
    m_data->h_position = 0;
    m_data->v_position = 0;
    m_data->currentSelected = -1;
    m_data->currentInserting = -1;
    m_data->redBrushForUnchangedStock = false;
    m_data->dragStart = false;
    m_data->primeFilterType = 0;
    connect(&m_data->data, SIGNAL(dataChanged()), this, SLOT(dataChanged()));

    m_data->contextMenu = new QMenu;
    m_data->contextMenu->addAction("复制单元格内容");
    m_data->contextMenu->addAction("修改");
    m_data->contextMenu->addAction("空白插入(下方)");
    m_data->contextMenu->addAction("复制本条插入(下方)");
    m_data->contextMenu->addAction("删除");

    connect(m_data->contextMenu, SIGNAL(triggered(QAction*)), this, SLOT(menuTriggered(QAction*)));
    connect(&m_data->data, SIGNAL(renameSuccessful(QString, QString)), this, SLOT(renameSuccessful(QString, QString)));

    refreshShownData();

    grabGesture(Qt::TapAndHoldGesture);
}

DataEditor::~DataEditor()
{
    delete m_data;
}

void DataEditor::add(const NamedDataList& data)
{
    m_data->data.add(data);
}

QList<ColumnDetail> DataEditor::columnDetails() const
{
    return m_data->data.columnDetails();
}

void DataEditor::dataChanged()
{
    refreshShownData();
//    m_data->v_position = 0;
//    m_data->h_position = 0;
    m_data->currentSelected = -1;

    resetBarValues();
    update();
}

void DataEditor::hbarValueChanged(int value)
{
    m_data->h_position = value;
    update();
}

void DataEditor::vbarValueChanged(int value)
{
    m_data->v_position = value;
    update();
}

void DataEditor::menuTriggered(QAction* action)
{
    if (action->text() == "删除") {
        if (QMessageBox::Yes == QMessageBox::question(nullptr, "确认", "确定删除本条吗？")) {
            if (m_data->data.drop(m_data->dataShown.at(m_data->currentSelected))) {
                m_data->dataShown.removeAt(m_data->currentSelected);
                m_data->currentSelected = -1;
                update();
            }
        }
    }
    else if (action->text() == "修改") {
        openModifyPopUp();
    }
    else if (action->text() == "空白插入(下方)") {
        m_data->currentInserting = m_data->currentSelected;
        addNew();
        m_data->currentInserting = -1;
    }
    else if (action->text() == "复制本条插入(下方)") {
        m_data->currentInserting = m_data->currentSelected;
        addNew(false);
        m_data->currentInserting = -1;
    }
    else if (action->text() == "复制单元格内容") {
        QApplication::clipboard()->setText(m_data->currentCellString);
    }
}

void DataEditor::requestUpdate(const NamedDataList& data)
{
    NamedDataList d = data;
    NamedDataList d1 = m_data->dataShown.at(m_data->currentSelected);
    m_data->data.update(d1, d);
}

void DataEditor::requestAddNew(const NamedDataList& data)
{
    if (m_data->currentInserting) {
        NamedDataList d1 = m_data->dataShown.at(m_data->currentInserting);
        m_data->data.insert(d1, data);
    }
    else {
        m_data->data.add(data);
    }
}

void DataEditor::requestFilterRefresh(const Filter& filter)
{
    m_data->primeFilterType = 0;
    m_data->filter = filter;
    refreshShownData();
    resetBarValues();
}

void DataEditor::requestUpdateLuluPrice(const QMap<QString, QString>& prices, const QList<NamedDataList>& data, const QMap<QString, QString>& changedShippingMap)
{
    m_data->data.updateLuluPrice(prices, data, changedShippingMap);
}

void DataEditor::setStockComparingDays(int days, bool redBrushForUnchangedStock)
{
    m_data->redBrushForUnchangedStock = redBrushForUnchangedStock;
    m_data->data.setStockComparingDays(days);
    refreshShownData();
    resetBarValues();
    emit updateColumnDetails(m_data->data.columnDetails());
}

void DataEditor::renameSuccessful(const QString& oldName, const QString& newName)
{
    for (int i = 0; i < m_data->dataShown.size(); ++i) {
        NamedDataList dl = m_data->dataShown.at(i);
        if (dl.at(3).data.toString() == oldName) {
            m_data->dataShown[i][3].data = newName;
            break;
        }
    }
}

void DataEditor::save()
{
    m_data->data.save();
}

void DataEditor::readStockData()
{
    m_data->data.readStockData();
    refreshShownData();
    resetBarValues();
    update();
}

void DataEditor::addNew(bool isBlank)
{
    ModifyPopUp pop;
    pop.setModal(true);
//    pop.resize(700, 300);

    NamedDataList d;
    if (!isBlank && -1 != m_data->currentSelected) {
        d = m_data->dataShown.at(m_data->currentSelected);
    }

    pop.showPopUp(m_data->data.brands(), d);
    connect(&pop, SIGNAL(requestUpdate(NamedDataList)), this, SLOT(requestAddNew(NamedDataList)));
    pop.exec();
}

void DataEditor::openFilter()
{
    FilterPopUp pop;
    pop.setModal(true);
    pop.showPopUp(m_data->filter, m_data->data.brands());
    connect(&pop, SIGNAL(requestUpdate(Filter)), this, SLOT(requestFilterRefresh(Filter)));
    pop.exec();
}

void DataEditor::clearFilter()
{
    m_data->primeFilterType = 0;
    m_data->filter = Filter();
    requestFilterRefresh(m_data->filter);
}

void DataEditor::openLuluFilter()
{
    m_data->filter = Filter();
    m_data->filter.luluHas = FILTER_STATE_YES;
    requestFilterRefresh(m_data->filter);
}

void DataEditor::exportLuluURL()
{
    Filter filter;
    filter.luluHas = FILTER_STATE_YES;
    QList<NamedDataList> dl = m_data->data.filteredData(filter);

    if (dl.isEmpty()) {
        QMessageBox::warning(nullptr, "警告", "没有发现任何沈璐家上架的货品！");
        return;
    }

    QString result;

    for (int i = 0; i < dl.size(); ++i) {
        result += dl.at(i).at(3).data.toString();
        result += "   ";
        result += dl.at(i).at(9).data.toString();
        result += '\n';
    }

    QApplication::clipboard()->setText(result);
    QMessageBox::warning(nullptr, "提示", "已全部复制到剪贴板！");
}

void DataEditor::showPriceChangedData()
{
    m_data->primeFilterType = ShowChangedPrice;
    m_data->dataShown = m_data->data.priceChangedData();
    resetBarValues();
    update();
}

void DataEditor::showShippingChangedData()
{
    m_data->primeFilterType = ShowChangedShipping;
    m_data->dataShown = m_data->data.shippingChangedData();
    resetBarValues();
    update();
}

void DataEditor::showWithoutBarcodeData()
{
    m_data->primeFilterType = ShowNoBarCode;
    m_data->dataShown = m_data->data.withoutBarcodeData();
    resetBarValues();
    update();
}

void DataEditor::startCheckPrice()
{
    CheckPricePopUp pop;

    Filter filter;
    filter.taxfree = FILTER_STATE_YES;
    filter.luluHas = FILTER_STATE_YES;
    QList<NamedDataList> data = m_data->data.filteredData(filter);

    pop.showPopUp(m_data->data.getLuluPrice(), data, m_data->data.getYesterdayLuluPrice());
    connect(&pop, SIGNAL(requestUpdate(QMap<QString, QString>, QList<NamedDataList>, QMap<QString, QString>)),
            this, SLOT(requestUpdateLuluPrice(QMap<QString, QString>, QList<NamedDataList>, QMap<QString, QString>)));
    pop.exec();
    refreshShownData();
    resetBarValues();
    update();
}

void DataEditor::filterLuluPriceSpread(bool over10)
{
    if (over10) {
        m_data->primeFilterType = ShowLuluPriceGreaterThan10;
    }
    else {
        m_data->primeFilterType = ShowLuluPriceLessThan10;
    }

    QList<NamedDataList> data = m_data->data.allData();
    m_data->dataShown.clear();

    for (int i = 0; i < data.size(); ++i) {
        NamedDataList dl = data.at(i);
        if (!dl.at(8).data.toString().isEmpty()) {
            int myPrice = dl.at(5).data.toString().toInt();
            int luluPrice = dl.at(8).data.toString().toInt();
            int spread = qAbs(myPrice - luluPrice);
            if (over10 && spread > 10) {
                m_data->dataShown << dl;
            }
            if (!over10 && spread <= 10) {
                m_data->dataShown <<dl;
            }
        }
    }
    resetBarValues();
    update();
}

void DataEditor::openStockComparingSetting()
{
    StockComparingSettingPopUp pop;
    connect(&pop, SIGNAL(comparingStockDays(int, bool)), this, SLOT(setStockComparingDays(int, bool)));
    pop.exec();
}

void DataEditor::showQuickFiltered(const QString &content)
{
    if (content.isEmpty()) {
        m_data->dataShown = m_data->data.allData();
    }
    else {
        m_data->dataShown = m_data->data.quickFilteredData(content);
    }
    resetBarValues();
    update();
}

void DataEditor::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    int startIndex = m_data->v_position / cell_height;
    int startY = startIndex * cell_height - m_data->v_position;
    int endIndex = (m_data->v_position + height()) / cell_height + 1;

    QPen gridPen;
    gridPen.setColor(QColor(57, 57, 57, 80));
    gridPen.setWidth(2);

    QPen textPen;
    textPen.setColor(QColor(5, 5, 5));

    QPen spTextPen;
    spTextPen.setColor(QColor(50, 50, 180));

    QFont normalFont = painter.font();
    QFont boldFont = normalFont;
    boldFont.setBold(true);

    QList<int> columnWidth = m_data->data.columnWidth();
    int prec = -1;

    for (int i = startIndex; i < m_data->dataShown.size() && i < endIndex; ++i) {
        NamedDataList d = m_data->dataShown.at(i);
        int startX = -m_data->h_position;
        int y = startY + (i-startIndex) * cell_height;

        bool priceEquals = false;

        if (i == m_data->currentSelected) {
            painter.setPen(Qt::NoPen);
            painter.setBrush(QColor(48, 84, 179, 150));
            painter.drawRect(QRect(0, y, width(), cell_height));
        }
        else {
            if (!d.at(8).data.toString().isEmpty()) {
                int myPrice = d.at(5).data.toString().toInt();
                int luluPrice = d.at(8).data.toString().toInt();

                if (myPrice < luluPrice || myPrice > luluPrice + 10) {
                    painter.setBrush(QColor(128, 60, 60, 100));
                    painter.drawRect(QRect(0, y, width(), cell_height));
                }

                if (myPrice == luluPrice) {
                    priceEquals = true;
                }
            }
        }

        painter.setBrush(Qt::NoBrush);

        for (int j = 0; j < columnWidth.size(); ++j) {
            QRect r(startX, y, columnWidth.at(j), cell_height);
            painter.setPen(gridPen);

            if (j == 10 || j == 11) {
                if (!d.at(11).data.toString().isEmpty()) {
                    int stock = 0;
                    if (!d.at(10).data.toString().isEmpty()) {
                        stock = d.at(10).data.toString().toInt();
                    }
                    int comparingStock = d.at(11).data.toString().toInt();
                    if (stock < comparingStock) {
                        painter.setBrush(QColor(50, 255, 50, 150));
                    }

                    if (m_data->redBrushForUnchangedStock && stock == comparingStock) {
                        painter.setBrush(QColor(220, 25, 25, 150));
                    }
                }
            }


            if (j == 2) {
                if (-1 != prec) {
                    painter.setBrush(QColor((prec + 128) % 255, 255-prec, prec, 100));
                }
                else {
                    if (i < m_data->dataShown.size()-1 && d.at(2).data == m_data->dataShown.at(i+1).at(2).data) { //商品ID一样，同款不同SKU

                        int realStart = i;
                        while (realStart > 0 && m_data->dataShown.at(realStart).at(2).data == m_data->dataShown.at(realStart-1).at(2).data) {
                            realStart--;
                        } //开始序列不一样的话，拉到一半会变色

                        prec = (realStart * 13) % 128;
                        painter.setBrush(QColor((prec + 128) % 255, 255-prec, prec, 100));
                    }
                }
            }

            if (j == 3) {
                if (-1 != prec) {
                    painter.setBrush(QColor((prec + 128) % 255, 255-prec, prec, 100));
                }

                if (i < m_data->dataShown.size()-1 && d.at(2).data != m_data->dataShown.at(i+1).at(2).data) {
                    prec = -1;
                }
            }


            painter.drawRect(r);
            painter.setBrush(Qt::NoBrush);

            if (j == 5 || j == 8 || j == 10 || j == 11) {
                if (priceEquals && (j == 5 || j == 8)) {
                    painter.setPen(QColor(50, 180, 50));
                }
                else {
                    painter.setPen(spTextPen);
                }
                painter.setFont(boldFont);
            }
            else {
                painter.setPen(textPen);
                painter.setFont(normalFont);
            }

            painter.drawText(r, Qt::AlignCenter, d.at(j).data.toString());
            startX += r.width();
        }

        if (m_data->dragStart) {
            QPen redPen;
            redPen.setColor(Qt::red);
            redPen.setWidth(4);
            painter.setPen(redPen);
            if (-1 == m_data->dragTargetIndex && 0 == i) {
                int lineStartY = y;
                if (lineStartY < 0) {
                    lineStartY = 0;
                }
                painter.drawLine(0, lineStartY, width(), lineStartY);
            }
            else if (m_data->dragTargetIndex == i) {
                int lineStartY = y + cell_height;

                if (lineStartY < 0) {
                    lineStartY = 0;
                }

                if (lineStartY > height()) {
                    lineStartY = height();
                }
                painter.drawLine(0, lineStartY, width(), lineStartY);
            }
        }
    }
}

void DataEditor::resizeEvent(QResizeEvent *)
{
    resetBarValues();
}

void DataEditor::mousePressEvent(QMouseEvent *e)
{
    QPoint p = e->pos();
    int index = (p.y() + m_data->v_position) / cell_height;

    if (index >= m_data->dataShown.size()) {
        index = -1;
    }

    m_data->currentSelected = index;
    update();
}

void DataEditor::wheelEvent(QWheelEvent *e)
{
    if (e->delta() > 0) {
        emit mouseWheelValue(-50);
    }
    else {
        emit mouseWheelValue(50);
    }
}

void DataEditor::contextMenuEvent(QContextMenuEvent *e)
{
    if (m_data->currentSelected != -1) {
        m_data->contextMenu->move(e->globalPos());
        QPoint p = mapFromGlobal(e->globalPos());
        int index = (m_data->v_position + p.y()) / cell_height;
        QList<int> columnWidth = m_data->data.columnWidth();
        int startX = -m_data->h_position;
        m_data->currentCellString.clear();
        for (int i = 0; i < columnWidth.size(); ++i) {
            if (startX < p.x() && p.x() < startX + columnWidth.at(i)) {
                m_data->currentCellString = m_data->dataShown.at(index).at(i).data.toString();
                break;
            }
            startX += columnWidth.at(i);
        }
        m_data->contextMenu->exec();
    }
}

void DataEditor::mouseDoubleClickEvent(QMouseEvent *)
{
    if (m_data->currentSelected != -1) {
        openModifyPopUp();
    }
}

void DataEditor::mouseMoveEvent(QMouseEvent *e)
{
    if (m_data->dragStart) {
        if (e->pos().y() < 0 || e->pos().y() > height()) {
            return;
        }

        if (m_data->dragSourceIndex >= m_data->dataShown.size()) {
            return;
        }

        int index = (e->pos().y() + m_data->v_position) / cell_height;

        if (index >= m_data->dataShown.size()) {
            m_data->dragTargetIndex = m_data->dataShown.size() - 1;
            return;
        }

        int dif = e->pos().y() + m_data->v_position - index * cell_height;
        if (dif <= cell_height/2) {
            m_data->dragTargetIndex = index - 1;
        }
        else {
            m_data->dragTargetIndex = index;
        }
        update();
    }
}

void DataEditor::mouseReleaseEvent(QMouseEvent *)
{
    if (m_data->dragStart) {
        if (m_data->dragSourceIndex < m_data->dataShown.size() && m_data->dragTargetIndex < m_data->dataShown.size()) {
            if (m_data->dragTargetIndex != m_data->dragSourceIndex) {
                if (-1 == m_data->dragTargetIndex) {
                    m_data->data.moveBefore(m_data->dataShown.at(m_data->dragSourceIndex),
                                            m_data->dataShown.at(0));
                }
                else {
                    m_data->data.moveAfter(m_data->dataShown.at(m_data->dragSourceIndex),
                                           m_data->dataShown.at(m_data->dragTargetIndex));
                }
            }
        }
        m_data->dragStart = false;
        update();
    }
}

bool DataEditor::event(QEvent* event)
{
    if (event->type() == QEvent::Gesture) {
        QGestureEvent* e = static_cast<QGestureEvent*>(event);
        QGesture* g = e->gesture(Qt::TapAndHoldGesture);
        if (g->state() == Qt::GestureStarted && !m_data->dataShown.isEmpty()) {
            m_data->dragSourceIndex = -2;
            m_data->dragTargetIndex = -2;
            m_data->dragStart = true;

            QTapAndHoldGesture* tg = static_cast<QTapAndHoldGesture*>(g);
            QPoint p = mapFromGlobal(tg->position().toPoint());
            m_data->dragSourceIndex = (p.y() + m_data->v_position) / cell_height;

            if (0 <= m_data->dragSourceIndex && m_data->dragSourceIndex < m_data->dataShown.size()) {
                m_data->dragTargetIndex = m_data->dragSourceIndex;
            }

            update();
        }
    }

    return QWidget::event(event);
}

void DataEditor::resetBarValues()
{
    int vvalue = m_data->dataShown.size() * cell_height - height();
    if (vvalue < 0) {
        vvalue = 0;
    }
    QList<int> columnWidth = m_data->data.columnWidth();
    int hvalue = 0;
    for (int i = 0; i < columnWidth.size(); ++i) {
        hvalue += columnWidth.at(i);
    }
    hvalue -= width();

    if (hvalue < 0) {
        hvalue = 0;
    }
    emit barValueChanged(hvalue, vvalue);
}

void DataEditor::openModifyPopUp()
{
    ModifyPopUp pop;
    pop.setModal(true);
//    pop.resize(700, 300);
    NamedDataList d = m_data->dataShown.at(m_data->currentSelected);
    pop.showPopUp(d, m_data->data.brands());
    connect(&pop, SIGNAL(requestUpdate(NamedDataList)), this, SLOT(requestUpdate(NamedDataList)));
    connect(&m_data->data, SIGNAL(renameSuccessful(QString, QString)), &pop, SLOT(renameSuccessful(QString, QString)));
    connect(&pop, SIGNAL(requestRename(QString, QString)), &m_data->data, SLOT(renameData(QString, QString)));
    pop.exec();
}

void DataEditor::refreshShownData()
{
    if (m_data->primeFilterType == ShowLuluPriceLessThan10) {
        filterLuluPriceSpread(false);
        return;
    }
    else if (m_data->primeFilterType == ShowLuluPriceGreaterThan10) {
        filterLuluPriceSpread(true);
        return;
    }
    else if (m_data->primeFilterType == ShowChangedPrice) {
        showPriceChangedData();
        return;
    }
    else if (m_data->primeFilterType == ShowChangedShipping) {
        showShippingChangedData();
        return;
    }
    else if (m_data->primeFilterType == ShowNoBarCode) {
        showWithoutBarcodeData();
        return;
    }

    if (m_data->filter.isValid()) {
        m_data->dataShown = m_data->data.filteredData(m_data->filter);
    }
    else {
        m_data->dataShown = m_data->data.allData();
    }
    update();
}
