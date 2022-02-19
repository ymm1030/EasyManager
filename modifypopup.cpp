#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QInputDialog>
#include <QDebug>
#include "modifypopup.h"
#include "basedefine.h"

class PrivateModifyPopUp
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
    QPushButton*    renameBtn;
    QLabel*         requestPriceLabel;
    QLineEdit*      requestPriceInput;
    QLabel*         actualPriceLabel;
    QLineEdit*      actualPriceInput;
    QLabel*         freeShippingLabel;
    QComboBox*      freeShippingCombo;
//    QLabel*         luluHasLabel;
//    QComboBox*      luluHasCombo;
    QLabel*         barcodeLabel;
    QLineEdit*      barcodeInput;
//    QLabel*         currentStockLabel;
//    QLineEdit*      currentStockInput;
//    QLabel*         lastStockLabel;
//    QLineEdit*      lastStockInput;
    QLabel*         luluURLLabel;
    QLineEdit*      luluURLInput;
    QLabel*         spAppendLabel;
    QLineEdit*      spAppendInput;

    QPushButton*    okBtn;
    QPushButton*    cancelBtn;
    QPushButton*    copyLastBtn;

    NamedDataList   data;
    NamedDataList   lastRecord;

    QString         newName;
};

ModifyPopUp::ModifyPopUp(QWidget* parent)
    : QDialog(parent)
    , m_data(new PrivateModifyPopUp)
{
    QVBoxLayout* layout = new QVBoxLayout;

    m_data->taxfreeLabel = new QLabel(this);
    m_data->taxfreeLabel->setText("免税品");
    m_data->taxfreeCombo = new QComboBox(this);
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
    m_data->nameInput->setReadOnly(true);
    m_data->renameBtn = new QPushButton(this);
    m_data->renameBtn->setText("改名");
    QHBoxLayout* nameLayout = new QHBoxLayout;
    nameLayout->addWidget(m_data->nameLabel);
    nameLayout->addWidget(m_data->nameInput);
    nameLayout->addWidget(m_data->renameBtn);

    m_data->requestPriceLabel = new QLabel(this);
    m_data->requestPriceLabel->setText("控价");
    m_data->requestPriceInput = new QLineEdit(this);
    QHBoxLayout* requestPriceLayout = new QHBoxLayout;
    requestPriceLayout->addWidget(m_data->requestPriceLabel);
    requestPriceLayout->addWidget(m_data->requestPriceInput);

    m_data->actualPriceLabel = new QLabel(this);
    m_data->actualPriceLabel->setText("上架价格");
    m_data->actualPriceInput = new QLineEdit(this);
    QHBoxLayout* actualPriceLayout = new QHBoxLayout;
    actualPriceLayout->addWidget(m_data->actualPriceLabel);
    actualPriceLayout->addWidget(m_data->actualPriceInput);

    m_data->freeShippingLabel = new QLabel(this);
    m_data->freeShippingLabel->setText("包邮");
    m_data->freeShippingCombo = new QComboBox(this);
    m_data->freeShippingCombo->addItem("是");
    m_data->freeShippingCombo->addItem("否");
    QHBoxLayout* freeShippingLayout = new QHBoxLayout;
    freeShippingLayout->addWidget(m_data->freeShippingLabel);
    freeShippingLayout->addWidget(m_data->freeShippingCombo);

    m_data->barcodeLabel = new QLabel(this);
    m_data->barcodeLabel->setText("条码");
    m_data->barcodeInput = new QLineEdit(this);
    QHBoxLayout* barcodeLayout = new QHBoxLayout;
    barcodeLayout->addWidget(m_data->barcodeLabel);
    barcodeLayout->addWidget(m_data->barcodeInput);

//    m_data->luluHasLabel = new QLabel(this);
//    m_data->luluHasLabel->setText("沈璐有");
//    m_data->luluHasCombo = new QComboBox(this);
//    m_data->luluHasCombo->addItem("是");
//    m_data->luluHasCombo->addItem("否");
//    QHBoxLayout* luluHasLayout = new QHBoxLayout;
//    luluHasLayout->addWidget(m_data->luluHasLabel);
//    luluHasLayout->addWidget(m_data->luluHasCombo);

//    m_data->currentStockLabel = new QLabel(this);
//    m_data->currentStockLabel->setText("当前库存");
//    m_data->currentStockInput = new QLineEdit(this);
//    QHBoxLayout* currentStockLayout = new QHBoxLayout;
//    currentStockLayout->addWidget(m_data->currentStockLabel);
//    currentStockLayout->addWidget(m_data->currentStockInput);

//    m_data->lastStockLabel = new QLabel(this);
//    m_data->lastStockLabel->setText("上次库存");
//    m_data->lastStockInput = new QLineEdit(this);
//    QHBoxLayout* lastStockLayout = new QHBoxLayout;
//    lastStockLayout->addWidget(m_data->lastStockLabel);
//    lastStockLayout->addWidget(m_data->lastStockInput);

    m_data->luluURLLabel = new QLabel(this);
    m_data->luluURLLabel->setText("沈璐家链接");
    m_data->luluURLInput = new QLineEdit(this);
    QHBoxLayout* luluURLLayout = new QHBoxLayout;
    luluURLLayout->addWidget(m_data->luluURLLabel);
    luluURLLayout->addWidget(m_data->luluURLInput);

    m_data->spAppendLabel = new QLabel(this);
    m_data->spAppendLabel->setText("特别备注");
    m_data->spAppendInput = new QLineEdit(this);
    QHBoxLayout* spAppendLayout = new QHBoxLayout;
    spAppendLayout->addWidget(m_data->spAppendLabel);
    spAppendLayout->addWidget(m_data->spAppendInput);

    m_data->okBtn = new QPushButton(this);
    m_data->okBtn->setText("确定");
    m_data->cancelBtn = new QPushButton(this);
    m_data->cancelBtn->setText("取消");
    m_data->copyLastBtn = new QPushButton(this);
    m_data->copyLastBtn->setText("复制上一条");
    QHBoxLayout* lastLayout = new QHBoxLayout;
    lastLayout->addWidget(m_data->okBtn);
    lastLayout->addWidget(m_data->cancelBtn);
    lastLayout->addWidget(m_data->copyLastBtn);

    layout->addLayout(taxfreeLayout);
    layout->addLayout(brandLayout);
    layout->addLayout(tbIdLayout);
    layout->addLayout(nameLayout);
    layout->addLayout(requestPriceLayout);
    layout->addLayout(actualPriceLayout);
    layout->addLayout(freeShippingLayout);
    layout->addLayout(barcodeLayout);
//    layout->addLayout(luluHasLayout);
//    layout->addLayout(currentStockLayout);
//    layout->addLayout(lastStockLayout);
    layout->addLayout(luluURLLayout);
    layout->addLayout(spAppendLayout);

    layout->addLayout(lastLayout);
    setLayout(layout);

    connect(m_data->okBtn, SIGNAL(clicked()), this, SLOT(okClicked()));
    connect(m_data->cancelBtn, SIGNAL(clicked()), this, SLOT(cancelClicked()));
    connect(m_data->copyLastBtn, SIGNAL(clicked()), this, SLOT(copyLastClicked()));
    connect(m_data->renameBtn, SIGNAL(clicked()), this, SLOT(renameClicked()));

    {
        QRegExp regExp("[0-9].+$");
        QRegExpValidator *pattern = new QRegExpValidator(regExp, this);
        m_data->tbIdInput->setValidator(pattern);
        m_data->requestPriceInput->setValidator(pattern);
        m_data->actualPriceInput->setValidator(pattern);
    }
    {
        QRegExp regExp("[0-9A-Za-z].+$");
        QRegExpValidator *pattern = new QRegExpValidator(regExp, this);
        m_data->barcodeInput->setValidator(pattern);
    }
//    m_data->currentStockInput->setValidator(pattern);
//    m_data->lastStockInput->setValidator(pattern);
}

ModifyPopUp::~ModifyPopUp()
{
    delete m_data;
}

void ModifyPopUp::showPopUp(QStringList brands, const NamedDataList& lastRecord)
{
    m_data->lastRecord = lastRecord;

    for (int i = 0; i < 13; ++i) {
        m_data->data << NamedData();
    }

    m_data->taxfreeCombo->setCurrentText("是");
    m_data->nameInput->setReadOnly(false);
    m_data->freeShippingCombo->setCurrentText("是");
//    m_data->luluHasCombo->setCurrentText("否");

    m_data->brandInput->addItems(brands);

    m_data->cancelBtn->setFocus();
}

void ModifyPopUp::showPopUp(const NamedDataList &data, QStringList brands)
{
    m_data->data = data;
    m_data->taxfreeCombo->setCurrentText(data.at(0).data.toString());
    m_data->brandInput->addItems(brands);
    m_data->brandInput->setCurrentText(data.at(1).data.toString());
    m_data->tbIdInput->setText(data.at(2).data.toString());
    m_data->nameInput->setText(data.at(3).data.toString());
    m_data->requestPriceInput->setText(data.at(4).data.toString());
    m_data->actualPriceInput->setText(data.at(5).data.toString());
    m_data->freeShippingCombo->setCurrentText(data.at(6).data.toString());
    m_data->barcodeInput->setText(data.at(7).data.toString());
//    m_data->luluHasCombo->setCurrentText(data.at(8).data.toString());
//    m_data->currentStockInput->setText(data.at(9).data.toString());
//    m_data->lastStockInput->setText(data.at(10).data.toString());
    m_data->luluURLInput->setText(data.at(9).data.toString());
    m_data->spAppendInput->setText(data.at(12).data.toString());

    m_data->actualPriceInput->setFocus();
}

void ModifyPopUp::okClicked()
{
    if (m_data->nameInput->text().isEmpty()) {
        QMessageBox::warning(nullptr, "错误", "名称不能为空！");
        return;
    }

    updateData();
    close();
}

void ModifyPopUp::cancelClicked()
{
    close();
}

void ModifyPopUp::copyLastClicked()
{
    NamedDataList data = m_data->lastRecord;

    if (data.isEmpty()) {
        return;
    }

    m_data->taxfreeCombo->setCurrentText(data.at(0).data.toString());
    m_data->brandInput->setCurrentText(data.at(1).data.toString());
    m_data->tbIdInput->setText(data.at(2).data.toString());
    m_data->nameInput->setText(data.at(3).data.toString());
    m_data->requestPriceInput->setText(data.at(4).data.toString());
    m_data->actualPriceInput->setText(data.at(5).data.toString());
    m_data->freeShippingCombo->setCurrentText(data.at(6).data.toString());
    m_data->barcodeInput->setText(data.at(7).data.toString());
    m_data->luluURLInput->setText(data.at(9).data.toString());
    m_data->spAppendInput->setText(data.at(12).data.toString());
}

void ModifyPopUp::renameClicked()
{
    m_data->newName = QInputDialog::getText(this, "改名", "请输入新的名称", QLineEdit::Normal, m_data->nameInput->text());
    if (!m_data->newName.isEmpty()) {
        emit requestRename(m_data->nameInput->text(), m_data->newName);
    }
}

void ModifyPopUp::renameSuccessful(const QString&, const QString&)
{
    m_data->nameInput->setText(m_data->newName);
}

void ModifyPopUp::updateData()
{
    m_data->data[0].data = m_data->taxfreeCombo->currentText();
    m_data->data[1].data = m_data->brandInput->currentText();
    m_data->data[2].data = m_data->tbIdInput->text().trimmed();
    m_data->data[3].data = m_data->nameInput->text().trimmed();
    m_data->data[4].data = m_data->requestPriceInput->text().trimmed();
    m_data->data[5].data = m_data->actualPriceInput->text().trimmed();
    m_data->data[6].data = m_data->freeShippingCombo->currentText();
    m_data->data[7].data = m_data->barcodeInput->text().trimmed();
//    m_data->data[8].data = m_data->luluHasCombo->currentText();
    m_data->data[9].data = m_data->luluURLInput->text().trimmed();
    m_data->data[12].data = m_data->spAppendInput->text().trimmed();

    emit requestUpdate(m_data->data);
}
