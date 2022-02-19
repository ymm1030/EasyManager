#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMessageBox>
#include "basedefine.h"
#include "filterpopup.h"
#include "myquickdata.h"

class PrivateFilterPopUp
{
public:
    QLabel*         taxfreeLabel;
    QComboBox*      taxfreeCombo;
    QLabel*         brandLabel;
    QComboBox*      brandInput;
    QLabel*         tbIdLabel;
    QLineEdit*      tbIdInput;
    QLabel*         nameLabel;
    QLineEdit*      nameInput;
    QLabel*         requestPriceLabel;
    QComboBox*      requestPriceCombo;
    QLabel*         actualPriceLabel;
    QComboBox*      actualPriceCombo;
    QLineEdit*      actualPriceInput;
    QLabel*         freeShippingLabel;
    QComboBox*      freeShippingCombo;
    QLabel*         luluHasLabel;
    QComboBox*      luluHasCombo;
    QLabel*         barcodeLabel;
    QLineEdit*      barcodeInput;
    QLabel*         currentStockLabel;
    QComboBox*      currentStockCombo;
    QLineEdit*      currentStockInput;
    QLabel*         spAppendLabel;
    QComboBox*      spAppendCombo;

    QPushButton*    okBtn;
    QPushButton*    cancelBtn;
};

FilterPopUp::FilterPopUp(QWidget* parent)
    : QDialog(parent)
    , m_data(new PrivateFilterPopUp)
{
    QVBoxLayout* layout = new QVBoxLayout;

    m_data->taxfreeLabel = new QLabel(this);
    m_data->taxfreeLabel->setText("免税品");
    m_data->taxfreeCombo = new QComboBox(this);
    m_data->taxfreeCombo->addItem("无所谓");
    m_data->taxfreeCombo->addItem("是");
    m_data->taxfreeCombo->addItem("否");
    QHBoxLayout* taxfreeLayout = new QHBoxLayout;
    taxfreeLayout->addWidget(m_data->taxfreeLabel);
    taxfreeLayout->addWidget(m_data->taxfreeCombo);

    m_data->brandLabel = new QLabel(this);
    m_data->brandLabel->setText("分类/品牌");
    m_data->brandInput = new QComboBox(this);
    m_data->brandInput->setEditable(true);
    QHBoxLayout* brandLayout = new QHBoxLayout;
    brandLayout->addWidget(m_data->brandLabel);
    brandLayout->addWidget(m_data->brandInput);

    m_data->tbIdLabel = new QLabel(this);
    m_data->tbIdLabel->setText("商品ID");
    m_data->tbIdInput = new QLineEdit(this);
    QHBoxLayout* tbIdLayout = new QHBoxLayout;
    tbIdLayout->addWidget(m_data->tbIdLabel);
    tbIdLayout->addWidget(m_data->tbIdInput);

    m_data->nameLabel = new QLabel(this);
    m_data->nameLabel->setText("名称");
    m_data->nameInput = new QLineEdit(this);
    QHBoxLayout* nameLayout = new QHBoxLayout;
    nameLayout->addWidget(m_data->nameLabel);
    nameLayout->addWidget(m_data->nameInput);

    m_data->requestPriceLabel = new QLabel(this);
    m_data->requestPriceLabel->setText("控价");
    m_data->requestPriceCombo = new QComboBox(this);
    m_data->requestPriceCombo->addItem("无所谓");
    m_data->requestPriceCombo->addItem("是");
    m_data->requestPriceCombo->addItem("否");
    QHBoxLayout* requestPriceLayout = new QHBoxLayout;
    requestPriceLayout->addWidget(m_data->requestPriceLabel);
    requestPriceLayout->addWidget(m_data->requestPriceCombo);

    m_data->actualPriceLabel = new QLabel(this);
    m_data->actualPriceLabel->setText("上架价格");
    m_data->actualPriceCombo = new QComboBox(this);
    m_data->actualPriceCombo->addItem("无所谓");
    m_data->actualPriceCombo->addItem("大于");
    m_data->actualPriceCombo->addItem("大于等于");
    m_data->actualPriceCombo->addItem("等于");
    m_data->actualPriceCombo->addItem("小于");
    m_data->actualPriceCombo->addItem("小于等于");
    m_data->actualPriceInput = new QLineEdit(this);
    QHBoxLayout* actualPriceLayout = new QHBoxLayout;
    actualPriceLayout->addWidget(m_data->actualPriceLabel);
    actualPriceLayout->addWidget(m_data->actualPriceCombo);
    actualPriceLayout->addWidget(m_data->actualPriceInput);

    m_data->freeShippingLabel = new QLabel(this);
    m_data->freeShippingLabel->setText("包邮");
    m_data->freeShippingCombo = new QComboBox(this);
    m_data->freeShippingCombo->addItem("无所谓");
    m_data->freeShippingCombo->addItem("是");
    m_data->freeShippingCombo->addItem("否");
    QHBoxLayout* freeShippingLayout = new QHBoxLayout;
    freeShippingLayout->addWidget(m_data->freeShippingLabel);
    freeShippingLayout->addWidget(m_data->freeShippingCombo);

    m_data->luluHasLabel = new QLabel(this);
    m_data->luluHasLabel->setText("沈璐有");
    m_data->luluHasCombo = new QComboBox(this);
    m_data->luluHasCombo->addItem("无所谓");
    m_data->luluHasCombo->addItem("是");
    m_data->luluHasCombo->addItem("否");
    QHBoxLayout* luluHasLayout = new QHBoxLayout;
    luluHasLayout->addWidget(m_data->luluHasLabel);
    luluHasLayout->addWidget(m_data->luluHasCombo);

    m_data->barcodeLabel = new QLabel(this);
    m_data->barcodeLabel->setText("条码");
    m_data->barcodeInput = new QLineEdit(this);
    QHBoxLayout* barcodeLayout = new QHBoxLayout;
    barcodeLayout->addWidget(m_data->barcodeLabel);
    barcodeLayout->addWidget(m_data->barcodeInput);

    m_data->currentStockLabel = new QLabel(this);
    m_data->currentStockLabel->setText("当前库存");
    m_data->currentStockCombo = new QComboBox(this);
    m_data->currentStockCombo->addItem("无所谓");
    m_data->currentStockCombo->addItem("大于");
    m_data->currentStockCombo->addItem("大于等于");
    m_data->currentStockCombo->addItem("等于");
    m_data->currentStockCombo->addItem("小于");
    m_data->currentStockCombo->addItem("小于等于");
    m_data->currentStockInput = new QLineEdit(this);
    QHBoxLayout* currentStockLayout = new QHBoxLayout;
    currentStockLayout->addWidget(m_data->currentStockLabel);
    currentStockLayout->addWidget(m_data->currentStockCombo);
    currentStockLayout->addWidget(m_data->currentStockInput);

    m_data->spAppendLabel = new QLabel(this);
    m_data->spAppendLabel->setText("含有备注");
    m_data->spAppendCombo = new QComboBox(this);
    m_data->spAppendCombo->addItem("无所谓");
    m_data->spAppendCombo->addItem("是");
    m_data->spAppendCombo->addItem("否");
    QHBoxLayout* spAppendLayout = new QHBoxLayout;
    spAppendLayout->addWidget(m_data->spAppendLabel);
    spAppendLayout->addWidget(m_data->spAppendCombo);

    m_data->okBtn = new QPushButton(this);
    m_data->okBtn->setText("确定");
    m_data->cancelBtn = new QPushButton(this);
    m_data->cancelBtn->setText("取消");
    QHBoxLayout* lastLayout = new QHBoxLayout;
    lastLayout->addWidget(m_data->okBtn);
    lastLayout->addWidget(m_data->cancelBtn);

    layout->addLayout(taxfreeLayout);
    layout->addLayout(brandLayout);
    layout->addLayout(tbIdLayout);
    layout->addLayout(nameLayout);
    layout->addLayout(requestPriceLayout);
    layout->addLayout(actualPriceLayout);
    layout->addLayout(freeShippingLayout);
    layout->addLayout(luluHasLayout);
    layout->addLayout(barcodeLayout);
    layout->addLayout(currentStockLayout);
    layout->addLayout(spAppendLayout);

    layout->addLayout(lastLayout);
    setLayout(layout);

    connect(m_data->okBtn, SIGNAL(clicked()), this, SLOT(okClicked()));
    connect(m_data->cancelBtn, SIGNAL(clicked()), this, SLOT(cancelClicked()));

    {
        QRegExp regExp("[0-9].+$");
        QRegExpValidator *pattern = new QRegExpValidator(regExp, this);
        m_data->tbIdInput->setValidator(pattern);
        m_data->actualPriceInput->setValidator(pattern);
        m_data->currentStockInput->setValidator(pattern);
    }

    {
        QRegExp regExp("[0-9A-Za-z].+$");
        QRegExpValidator *pattern = new QRegExpValidator(regExp, this);
        m_data->barcodeInput->setValidator(pattern);
    }
}

FilterPopUp::~FilterPopUp()
{
    delete m_data;
}

void FilterPopUp::showPopUp(const Filter& filter, QStringList brands)
{
    switch (filter.taxfree) {
    case FILTER_STATE_NOTCARE:
        m_data->taxfreeCombo->setCurrentIndex(0);
        break;
    case FILTER_STATE_YES:
        m_data->taxfreeCombo->setCurrentIndex(1);
        break;
    case FILTER_STATE_NO:
        m_data->taxfreeCombo->setCurrentIndex(2);
        break;
    default:
        break;
    }

    m_data->brandInput->addItems(brands);
    m_data->brandInput->setCurrentText(filter.brand);
    m_data->tbIdInput->setText(filter.tbId);
    m_data->nameInput->setText(filter.name);

    switch (filter.controlledValue) {
    case FILTER_STATE_NOTCARE:
        m_data->requestPriceCombo->setCurrentIndex(0);
        break;
    case FILTER_STATE_YES:
        m_data->requestPriceCombo->setCurrentIndex(1);
        break;
    case FILTER_STATE_NO:
        m_data->requestPriceCombo->setCurrentIndex(2);
        break;
    default:
        break;
    }

    m_data->actualPriceInput->setText(QString::number(filter.actualValue.target));
    switch (filter.actualValue.method) {
    case COMPARE_METHOD_NOTCARE:
        m_data->actualPriceCombo->setCurrentText("无所谓");
        break;
    case COMPARE_METHOD_GREATER:
        m_data->actualPriceCombo->setCurrentText("大于");
        break;
    case COMPARE_METHOD_GREATER_EQUAL:
        m_data->actualPriceCombo->setCurrentText("大于等于");
        break;
    case COMPARE_METHOD_EQUAL:
        m_data->actualPriceCombo->setCurrentText("等于");
        break;
    case COMPARE_METHOD_LESS:
        m_data->actualPriceCombo->setCurrentText("小于");
        break;
    case COMPARE_METHOD_LESS_EQUAL:
        m_data->actualPriceCombo->setCurrentText("小于等于");
        break;
    default:
        break;
    }

    switch (filter.freeShipping) {
    case FILTER_STATE_NOTCARE:
        m_data->freeShippingCombo->setCurrentIndex(0);
        break;
    case FILTER_STATE_YES:
        m_data->freeShippingCombo->setCurrentIndex(1);
        break;
    case FILTER_STATE_NO:
        m_data->freeShippingCombo->setCurrentIndex(2);
        break;
    default:
        break;
    }

    switch (filter.luluHas) {
    case FILTER_STATE_NOTCARE:
        m_data->luluHasCombo->setCurrentIndex(0);
        break;
    case FILTER_STATE_YES:
        m_data->luluHasCombo->setCurrentIndex(1);
        break;
    case FILTER_STATE_NO:
        m_data->luluHasCombo->setCurrentIndex(2);
        break;
    default:
        break;
    }

    m_data->barcodeInput->setText(filter.barcode);

    m_data->currentStockInput->setText(QString::number(filter.currentStock.target));
    switch (filter.currentStock.method) {
    case COMPARE_METHOD_NOTCARE:
        m_data->currentStockCombo->setCurrentText("无所谓");
        break;
    case COMPARE_METHOD_GREATER:
        m_data->currentStockCombo->setCurrentText("大于");
        break;
    case COMPARE_METHOD_GREATER_EQUAL:
        m_data->currentStockCombo->setCurrentText("大于等于");
        break;
    case COMPARE_METHOD_EQUAL:
        m_data->currentStockCombo->setCurrentText("等于");
        break;
    case COMPARE_METHOD_LESS:
        m_data->currentStockCombo->setCurrentText("小于");
        break;
    case COMPARE_METHOD_LESS_EQUAL:
        m_data->currentStockCombo->setCurrentText("小于等于");
        break;
    default:
        break;
    }

    switch (filter.hasSpAppend) {
    case FILTER_STATE_NOTCARE:
        m_data->spAppendCombo->setCurrentIndex(0);
        break;
    case FILTER_STATE_YES:
        m_data->spAppendCombo->setCurrentIndex(1);
        break;
    case FILTER_STATE_NO:
        m_data->spAppendCombo->setCurrentIndex(2);
        break;
    default:
        break;
    }

    m_data->cancelBtn->setFocus();
}

void FilterPopUp::okClicked()
{
    updateData();
    close();
}

void FilterPopUp::cancelClicked()
{
    close();
}

void FilterPopUp::updateData()
{
    Filter filter;

    if ("无所谓" == m_data->taxfreeCombo->currentText()) {
        filter.taxfree = FILTER_STATE_NOTCARE;
    }
    else if ("是" == m_data->taxfreeCombo->currentText()) {
        filter.taxfree = FILTER_STATE_YES;
    }
    else if ("否" == m_data->taxfreeCombo->currentText()) {
        filter.taxfree = FILTER_STATE_NO;
    }

    filter.brand = m_data->brandInput->currentText();
    filter.tbId = m_data->tbIdInput->text().trimmed();
    filter.name = m_data->nameInput->text().trimmed();

    if ("无所谓" == m_data->requestPriceCombo->currentText()) {
        filter.controlledValue = FILTER_STATE_NOTCARE;
    }
    else if ("是" == m_data->requestPriceCombo->currentText()) {
        filter.controlledValue = FILTER_STATE_YES;
    }
    else if ("否" == m_data->requestPriceCombo->currentText()) {
        filter.controlledValue = FILTER_STATE_NO;
    }

    filter.actualValue.target = m_data->actualPriceInput->text().trimmed().toInt();
    if ("无所谓" == m_data->actualPriceCombo->currentText()) {
        filter.actualValue.method = COMPARE_METHOD_NOTCARE;
    }
    else if ("大于" == m_data->actualPriceCombo->currentText()) {
        filter.actualValue.method = COMPARE_METHOD_GREATER;
    }
    else if ("大于等于" == m_data->actualPriceCombo->currentText()) {
        filter.actualValue.method = COMPARE_METHOD_GREATER_EQUAL;
    }
    else if ("等于" == m_data->actualPriceCombo->currentText()) {
        filter.actualValue.method = COMPARE_METHOD_EQUAL;
    }
    else if ("小于" == m_data->actualPriceCombo->currentText()) {
        filter.actualValue.method = COMPARE_METHOD_LESS;
    }
    else if ("小于等于" == m_data->actualPriceCombo->currentText()) {
        filter.actualValue.method = COMPARE_METHOD_LESS_EQUAL;
    }

    if ("无所谓" == m_data->freeShippingCombo->currentText()) {
        filter.freeShipping = FILTER_STATE_NOTCARE;
    }
    else if ("是" == m_data->freeShippingCombo->currentText()) {
        filter.freeShipping = FILTER_STATE_YES;
    }
    else if ("否" == m_data->freeShippingCombo->currentText()) {
        filter.freeShipping = FILTER_STATE_NO;
    }

    if ("无所谓" == m_data->luluHasCombo->currentText()) {
        filter.luluHas = FILTER_STATE_NOTCARE;
    }
    else if ("是" == m_data->luluHasCombo->currentText()) {
        filter.luluHas = FILTER_STATE_YES;
    }
    else if ("否" == m_data->luluHasCombo->currentText()) {
        filter.luluHas = FILTER_STATE_NO;
    }

    filter.barcode = m_data->barcodeInput->text().trimmed();

    filter.currentStock.target = m_data->currentStockInput->text().trimmed().toInt();
    if ("无所谓" == m_data->currentStockCombo->currentText()) {
        filter.currentStock.method = COMPARE_METHOD_NOTCARE;
    }
    else if ("大于" == m_data->currentStockCombo->currentText()) {
        filter.currentStock.method = COMPARE_METHOD_GREATER;
    }
    else if ("大于等于" == m_data->currentStockCombo->currentText()) {
        filter.currentStock.method = COMPARE_METHOD_GREATER_EQUAL;
    }
    else if ("等于" == m_data->currentStockCombo->currentText()) {
        filter.currentStock.method = COMPARE_METHOD_EQUAL;
    }
    else if ("小于" == m_data->currentStockCombo->currentText()) {
        filter.currentStock.method = COMPARE_METHOD_LESS;
    }
    else if ("小于等于" == m_data->currentStockCombo->currentText()) {
        filter.currentStock.method = COMPARE_METHOD_LESS_EQUAL;
    }

    if ("无所谓" == m_data->spAppendCombo->currentText()) {
        filter.hasSpAppend = FILTER_STATE_NOTCARE;
    }
    else if ("是" == m_data->spAppendCombo->currentText()) {
        filter.hasSpAppend = FILTER_STATE_YES;
    }
    else if ("否" == m_data->spAppendCombo->currentText()) {
        filter.hasSpAppend = FILTER_STATE_NO;
    }

    emit requestUpdate(filter);
}
