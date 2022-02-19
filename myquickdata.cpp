#include <QVariant>
#include <QMessageBox>
#include <QDebug>
#include <QFileDialog>
#include <QFileInfo>
#include <QDate>
#include <QDir>
#include <QAxObject>
#include <QFileDialog>
#include "myquickdata.h"
#include "dataeditor.h"
#include "basedefine.h"

class PrivateMyQuickData
{
public:
    QList<NamedDataList>       data;
    QStringList                names;
    QList<int>                 lens;
    QList<StockData>           stockData;
    QList<StockData>           comparingStockData;
    QStringList                brands;
    NamedDataList              lastRecord;
    QMap<QString, QString>     unchangedPriceMap;  //未修改的价格表，每天第一时间生成，作为价格是否变化的依据
    QMap<QString, QString>     unchangedShippingMap;
    QMap<QString, QString>     luluPrice;
    QMap<QString, QString>     yesterdayLuluPrice;
};

MyQuickData::MyQuickData(QObject* parent)
    : QObject(parent)
    , m_data(new PrivateMyQuickData)
{
    m_data->names << "免税品"
                  << "分类/品牌"
                  << "商品ID"
                  << "名称"
                  << "控价"
                  << "上架价格"
                  << "包邮"
                  << "条码"
                  << "沈璐家价格"
                  << "沈璐家链接"
                  << "今日库存"
                  << "对比库存"
                  << "特别备注";

    m_data->lens << 50      //免税品
                 << 120     //分类/品牌
                 << 120     //商品ID
                 << 300     //名称
                 << 50      //控价
                 << 80      //上架价格
                 << 50      //包邮
                 << 120     //条码
                 << 100     //沈璐家价格
                 << 300     //沈璐家链接
                 << 80      //今日库存
                 << 100     //对比库存
                 << 300;    //特别备注
}

MyQuickData::~MyQuickData()
{
    delete m_data;
}

const QList<NamedDataList>& MyQuickData::allData() const
{
    return m_data->data;
}

QList<NamedDataList> MyQuickData::filteredData(const Filter& filter) const
{
    QList<NamedDataList> result;
    for (int i = 0; i < m_data->data.size(); ++i) {
        NamedDataList d = m_data->data.at(i);
        if (filter.taxfree != FILTER_STATE_NOTCARE) {
            bool b = false;
            if (d.at(0).data == "是") {
                b = true;
            }

            if (filter.taxfree == FILTER_STATE_YES && !b) {
                continue;
            }

            if (filter.taxfree == FILTER_STATE_NO && b) {
                continue;
            }
        }

        if (!filter.brand.isEmpty()) {
            if (!d.at(1).data.toString().toLower().contains(filter.brand.toLower())) {
                continue;
            }
        }

        if (!filter.tbId.isEmpty()) {
            if (!d.at(2).data.toString().toLower().contains(filter.tbId.toLower())) {
                continue;
            }
        }

        if (!filter.name.isEmpty()) {
            if (!d.at(3).data.toString().toLower().contains(filter.name.toLower())) {
                continue;
            }
        }

        if (filter.controlledValue != FILTER_STATE_NOTCARE) {
            bool b = false;
            if (!d.at(4).data.toString().isEmpty()) {
                b = true;
            }

            if (filter.controlledValue == FILTER_STATE_YES && !b) {
                continue;
            }

            if (filter.controlledValue == FILTER_STATE_NO && b) {
                continue;
            }
        }

        if (filter.actualValue.method != COMPARE_METHOD_NOTCARE) {
            int v = d.at(5).data.toString().toInt();
            int t = filter.actualValue.target;
            switch (filter.actualValue.method) {
            case COMPARE_METHOD_GREATER:
                if (v <= t) {
                    continue;
                }
                break;
            case COMPARE_METHOD_GREATER_EQUAL:
                if (v < t) {
                    continue;
                }
                break;
            case COMPARE_METHOD_EQUAL:
                if (v != t) {
                    continue;
                }
                break;
            case COMPARE_METHOD_LESS:
                if (v >= t) {
                    continue;
                }
                break;
            case COMPARE_METHOD_LESS_EQUAL:
                if (v > t) {
                    continue;
                }
                break;
            default:
                break;
            }
        }

        if (filter.freeShipping != FILTER_STATE_NOTCARE) {
            bool b = false;
            if (d.at(6).data == "是") {
                b = true;
            }

            if (filter.freeShipping == FILTER_STATE_YES && !b) {
                continue;
            }

            if (filter.freeShipping == FILTER_STATE_NO && b) {
                continue;
            }
        }

        if (!filter.barcode.isEmpty()) {
            if (!d.at(7).data.toString().toLower().contains(filter.barcode.toLower())) {
                continue;
            }
        }

        if (filter.luluHas != FILTER_STATE_NOTCARE) {
            bool b = false;
            if (!d.at(9).data.toString().isEmpty()) {
                b = true;
            }

            if (filter.luluHas == FILTER_STATE_YES && !b) {
                continue;
            }

            if (filter.luluHas == FILTER_STATE_NO && b) {
                continue;
            }
        }

        if (filter.currentStock.method != COMPARE_METHOD_NOTCARE) {
            int v = d.at(10).data.toString().toInt();
            int t = filter.currentStock.target;
            switch (filter.currentStock.method) {
            case COMPARE_METHOD_GREATER:
                if (v <= t) {
                    continue;
                }
                break;
            case COMPARE_METHOD_GREATER_EQUAL:
                if (v < t) {
                    continue;
                }
                break;
            case COMPARE_METHOD_EQUAL:
                if (v != t) {
                    continue;
                }
                break;
            case COMPARE_METHOD_LESS:
                if (v >= t) {
                    continue;
                }
                break;
            case COMPARE_METHOD_LESS_EQUAL:
                if (v > t) {
                    continue;
                }
                break;
            default:
                break;
            }
        }

        if (filter.hasSpAppend != FILTER_STATE_NOTCARE) {
            bool b = false;
            if (!d.at(12).data.toString().isEmpty()) {
                b = true;
            }

            if (filter.hasSpAppend == FILTER_STATE_YES && !b) {
                continue;
            }

            if (filter.hasSpAppend == FILTER_STATE_NO && b) {
                continue;
            }
        }

        result << d;
    }

    return result;
}

const NamedDataList& MyQuickData::lastRecord() const
{
    return m_data->lastRecord;
}

QList<NamedDataList> MyQuickData::priceChangedData() const
{
    QList<NamedDataList> result;
    for (int i = 0; i < m_data->data.size(); ++i) {
        NamedDataList dl = m_data->data.at(i);
        QString name = dl.at(3).data.toString();
        QString price = dl.at(5).data.toString();
        auto iter = m_data->unchangedPriceMap.find(name);
        if (iter != m_data->unchangedPriceMap.end()) {
            if (iter.value() != price) {
                result << dl;
            }
        }
    }
    return result;
}

QList<NamedDataList> MyQuickData::shippingChangedData() const
{
    QList<NamedDataList> result;
    for (int i = 0; i < m_data->data.size(); ++i) {
        NamedDataList dl = m_data->data.at(i);
        QString name = dl.at(3).data.toString();
        QString shipping = dl.at(6).data.toString();
        auto iter = m_data->unchangedShippingMap.find(name);
        if (iter != m_data->unchangedShippingMap.end()) {
            if (iter.value() != shipping) {
                result << dl;
            }
        }
    }
    return result;
}

QList<NamedDataList> MyQuickData::withoutBarcodeData() const
{
    QList<NamedDataList> result;
    for (int i = 0; i < m_data->data.size(); ++i) {
        NamedDataList dl = m_data->data.at(i);
        if (dl.at(7).data.toString().isEmpty()) {
            result << dl;
        }
    }
    return result;
}

QList<ColumnDetail> MyQuickData::columnDetails() const
{
    QList<ColumnDetail> dl;
    for (int i = 0; i < m_data->names.size(); ++i) {
        ColumnDetail d;
        d.name = m_data->names.at(i);
        d.width = m_data->lens.at(i);
        dl << d;
    }
    return dl;
}

const QList<int>& MyQuickData::columnWidth() const
{
    return m_data->lens;
}

QStringList MyQuickData::brands()
{
    if (!m_data->brands.isEmpty()) {
        return m_data->brands;
    }

    for (int i = 0; i < m_data->data.size(); ++i) {
        NamedDataList dl = m_data->data.at(i);
        QString brand = dl.at(1).data.toString();
        if (!m_data->brands.contains(brand)) {
            m_data->brands.push_back(brand);
        }
    }
    return m_data->brands;
}

bool MyQuickData::open()
{
    QFile* fi = new QFile("data.dat");
    if (!fi->exists()) {
        QMessageBox::warning(nullptr, "提示", "未发现数据文件data.dat，即将新建。");
    }
    if(!fi->open(QFile::ReadOnly)) {
        QMessageBox::warning(nullptr, "错误", "打开文件data.dat失败！");
        return false;
    }

    if (0 == fi->bytesAvailable()) {
        //新建
        fi->deleteLater();
        return true;
    }

    loadAllData(fi);
    fi->deleteLater();

    QDate date = QDate::currentDate();
    QString stockFileName = "stock/" + QString::number(date.year()) + "-" + QString::number(date.month()) + "-" + QString::number(date.day()) + ".stock";
    QFile stockfi(stockFileName);

    if (stockfi.exists()) {
        stockfi.open(QFile::ReadOnly);

        char buf[1024];
        qint64 lineLength = stockfi.readLine(buf, sizeof(buf));
        while (-1 != lineLength) {
            QString s(buf);
            QStringList sl = s.split("|||");

            StockData sd;
            sd.barcode = sl.at(0).trimmed();
            sd.number = sl.at(1).trimmed();

            m_data->stockData << sd;

            lineLength = stockfi.readLine(buf, sizeof(buf));
        }

        loadStock();
//        QMessageBox::warning(nullptr, "提示", "已读取本日库存！");
    }
    else {
        QMessageBox::warning(nullptr, "警告", "本日库存尚未创建！请尽快创建！");
    }

    QString priceFileName = "price/" + QString::number(date.year()) + "-" + QString::number(date.month()) + "-" + QString::number(date.day()) + ".price";
    QFile priceFi(priceFileName);
    if (!priceFi.exists()) {
        QDir dir("price");
        if (!dir.exists()) {
            dir.mkpath(dir.currentPath() + "/price");
        }

        for (int i = 0; i < m_data->data.size(); ++i) {
            NamedDataList dl = m_data->data.at(i);
            m_data->unchangedPriceMap[dl.at(3).data.toString()] = dl.at(5).data.toString();
            m_data->unchangedShippingMap[dl.at(3).data.toString()] = dl.at(6).data.toString();
        }

        saveUnchangedPriceMap();
        QMessageBox::warning(nullptr, "提示", "已经生成本日价格基础文件:" + priceFileName + "！");
    }
    else {
        priceFi.open(QFile::ReadOnly);
        char buf[1024];
        qint64 lineLength = priceFi.readLine(buf, sizeof(buf));
        while (-1 != lineLength) {
            QString s(buf);
            QStringList sl = s.split("|||");

            m_data->unchangedPriceMap[sl.at(0).trimmed()] = sl.at(1).trimmed();
            m_data->unchangedShippingMap[sl.at(0).trimmed()] = sl.at(2).trimmed();

            lineLength = priceFi.readLine(buf, sizeof(buf));
        }
    }

    QString luluPriceFileName = "price/" + QString::number(date.year()) + "-" + QString::number(date.month()) + "-" + QString::number(date.day()) + ".lulu";
    QFile luluPriceFi(luluPriceFileName);

    if (!luluPriceFi.exists()) {
        QMessageBox::warning(nullptr, "警告", "今日尚未开始核对璐璐家的价格，请尽快开始！");
    }
    else {
        luluPriceFi.open(QFile::ReadOnly);
        char buf[1024];
        qint64 lineLength = luluPriceFi.readLine(buf, sizeof(buf));
        while (-1 != lineLength) {
            QString s(buf);
            QStringList sl = s.split("|||");

            m_data->luluPrice[sl.at(0).trimmed()] = sl.at(1).trimmed();

            lineLength = luluPriceFi.readLine(buf, sizeof(buf));
        }
        loadLuluPrice();
    }

    date = date.addDays(-1);
    QString yesterdayLuluPriceFileName = "price/" + QString::number(date.year()) + "-" + QString::number(date.month()) + "-" + QString::number(date.day()) + ".lulu";
    QFile yesterdayLuluPriceFi(yesterdayLuluPriceFileName);

    int yesterdayLuluPriceCount = 0;
    while (!yesterdayLuluPriceFi.exists()) {
        date = date.addDays(-1);
        yesterdayLuluPriceFileName = "price/" + QString::number(date.year()) + "-" + QString::number(date.month()) + "-" + QString::number(date.day()) + ".lulu";
        yesterdayLuluPriceFi.setFileName(yesterdayLuluPriceFileName);
        yesterdayLuluPriceCount += 1;

        if (yesterdayLuluPriceCount > 6) {
            QMessageBox::warning(nullptr, "警告", "一周内都没有璐璐家的价格数据！");
            break;
        }
    }

    if (yesterdayLuluPriceFi.exists()) {
        yesterdayLuluPriceFi.open(QFile::ReadOnly);
        char buf[1024];
        qint64 lineLength = yesterdayLuluPriceFi.readLine(buf, sizeof(buf));
        while (-1 != lineLength) {
            QString s(buf);
            QStringList sl = s.split("|||");

            m_data->yesterdayLuluPrice[sl.at(0).trimmed()] = sl.at(1).trimmed();

            lineLength = yesterdayLuluPriceFi.readLine(buf, sizeof(buf));
        }
    }

    return true;
}

void MyQuickData::save()
{
    QFile* fi = new QFile("data.dat");
    fi->open(QFile::WriteOnly);
    fi->seek(0);

    for (int i = 0; i < m_data->data.size(); ++i) {
        NamedDataList d = m_data->data.at(i);
        QString line;
        for (int j = 0; j < d.size() - 3; ++j) { //库存数据不用保存, 特别备注放在最后一个，跳过沈璐家价格
            if (j != 8) {
                line += d.at(j).data.toString();
                line += "|||";
            }
        }

        line += d.back().data.toString(); //加入特别备注

        if (i < m_data->data.size() - 1) {
            line += "\n";
        }
        fi->write(line.toUtf8());
    }

    fi->flush();
    fi->deleteLater();
    QMessageBox::warning(nullptr, "提示", "已保存！");
}

void MyQuickData::add(const NamedDataList& data)
{
    if (!checkValid(data)) {
        QMessageBox::warning(nullptr, "错误", "数据不完整！");
        return;
    }

    if (checkDuplicate(data)) {
        QMessageBox::warning(nullptr, "错误", "已经存在该名称的货品！");
        return;
    }

    NamedDataList dl = data;
    compleData(dl);

    m_data->data << dl;

    if (!m_data->brands.contains(data.at(1).data.toString())) {
        m_data->brands.push_back(data.at(1).data.toString());
    }

    m_data->lastRecord = data; //不要包含库存数据

    emit dataChanged();
}

bool MyQuickData::drop(const NamedDataList& data)
{
    if (!checkValid(data)) {
        QMessageBox::warning(nullptr, "错误", "数据不完整！");
        return false;
    }

    for (int i = 0; i < m_data->data.size(); ++i) {
        NamedDataList d = m_data->data.at(i);
        if (!data.at(7).data.toString().isEmpty() && d.at(7).data == data.at(7).data) {
            m_data->data.removeAt(i);
            emit dataChanged();
            return true;
        }
        if (!data.at(3).data.toString().isEmpty() && d.at(3).data == data.at(3).data) {
            m_data->data.removeAt(i);
            emit dataChanged();
            return true;
        }
    }

    return false;
}

void MyQuickData::update(const NamedDataList& oldData, const NamedDataList& newData)
{
    if (oldData.at(3).data.toString() != newData.at(3).data.toString()) {
        QMessageBox::warning(nullptr, "错误", "名称不一样，不能执行更新！");
        return;
    }

    if (!checkValid(oldData) || !checkValid(newData)) {
        QMessageBox::warning(nullptr, "错误", "数据不完整！");
        return;
    }

    NamedDataList dl = newData;
    compleData(dl);

    bool found = false;
    for (int i = 0; i < m_data->data.size(); ++i) {
        NamedDataList d = m_data->data.at(i);
        if (!oldData.at(3).data.toString().isEmpty() && d.at(3).data == oldData.at(3).data) {
            m_data->data[i] = dl;
            found = true;
            break;
        }
    }

    if (!found) {
        QMessageBox::warning(nullptr, "错误", "没有找到名称[" + oldData.at(3).data.toString() + "]的数据！");
    }
    else {
        if (!m_data->brands.contains(dl.at(1).data.toString())) {
            m_data->brands.push_back(dl.at(1).data.toString());
        }

        for (int i = 0; i < m_data->data.size(); ++i) {  //同一商品ID，包邮一致化，品牌/分类一致化
            NamedDataList dl = m_data->data.at(i);
            if (dl.at(2).data.toString() == newData.at(2).data.toString()) {
                m_data->data[i][6].data = newData.at(6).data;
                m_data->data[i][0].data = newData.at(0).data;
                m_data->data[i][1].data = newData.at(1).data;
            }
        }

        emit dataChanged();
    }
}

void MyQuickData::insert(const NamedDataList &current, const NamedDataList &newData)
{
    if (!checkValid(current) || !checkValid(newData)) {
        QMessageBox::warning(nullptr, "错误", "数据不完整！");
        return;
    }

    if (checkDuplicate(newData)) {
        QMessageBox::warning(nullptr, "错误", "已经存在该名称的货品！");
        return;
    }

    NamedDataList dl = newData;
    m_data->lastRecord = newData;
    compleData(dl);

    bool found = false;
    for (int i = 0; i < m_data->data.size(); ++i) {
        NamedDataList d = m_data->data.at(i);
        if (!current.at(3).data.toString().isEmpty() && d.at(3).data == current.at(3).data) {
            m_data->data.insert(i+1, dl);
            found = true;
            break;
        }
    }

    if (!found) {
        QMessageBox::warning(nullptr, "错误", "没有找到名称[" + current.at(3).data.toString() + "]的数据！");
    }
    else {
        if (!m_data->brands.contains(dl.at(1).data.toString())) {
            m_data->brands.push_back(dl.at(1).data.toString());
        }
        emit dataChanged();
    }
}

void MyQuickData::moveBefore(const NamedDataList &source, const NamedDataList &target)
{
    QString sourceName = source.at(3).data.toString();
    QString targetName = target.at(3).data.toString();

    if (sourceName == targetName) {
        return;
    }

    int sourceIndex = -1;
    int targetIndex = -1;
    NamedDataList sourceDl;
    for (int i = 0;  i < m_data->data.size(); ++i) {
        QString name = m_data->data.at(i).at(3).data.toString();
        if (name == sourceName) {
            sourceIndex = i;
            sourceDl = m_data->data.at(i);
        }
        else if (name == targetName) {
            targetIndex = i;
        }
    }

    if (-1 == sourceIndex || -1 == targetIndex || sourceIndex == targetIndex-1) {
        return;
    }

    m_data->data.insert(targetIndex, sourceDl);

    if (sourceIndex > targetIndex) {
        m_data->data.removeAt(sourceIndex+1);
    }
    else {
        m_data->data.removeAt(sourceIndex);
    }

    emit dataChanged();
}

void MyQuickData::moveAfter(const NamedDataList &source, const NamedDataList &target)
{
    QString sourceName = source.at(3).data.toString();
    QString targetName = target.at(3).data.toString();

    if (sourceName == targetName) {
        return;
    }

    int sourceIndex = -1;
    int targetIndex = -1;
    NamedDataList sourceDl;
    for (int i = 0;  i < m_data->data.size(); ++i) {
        QString name = m_data->data.at(i).at(3).data.toString();
        if (name == sourceName) {
            sourceIndex = i;
            sourceDl = m_data->data.at(i);
        }
        else if (name == targetName) {
            targetIndex = i;
        }
    }

    if (-1 == sourceIndex || -1 == targetIndex || sourceIndex == targetIndex+1) {
        return;
    }

    m_data->data.insert(targetIndex+1, sourceDl);

    if (sourceIndex < targetIndex) {
        m_data->data.removeAt(sourceIndex);
    }
    else {
        m_data->data.removeAt(sourceIndex+1);
    }

    emit dataChanged();
}

void MyQuickData::readStockData()
{
    QDir dir("stock");
    if (!dir.exists()) {
        dir.mkpath(dir.currentPath() + "/stock");
    }

    QDate date = QDate::currentDate();
    QString stockFileName = "stock/" + QString::number(date.year()) + "-" + QString::number(date.month()) + "-" + QString::number(date.day()) + ".stock";
    QFile fi(stockFileName);

    if (fi.exists()) {
        QMessageBox::warning(nullptr, "注意", "文件" + stockFileName + "已存在！如果需要重新制作请手动删除后重新尝试。");
        return;
    }

    QString fileName = QFileDialog::getOpenFileName(nullptr, "打开Excel文件",
                                                    QDir::currentPath(),
                                                    "Excel文件 (*.xls *.xlsx)");

    if (fileName.isEmpty()) {
        return;
    }

    QAxObject axobj("Excel.Application");
    auto workbooks = axobj.querySubObject("Workbooks");
    auto workbook  = workbooks->querySubObject("Open(QString, QVariant)", fileName);
    auto worksheet = workbook->querySubObject("WorkSheets(int)", 1);
    auto usedrange = worksheet->querySubObject("UsedRange");
    auto rows = usedrange->querySubObject("Rows");

    int intRows = rows->property("Count").toInt();

    QString range = "B2:G" +QString::number(intRows);
    QAxObject *allEnvData = worksheet->querySubObject("Range(QString)", range);
    QVariant allEnvDataQVariant = allEnvData->property("Value");
    QVariantList allEnvDataList = allEnvDataQVariant.toList();

    m_data->stockData.clear();

    for(int i=0; i< intRows-1; i++)
    {
        QVariantList allEnvDataList_i =  allEnvDataList[i].toList();
        QString barcode = allEnvDataList_i[0].toString(); //第i行第0列的数据
        QString number = allEnvDataList_i[5].toString();

        StockData sd;
        sd.barcode = barcode;
        sd.number = number;
        m_data->stockData << sd;
    }

    workbooks->dynamicCall("Close()");
    axobj.dynamicCall("Quit()");

    fi.open(QFile::WriteOnly);
    fi.seek(0);

    for (int i = 0; i < m_data->stockData.size(); ++i) {
        StockData d = m_data->stockData.at(i);
        QString line = d.barcode + "|||" + d.number + "\n";
        fi.write(line.toUtf8());
    }

    fi.flush();
    loadStock();
    QMessageBox::warning(nullptr, "提示", "已经成功创建" + stockFileName + "！");
}

const QMap<QString, QString>& MyQuickData::getLuluPrice() const
{
    return m_data->luluPrice;
}

void MyQuickData::updateLuluPrice(const QMap<QString, QString> &prices, const QList<NamedDataList>& data, const QMap<QString, QString>& changedShippingMap)
{
    for (int i = 0; i < data.size(); ++i) {
        QString name = data.at(i).at(3).data.toString();

        for (int j = 0; j < m_data->data.size(); ++j) {
            if (m_data->data.at(j).at(3).data.toString() == name) {
                m_data->data[j][5].data = data[i][5].data;
                m_data->data[j][6].data = data[i][6].data;
                break;
            }
        }
    }

    for (auto iter = changedShippingMap.begin(); iter != changedShippingMap.end(); ++iter) {
        QString tbId = iter.key();
        for (int i = 0; i < m_data->data.size(); ++i) {
            if (m_data->data.at(i).at(2).data.toString() == tbId) {
                m_data->data[i][6].data = iter.value();
            }
        }
    }

    m_data->luluPrice = prices;

    QDate date = QDate::currentDate();
    QString luluPriceFileName = "price/" + QString::number(date.year()) + "-" + QString::number(date.month()) + "-" + QString::number(date.day()) + ".lulu";
    QFile luluPriceFi(luluPriceFileName);

    QDir dir("price");
    if (!dir.exists()) {
        dir.mkpath(dir.currentPath() + "/price");
    }

    luluPriceFi.open(QFile::WriteOnly);

    QString result;
    for (auto iter = prices.begin(); iter != prices.end(); ++iter) {
        result += iter.key() + "|||" + iter.value() + '\n';
    }
    result = result.left(result.size()-1); //去掉最后一个回车
    luluPriceFi.seek(0);
    luluPriceFi.write(result.toUtf8());
    luluPriceFi.flush();

    loadLuluPrice();
}

const QMap<QString, QString>& MyQuickData::getYesterdayLuluPrice() const
{
    return m_data->yesterdayLuluPrice;
}

void MyQuickData::setStockComparingDays(int days)
{
    QDate date = QDate::currentDate();
    date = date.addDays(-days);

    QString stockFileName = "stock/" + QString::number(date.year()) + "-" + QString::number(date.month()) + "-" + QString::number(date.day()) + ".stock";
    QFile stockfi(stockFileName);

    if (stockfi.exists()) {
        stockfi.open(QFile::ReadOnly);
        m_data->comparingStockData.clear();

        char buf[1024];
        qint64 lineLength = stockfi.readLine(buf, sizeof(buf));
        while (-1 != lineLength) {
            QString s(buf);
            QStringList sl = s.split("|||");

            StockData sd;
            sd.barcode = sl.at(0).trimmed();
            sd.number = sl.at(1).trimmed();

            m_data->comparingStockData << sd;

            lineLength = stockfi.readLine(buf, sizeof(buf));
        }

        for (int i = 0; i < m_data->data.size(); ++i) {
            NamedDataList dl = m_data->data.at(i);
            QString barcode = dl.at(7).data.toString();
            if (!barcode.isEmpty()) {
                for (int j = 0; j < m_data->comparingStockData.size(); ++j) {
                    StockData d1 = m_data->comparingStockData.at(j);
                    if (d1.barcode == barcode) {
                        m_data->data[i][11].data = d1.number;
                    }
                }
            }
        }

        m_data->names[11] = QString::number(days) + "日前库存";
    }
    else {
        QMessageBox::warning(nullptr, "警告", "未找到" + QString::number(days) + "天前的库存文件！");
    }
}

void MyQuickData::renameData(const QString &oldName, const QString &newName)
{
    for (int i = 0; i < m_data->data.size(); ++i) {
        NamedDataList dl = m_data->data.at(i);
        if (dl.at(3).data.toString() == newName) {
            QMessageBox::warning(nullptr, "错误", "名称[" + newName + "]已经存在！");
            return;
        }
    }

    bool found = false;
    NamedDataList sourceDl;
    NamedDataList targetDl;
    for (int i = 0; i < m_data->data.size(); ++i) {
        NamedDataList dl = m_data->data.at(i);
        if (dl.at(3).data.toString() == oldName) {
            found = true;
            m_data->data[i][3].data = newName;
            sourceDl = dl;
            targetDl = m_data->data.at(i);
            break;
        }
    }

    if (!found) {
        QMessageBox::warning(nullptr, "错误", "名称[" + oldName + "]没有发现！");
        return;
    }

    auto iter = m_data->unchangedPriceMap.find(oldName);
    if (iter != m_data->unchangedPriceMap.end()) {
        m_data->unchangedPriceMap[newName] = iter.value();
        m_data->unchangedPriceMap.erase(iter);

        auto iter_1 = m_data->unchangedShippingMap.find(oldName);
        m_data->unchangedShippingMap[newName] = iter_1.value();
        m_data->unchangedShippingMap.erase(iter_1);

        saveUnchangedPriceMap();
    }

    auto iter_1 = m_data->luluPrice.find(oldName);
    if (iter_1 != m_data->luluPrice.end()) {
        m_data->luluPrice[newName] = iter_1.value();
        m_data->luluPrice.erase(iter_1);
        updateLuluPrice(m_data->luluPrice, QList<NamedDataList>(), QMap<QString, QString>());
    }

    QString sourceLine;
    QString targetLine;
    for (int i = 0; i < sourceDl.size() - 3; ++i) { //库存数据跳过, 特别备注放在最后一个，跳过沈璐家价格
        if (i != 8) {
            sourceLine += sourceDl.at(i).data.toString();
            sourceLine += "|||";

            targetLine += targetDl.at(i).data.toString();
            targetLine += "|||";
        }
    }

    sourceLine += sourceDl.back().data.toString(); //加入特别备注
    targetLine += targetDl.back().data.toString();

    QString allData;

    {
        QFile fi("data.dat");
        fi.open(QFile::ReadOnly);
        allData = fi.readAll();
        allData.replace(sourceLine, targetLine);
    }

    {
        QFile fi("data.dat");
        fi.open(QFile::WriteOnly);
        fi.seek(0);
        fi.write(allData.toUtf8());
        fi.flush();
        emit renameSuccessful(oldName, newName);
    }
}

void MyQuickData::loadAllData(QFile *fi)
{
    char buf[1024];
    qint64 lineLength = fi->readLine(buf, sizeof(buf));
    int lineCount = 0;
    while (-1 != lineLength) {
        QString s(buf);
        QStringList sl = s.split("|||");

        if (sl.size() != m_data->names.size() -3) {  //两个库存单独读库存文件,沈璐家价格单独存
            QString errorString;
            for (int x = 0; x < sl.size(); ++x) {
                errorString += sl.at(x);
                if (x < sl.size() - 1) {
                    errorString += ",";
                }
            }
            QMessageBox::warning(nullptr, "错误", "发现数据有效性问题:" + errorString + "在行[" + QString::number(lineCount) + "]");
        }

        NamedDataList dl;
        for (int i = 0; i < sl.size()-1; ++i) { //特别备注加在库存后面
            NamedData d;
            d.data = sl.at(i).trimmed();
            dl << d;

            if (i == 7) {
                dl << NamedData(); //为沈璐家价格加入一个空的占位符
            }
        }

        dl[7].priority = 7; //条码优先级最高
        dl[3].priority = 6; //名称其次

        dl << NamedData() << NamedData(); //两个空的给库存占位

        NamedData dappend;  //最后加入特别备注
        dappend.data = sl.back().trimmed();
        dl << dappend;

        m_data->data << dl;

        lineLength = fi->readLine(buf, sizeof(buf));
        lineCount += 1;
    }
}

void MyQuickData::loadStock()
{
    for (int i = 0; i < m_data->data.size(); ++i) {
        NamedDataList dl = m_data->data.at(i);
        QString barcode = dl.at(7).data.toString();
        if (!barcode.isEmpty()) {
            QString number = findStock(barcode);
            m_data->data[i][10].data = number;
        }
    }
}

void MyQuickData::loadLuluPrice()
{
    for (int i = 0; i < m_data->data.size(); ++i) {
        NamedDataList dl = m_data->data.at(i);
        QString name = dl.at(3).data.toString();
        if (m_data->luluPrice.keys().contains(name)) {
            m_data->data[i][8].data = m_data->luluPrice[name];
        }
    }
}

QString MyQuickData::findStock(const QString &barcode)
{
    for (int i = 0; i < m_data->stockData.size(); ++i) {
        StockData d = m_data->stockData.at(i);
        if (d.barcode == barcode) {
            return d.number;
        }
    }
    return QString();
}

bool MyQuickData::checkValid(const NamedDataList &data)
{
    if (data.size() != m_data->names.size()) {
        return false;
    }

    if (data.at(3).data.toString().isEmpty()) {
        return false;
    }

    return true;
}

bool MyQuickData::checkDuplicate(const NamedDataList &data)
{
    for (int i = 0; i < m_data->data.size(); ++i) {
        NamedDataList d = m_data->data.at(i);
        if (!d.at(3).data.toString().isEmpty() && d.at(3).data == data.at(3).data) {
            return true;
        }
    }
    return false;
}

void MyQuickData::compleData(NamedDataList &data)
{
    QString barcode = data.at(7).data.toString();
    for (int i = 0; i < m_data->stockData.size(); ++i) {
        StockData d = m_data->stockData.at(i);
        if (d.barcode == barcode) {
            data[10].data = d.number;
        }
    }

    QString name = data.at(3).data.toString();
    if (m_data->luluPrice.keys().contains(name)) {
        data[8].data = m_data->luluPrice[name];
    }

    for (int j = 0; j < m_data->comparingStockData.size(); ++j) {
        StockData d1 = m_data->comparingStockData.at(j);
        if (d1.barcode == barcode) {
            data[11].data = d1.number;
        }
    }
}

void MyQuickData::saveUnchangedPriceMap()
{
    QDate date = QDate::currentDate();
    QString priceFileName = "price/" + QString::number(date.year()) + "-" + QString::number(date.month()) + "-" + QString::number(date.day()) + ".price";
    QFile priceFi(priceFileName);

    priceFi.open(QFile::WriteOnly);
    QString allText;
    for (auto iter = m_data->unchangedPriceMap.begin(); iter != m_data->unchangedPriceMap.end(); ++iter) {
        allText += iter.key() + "|||" + iter.value() + "|||";
        auto iter_1 = m_data->unchangedShippingMap.find(iter.key());
        allText += iter_1.value() + '\n';
    }
    allText = allText.left(allText.size()-1); //去掉最后一个回车
    priceFi.seek(0);
    priceFi.write(allText.toUtf8());
    priceFi.flush();
}
