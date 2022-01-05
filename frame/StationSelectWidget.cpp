#include "StationSelectWidget.h"
#include "ui_StationSelectWidget.h"
#include "DataCenter.h"
#include "LineStations.h"
#include "StationListWidget.h"

StationSelectWidget::StationSelectWidget(QWidget *parent) :
    WidgetBase(parent),
    ui(new Ui::StationSelectWidget)
{
    ui->setupUi(this);

#if IS_TEST_MODE
//    setTestData();
#endif

    init();
}

StationSelectWidget::~StationSelectWidget()
{
    delete ui;
}

bool StationSelectWidget::showData()
{
    // 检查数据更新 TODO:
//    if (m_dataUpdateNum[LINE_STATION_LIST - 1])
    {
        m_dataUpdateNum[LINE_STATION_LIST - 1] = false;
        // 加载数据
        m_tabWidget->clear();
        QList<LineStations*> lines = DataCenter::getThis()->getLineStations();

        for(LineStations* line:lines) {
            QString lineName = line->getName();
            StationListWidget* stationWidget = new StationListWidget(lineName, line->stationList(), this);
            connect(stationWidget, &StationListWidget::selected, this, &StationSelectWidget::onSelected);
            m_tabWidget->addTab(stationWidget, lineName);
        }

        show();
        return true;
    }

//    return false;
}

void StationSelectWidget::setStyle()
{
    QHBoxLayout *layoutWnd = new QHBoxLayout();
    layoutWnd->setMargin(0);
    layoutWnd->setSpacing(0);

    m_tabWidget = new CustomTabWidget(this);
    m_tabWidget->setTabPosition(QTabWidget::TabPosition::West);
    layoutWnd->addWidget(m_tabWidget);
    ui->frame->setLayout(layoutWnd);


    this->setAttribute(Qt::WA_TranslucentBackground, true);
    this->setWindowFlags(Qt::FramelessWindowHint);
//    QScreen *screen = QGuiApplication::primaryScreen();   //获取当前屏幕的大小
//    QRect mm = screen->availableGeometry();
//    int screenWidth = mm.width();
//    int screenHeight = mm.height();

//    this->setFixedSize(screenWidth, screenHeight);

    int screenWidth = 1920;
    int screenHeight = 1080;
    QList<QScreen *> listScreen =  QGuiApplication::screens();
    if (listScreen.size() > 0) {
        QScreen *screen = listScreen.at(0);
        QRect rect = screen->geometry();
        screenWidth = rect.width();
        screenHeight = rect.height();
    }
    this->setFixedSize(screenWidth, screenHeight);

}

void StationSelectWidget::init()
{
    setStyle();
    connect(ui->closeBtn, &QPushButton::clicked, this, &StationSelectWidget::onClose);
}

void StationSelectWidget::setTestData()
{
    QList<LineStations*> lineList;
//    // 测试数据
//    LineStations* line1 = new LineStations("1号线", "", "images/info/line1.png", "#F01C4B");
//    line1->addStations(QList<QString>() <<
//                       "双港站" << "孔目湖站" << "长江路站" << "珠江路站" << "庐山南大道站" <<
//                       "绿茵路站" << "卫东站" << "地铁大厦站" << "秋水广场站" << "滕王阁站" <<
//                       "万寿宫站" << "八一馆站" << "八一广场站" << "丁公路北站" << "师大南路站" <<
//                       "彭家桥站" << " 谢家村站" << "青山湖大道站" << "高新大道站" << "艾溪湖西站" <<
//                       "艾溪湖东站" << "太子殿站" << "奥体中心站" << " 瑶湖西站");

//    LineStations* line2 = new LineStations("2号线", "",  "images/info/line2.png", "#FFC931");
//    line2->addStations(QList<QString>() <<
//                       "南路站" << "大岗站" << "生米站" << "九龙湖南站" << "市民中心站" << "鹰潭街站" <<
//                       "集嘉坊站" << "南广场站" << "站前南大道站" << "南昌西站" << "龙岗站" <<
//                       "国体中心站" << "卧龙山站" << "岭北三路站" << "岭北站" << "学府大道东站" <<
//                       "翠苑路站" << "地铁大厦站" << "雅苑路站" << "红谷中大道站" << "阳明公园站" <<
//                       "墩子塘站" << "福州路站" << "八一广场站" << "永叔路站" << "丁公路南站" <<
//                       "南昌火车站" << "顺外站" <<  "辛家庵站" << "上海南路" << "新溪桥路" <<
//                       "城南中心" << "昌南大市场" << "罗家集" << "南昌东站");

//    LineStations* line3 = new LineStations("3号线", "", "images/info/line3.png", "#1E5789");
//    line3->addStations(QList<QString>() <<
//                       "银三角北" << "斗门" << "斗柏路" << "沥山" << "振兴大道" << "岱山" <<
//                       "八大山人" << "蔡家坊" << "江铃" << "京家山" << "十字街" << "绳金塔" <<
//                       "六眼井" << "墩子塘" << "上沙沟" << "青山湖西" << "国威路" << "火炬广场" <<
//                       "梁万" << "京东大道");

//    lineList.append(line1);
//    lineList.append(line2);
//    lineList.append(line3);

    DataCenter::getThis()->setLineStations(lineList);
}

void StationSelectWidget::onClose()
{
    this->close();
}

void StationSelectWidget::onSelected(QString lineName, QString stationName, QString stationCode)
{
    emit stationSelected(lineName, stationName, stationCode);
    this->close();
}
