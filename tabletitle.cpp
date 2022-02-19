#include <QPainter>
#include "tabletitle.h"

class PrivateTableTitle
{
public:
    int     h_position;
    QList<ColumnDetail>     columnDetails;
};

TableTitle::TableTitle(QWidget *parent)
    : QWidget(parent)
    , m_data(new PrivateTableTitle)
{
    m_data->h_position = 0;

    QFont f = font();
    f.setBold(true);
    setFont(f);
}

TableTitle::~TableTitle()
{
    delete m_data;
}

void TableTitle::setColumnDetails(const QList<ColumnDetail> &details)
{
    m_data->columnDetails = details;
    update();
}

void TableTitle::hvalueChanged(int value)
{
    m_data->h_position = value;
    update();
}

void TableTitle::paintEvent(QPaintEvent *)
{
    if (m_data->columnDetails.isEmpty()) {
        return;
    }

    QPainter painter(this);

    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(227, 190, 18, 150));
    painter.drawRect(rect());

    QPen gridPen;
    gridPen.setColor(QColor(57, 57, 57, 80));
    gridPen.setWidth(2);
    painter.setPen(gridPen);

    QPen textPen;
    textPen.setColor(QColor(5, 5, 5));

    QRect r1(-m_data->h_position, 0, m_data->columnDetails.at(0).width, height());
    painter.drawRect(r1);
    painter.setPen(textPen);
    painter.drawText(r1, Qt::AlignCenter, m_data->columnDetails.at(0).name);

    int startX = -m_data->h_position + r1.width();
    for (int i = 1; i < m_data->columnDetails.size(); ++i) {
        QRect r(startX, 0, m_data->columnDetails.at(i).width, height());
        painter.setPen(gridPen);
        painter.drawRect(r);
        painter.setPen(textPen);
        painter.drawText(r, Qt::AlignCenter, m_data->columnDetails.at(i).name);
        startX += r.width();
    }
}
