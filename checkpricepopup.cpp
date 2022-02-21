#include <QScrollArea>
#include <QLabel>
#include <QDebug>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QLineEdit>
#include <QComboBox>
#include <QRegExpValidator>
#include <QDesktopServices>
#include <QUrl>
#include <QTimer>
#include "checkpricepopup.h"

class PrivateCheckPricePopUp
{
public:
    QScrollArea*            scrollArea;
    QWidget*                frame;

    QLabel*                 showRemainLabel;
    QPushButton*            openURLBtn;
    QPushButton*            prevBtn;
    QPushButton*            nextBtn;

    QList<QLabel*>          labels;
    QList<QLineEdit*>       inputs;

    QMap<QString, QString>          prices;
    QMap<QString, QString>          yesterdayPrices;
    QMap<QString, QString>          changedShippingMap;
    QMap<QString, QStringList>      dataMap;
    QStringList                     backingKeys;
    QString                         currentUrl;

    QRegExpValidator*              pattern;

    QList<NamedDataList>           data;

    QTimer                  timer;
};

CheckPricePopUp::CheckPricePopUp(QWidget* parent)
    : QDialog(parent)
    , m_data(new PrivateCheckPricePopUp)
{
    m_data->scrollArea = new QScrollArea(this);
    m_data->frame = nullptr;

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(m_data->scrollArea);

    QHBoxLayout* lastLayout = new QHBoxLayout(this);
    m_data->showRemainLabel = new QLabel(this);
    m_data->showRemainLabel->setText("剩余x项");
    QFont f = m_data->showRemainLabel->font();
    f.setBold(true);
    m_data->showRemainLabel->setFont(f);
    m_data->openURLBtn = new QPushButton(this);
    m_data->openURLBtn->setText("打开链接");
    m_data->prevBtn = new QPushButton(this);
    m_data->prevBtn->setText("上一个");
    m_data->nextBtn = new QPushButton(this);
    m_data->nextBtn->setText("下一个");
    lastLayout->addWidget(m_data->showRemainLabel);
    lastLayout->addWidget(m_data->prevBtn);
    lastLayout->addWidget(m_data->openURLBtn);
    lastLayout->addWidget(m_data->nextBtn);

    layout->addLayout(lastLayout);
    setLayout(layout);

    connect(m_data->prevBtn, SIGNAL(clicked()), this, SLOT(prevClicked()));
    connect(m_data->nextBtn, SIGNAL(clicked()), this, SLOT(nextClicked()));
    connect(m_data->openURLBtn, SIGNAL(clicked()), this, SLOT(requestOpenUrl()));

    QRegExp regExp("[0-9].+$");
    m_data->pattern = new QRegExpValidator(regExp, this);

    m_data->timer.setInterval(2000);
    m_data->timer.setSingleShot(true);
    connect(&m_data->timer, SIGNAL(timeout()), this, SLOT(clickTimeOut()));
}

CheckPricePopUp::~CheckPricePopUp()
{
    delete m_data;
}

void CheckPricePopUp::requestOpenUrl()
{
    QDesktopServices::openUrl(QUrl(m_data->currentUrl));
}

void CheckPricePopUp::prevClicked()
{
    for (int i = 0; i < m_data->labels.size(); ++i) {
        QLabel* label = m_data->labels.at(i);
        auto iter = m_data->prices.find(label->text());
        if (iter != m_data->prices.end()) {
            m_data->prices.erase(iter);  //返回上一个时候，清掉璐璐的价格，这样如果关闭对话框重新打开，就可以重新核对一次
        }
    }

    m_data->backingKeys.pop_back();
    makeNextFrame();
}

void CheckPricePopUp::nextClicked()
{
    for (int i = 0; i < m_data->inputs.size(); ++i) {
        QLineEdit* input = m_data->inputs.at(i);
        if (input->text().trimmed().isEmpty()) {
            QMessageBox::warning(nullptr, "错误", "还有" + m_data->labels.at(i)->text() + "的价格没有填写！");
            return;
        }
    }

    for (int i = 0; i < m_data->labels.size(); ++i) {
        QLabel* label = m_data->labels.at(i);
        QLineEdit* input = m_data->inputs.at(i);
        QString price = input->text().trimmed();
        m_data->prices[label->text()] = price;
        if (!price.isEmpty()) {
            m_data->yesterdayPrices[label->text()] = price; //这里是反复上一个下一个的时候，保留之前的输入作为参考
        }
    }

    m_data->backingKeys.push_back(m_data->currentUrl);
    makeNextFrame();
}

void CheckPricePopUp::ourPriceChanged(const QString& price)
{
    QString name = sender()->objectName();

    for (int i = 0; i < m_data->data.size(); ++i) {
        if (m_data->data.at(i).at(3).data.toString() == name) {
            m_data->data[i][5].data = price;
            break;
        }
    }
}

void CheckPricePopUp::shippingChanged(const QString& shipping)
{
    m_data->changedShippingMap[sender()->objectName()] = shipping;
}

void CheckPricePopUp::clickTimeOut()
{
    if (!m_data->backingKeys.isEmpty()) {
        m_data->prevBtn->setEnabled(true);
    }
    m_data->nextBtn->setEnabled(true);
}

void CheckPricePopUp::showPopUp(const QMap<QString, QString> &prices, const QList<NamedDataList> &data, const QMap<QString, QString>& yesterdayPrices)
{
    m_data->prices = prices;
    m_data->yesterdayPrices = yesterdayPrices;
    m_data->data = data;

    for (int i = 0; i < data.size(); ++i) {
        NamedDataList dl = data.at(i);
        QString name = dl.at(3).data.toString();
        QString url = dl.at(9).data.toString();

        if (!m_data->prices.keys().contains(name)) {
            if (!m_data->dataMap.keys().contains(url)) {
                m_data->dataMap[url] = QStringList();
            }
            m_data->dataMap[url] << name;
        }
    }

    makeNextFrame();
}

void CheckPricePopUp::makeNextFrame()
{
    if (m_data->backingKeys.size() == m_data->dataMap.size()) {
        QMessageBox::warning(nullptr, "提示", "今日所有价格已经核对完毕！");
        close();
        return;
    }

    m_data->showRemainLabel->setText("剩余" + QString::number(m_data->dataMap.size() - m_data->backingKeys.size()) + "项");

    QString url;
    QStringList names;

    if (m_data->backingKeys.isEmpty()) {
        auto iter = m_data->dataMap.begin();
        url = iter.key();
        names = iter.value();
    }
    else {
        auto iter = m_data->dataMap.find(m_data->backingKeys.back());
        iter++;
        url = iter.key();
        names = iter.value();
    }

    m_data->currentUrl = url;

    if (m_data->frame) {
        m_data->frame->hide();
        m_data->frame->deleteLater();
    }
    m_data->labels.clear();
    m_data->inputs.clear();

    m_data->frame = new QWidget(m_data->scrollArea);

    QVBoxLayout* layout = new QVBoxLayout(m_data->frame);

    QHBoxLayout* shippingLayout = new QHBoxLayout;
    QLabel* shipping = new QLabel(m_data->frame);
    shipping->setText("包邮:  ");
    QComboBox* shippingCombo = new QComboBox(m_data->frame);
    shippingCombo->addItem("是");
    shippingCombo->addItem("否");
    connect(shippingCombo, SIGNAL(currentTextChanged(QString)), this, SLOT(shippingChanged(QString)));
    shippingLayout->addWidget(shipping);
    shippingLayout->addWidget(shippingCombo);
    layout->addLayout(shippingLayout);

    for (int i = 0; i < names.size(); ++i) {
        QLabel* label = new QLabel(m_data->frame);
        label->setText(names.at(i));
        QLineEdit* input = new QLineEdit(m_data->frame);
        input->setValidator(m_data->pattern);

        auto iter = m_data->yesterdayPrices.find(names.at(i));
        if (iter != m_data->yesterdayPrices.end()) {
            input->setText(iter.value());
        }

        QLabel* stock = new QLabel(m_data->frame);
        QPalette palette;
        palette.setBrush(stock->backgroundRole(), Qt::yellow);
        stock->setAutoFillBackground(true);
        stock->setPalette(palette);

        QLabel* price = new QLabel(m_data->frame);
        price->setText("上架价格");

        QLineEdit* priceInput = new QLineEdit(m_data->frame);
        priceInput->setObjectName(names.at(i));
        priceInput->setValidator(m_data->pattern);
        connect(priceInput, SIGNAL(textChanged(QString)), this, SLOT(ourPriceChanged(QString)));
        for (int j = 0; j < m_data->data.size(); ++j) {
            if (m_data->data.at(j).at(3).data.toString() == names.at(i)) {
                if (i == 0) {
                    shippingCombo->setObjectName(m_data->data.at(j).at(2).data.toString()); //商品ID
                    shippingCombo->setCurrentText(m_data->data.at(j).at(6).data.toString());
                }
                priceInput->setText(m_data->data.at(j).at(5).data.toString());
                QString stockNum = m_data->data.at(j).at(10).data.toString();
                if (!stockNum.isEmpty()) {
                    stock->setText("库存:" + stockNum + "  ");
                }
                else {
                    QString barcode = m_data->data.at(j).at(7).data.toString();
                    if (barcode.isEmpty()) {
                        stock->setText("无条码  ");
                    }
                    else {
                        stock->setText("库存:0  ");
                    }
                }
                break;
            }
        }

        QHBoxLayout* hlayout = new QHBoxLayout;
        hlayout->addWidget(label);
        hlayout->addWidget(input);
        hlayout->addWidget(stock);
        hlayout->addWidget(price);
        hlayout->addWidget(priceInput);
        layout->addLayout(hlayout);
        m_data->labels << label;
        m_data->inputs << input;
    }

    m_data->frame->resize(layout->sizeHint());
    m_data->scrollArea->setWidget(m_data->frame);
    resize(m_data->scrollArea->sizeHint().width() + 50, m_data->scrollArea->sizeHint().height() + 100);

    m_data->prevBtn->setEnabled(false);
    m_data->nextBtn->setEnabled(false);
    m_data->timer.start();
}

void CheckPricePopUp::closeEvent(QCloseEvent *)
{
    for (int i = 0; i < m_data->labels.size(); ++i) {
        QLabel* label = m_data->labels.at(i);
        QLineEdit* input = m_data->inputs.at(i);
        QString price = input->text().trimmed();
        if (price.isEmpty()) {
            auto iter = m_data->prices.find(label->text());
            if (iter != m_data->prices.end()) {
                m_data->prices.erase(iter);
            }
        }
    }   //如果退出前清空了璐璐的价格，意思就是下次打开的时候还希望在这里

    emit requestUpdate(m_data->prices, m_data->data, m_data->changedShippingMap);
    //第一个参数，璐璐的价格，如果有值，下次就不会加进来
}
