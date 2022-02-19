#include <QLineEdit>
#include <QLabel>
#include <QHBoxLayout>
#include <QRegExpValidator>
#include <QPushButton>
#include <QRadioButton>
#include "stockcomparingsettingpopup.h"

class PrivateStockComparingSettingPopUp
{
public:
    QLineEdit*      input;
    QRadioButton*   radioButton;
};

StockComparingSettingPopUp::StockComparingSettingPopUp(QWidget* parent)
    : QDialog(parent)
    , m_data(new PrivateStockComparingSettingPopUp)
{
    QVBoxLayout* layout = new QVBoxLayout;
    QHBoxLayout* layout1 = new QHBoxLayout;

    QLabel* label = new QLabel(this);
    label->setText("设定对比库存为");

    m_data->input = new QLineEdit(this);
    QRegExp regExp("[0-9]+$");
    QRegExpValidator *pattern = new QRegExpValidator(regExp, this);
    m_data->input->setValidator(pattern);

    QLabel* label2 = new QLabel(this);
    label2->setText("天");

    QPushButton* button = new QPushButton(this);
    button->setText("确定");

    layout1->addWidget(label);
    layout1->addWidget(m_data->input);
    layout1->addWidget(label2);
    layout1->addWidget(button);

    m_data->radioButton = new QRadioButton(this);
    QLabel* label3 = new QLabel(this);
    label3->setText("未变化的库存涂成红色");
    QHBoxLayout* layout2 = new QHBoxLayout;
    layout2->addStretch();
    layout2->addWidget(m_data->radioButton);
    layout2->addWidget(label3);
    layout2->addStretch();

    layout->addLayout(layout1);
    layout->addLayout(layout2);

    setLayout(layout);

    connect(button, SIGNAL(clicked()), this, SLOT(btnClicked()));
}

StockComparingSettingPopUp::~StockComparingSettingPopUp()
{
    delete m_data;
}

void StockComparingSettingPopUp::btnClicked()
{
    QString days = m_data->input->text().trimmed();
    if (!days.isEmpty()) {
        emit comparingStockDays(days.toInt(), m_data->radioButton->isChecked());
    }
    close();
}
