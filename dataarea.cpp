#include <QScrollBar>
#include <QDebug>
#include "dataarea.h"
#include "dataeditor.h"
#include "tabletitle.h"

class PrivateDataArea
{
public:
    DataEditor*     editor;
    QScrollBar*     hbar;
    QScrollBar*     vbar;
    TableTitle*     tableTitle;
};

DataArea::DataArea(QWidget *parent)
    : QWidget(parent)
    , m_data(new PrivateDataArea)
{
    m_data->editor = new DataEditor(this);
    connect(m_data->editor, SIGNAL(barValueChanged(int, int)), this, SLOT(barValueChanged(int, int)));

    m_data->hbar = new QScrollBar(this);
    m_data->hbar->setOrientation(Qt::Horizontal);
    m_data->hbar->setMinimum(0);
    m_data->hbar->setMaximum(0);
    connect(m_data->hbar, SIGNAL(valueChanged(int)), m_data->editor, SLOT(hbarValueChanged(int)));
    m_data->vbar = new QScrollBar(this);
    m_data->vbar->setOrientation(Qt::Vertical);
    m_data->vbar->setMinimum(0);
    m_data->vbar->setMaximum(0);
    connect(m_data->vbar, SIGNAL(valueChanged(int)), m_data->editor, SLOT(vbarValueChanged(int)));

    m_data->tableTitle = new TableTitle(this);
    m_data->tableTitle->setColumnDetails(m_data->editor->columnDetails());
    connect(m_data->hbar, SIGNAL(valueChanged(int)), m_data->tableTitle, SLOT(hvalueChanged(int)));

    connect(m_data->editor, SIGNAL(mouseWheelValue(int)), this, SLOT(mouseWheelValue(int)));
    connect(m_data->editor, SIGNAL(updateColumnDetails(QList<ColumnDetail>)), m_data->tableTitle, SLOT(setColumnDetails(QList<ColumnDetail>)));
}

DataArea::~DataArea()
{
    delete m_data;
}

void DataArea::barValueChanged(int hvalue, int vvalue)
{
    m_data->hbar->setMaximum(hvalue);
    m_data->hbar->setPageStep(m_data->editor->width());
    m_data->vbar->setMaximum(vvalue);
    m_data->vbar->setPageStep(m_data->editor->height());
}

void DataArea::requestInsert(const NamedDataList& data)
{
    m_data->editor->add(data);
}

void DataArea::mouseWheelValue(int value)
{
    m_data->vbar->setValue(m_data->vbar->value() + value);
}

void DataArea::save()
{
    m_data->editor->save();
}

void DataArea::readStockData()
{
    m_data->editor->readStockData();
}

void DataArea::addNew()
{
    m_data->editor->addNew();
}

void DataArea::openFilter()
{
    m_data->editor->openFilter();
}

void DataArea::clearFilter()
{
    m_data->editor->clearFilter();
}

void DataArea::openLuluFilter()
{
    m_data->editor->openLuluFilter();
}

void DataArea::exportLuluURL()
{
    m_data->editor->exportLuluURL();
}

void DataArea::showPriceChangedData()
{
    m_data->editor->showPriceChangedData();
}

void DataArea::showShippingChangedData()
{
    m_data->editor->showShippingChangedData();
}

void DataArea::showWithoutBarcodeData()
{
    m_data->editor->showWithoutBarcodeData();
}

void DataArea::startCheckPrice()
{
    m_data->editor->startCheckPrice();
}

void DataArea::filterLuluPriceSpread(bool over10)
{
    m_data->editor->filterLuluPriceSpread(over10);
}

void DataArea::openStockComparingSetting()
{
    m_data->editor->openStockComparingSetting();
}

void DataArea::resizeEvent(QResizeEvent *)
{
    m_data->tableTitle->setGeometry(0, 0, width() - 20, 30);
    m_data->editor->setGeometry(0, 30, width() - 20, height() - 20 - 30);
    m_data->vbar->setGeometry(width() - 20, 0, 20, height());
    m_data->hbar->setGeometry(0, height() - 20, width(), 20);
}
