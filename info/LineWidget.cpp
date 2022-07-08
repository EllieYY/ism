#include "LineWidget.h"
#include "ui_LineWidget.h"
#include "CommonHead.h"
#include "LineInfo.h"
#include "DataCenter.h"
#include "LineStations.h"
#include "Station.h"
#include "HttpTool.h"

#include <QVBoxLayout>
#include <QComboBox>
#include <QListView>

#define LINE_ALL_ID  60

LineWidget::LineWidget(QWidget *parent) :
    WidgetBase(parent),
    ui(new Ui::LineWidget)
{
    ui->setupUi(this);

    init();
}

LineWidget::~LineWidget()
{
    delete ui;
}

void LineWidget::init()
{
    m_layout = NULL;
    ui->widget_query->hide();
    ui->lineFrame3->show();
//    ui->label_image->setText("");
//    ui->label_image->hide();
//    QString style = "background-color:white;";
//    ui->label_image->setStyleSheet(style);
//    ui->lineFrame3->setStyleSheet(style);


    ui->comboBox_line_e->setView(new QListView());
    ui->comboBox_line_s->setView(new QListView());
    ui->comboBox_station_e->setView(new QListView());
    ui->comboBox_station_s->setView(new QListView());

    connect(ui->pushButton_lineQuery, &QPushButton::clicked, this, &LineWidget::lineQuery);
    connect(HttpTool::getThis(), &HttpTool::lineInfoReceived, this, &LineWidget::onLineQuery);


    // 获取下拉列表 - 线路
    connect(ui->comboBox_line_s, SIGNAL(currentIndexChanged(QString)), this, SLOT(onStartLineSelect(QString)));
    connect(ui->comboBox_line_e, SIGNAL(currentIndexChanged(QString)), this, SLOT(onEndLineSelect(QString)));


    ui->comboBox_line_s->clear();
    ui->comboBox_line_e->clear();

    QList<LineInfo *> lineList = DataCenter::getThis()->getLineList();
    for (LineInfo* line:lineList) {
        ui->comboBox_line_s->addItem(line->getName(), line->getCode());
        ui->comboBox_line_e->addItem(line->getName(), line->getCode());
    }


//    QString msg = "";
//    for (int i = 0; i < 100; i++) {
//        msg += "这是一段废话，test。";
//    }
//    ui->textBrowser->setText(msg);

//    connect(DataCenter::getThis(), &DataCenter::lineReceived, this, &LineWidget::onReadLines);
}

void LineWidget::onReadLines()
{
    m_lineInfoLst.clear();
    m_lineInfoLst = DataCenter::getThis()->getLineList();

//    if (NULL != m_layout) {
//        delete m_layout;
//    }
    m_layout = new QVBoxLayout();

//    delete m_btnGroup;
    m_btnGroup = new QButtonGroup(this);

    // 线路总图
    QPushButton* btnAll = new QPushButton(this);
    btnAll->setText("运营线路图");
    btnAll->setProperty("pname", "line");
    btnAll->setStyleSheet("background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 #26B6B5, stop:1 #62AF3E);" );
    m_layout->addWidget(btnAll);
    m_btnGroup->addButton(btnAll, LINE_ALL_ID);


    // 各线路详情图按钮
    int lineNum = m_lineInfoLst.size();
    for (int i = 0; i < lineNum; i++) {
        QPushButton* btn = new QPushButton(this);
        btn->setText(m_lineInfoLst[i]->getName());
        btn->setProperty("pname", "line");
        btn->setStyleSheet("background-color: " + m_lineInfoLst[i]->getColor() + ";");
        m_layout->addWidget(btn);
        m_btnGroup->addButton(btn, i);
    }

    // 线路查询
    QPushButton* queryBtn = new QPushButton(this);
    queryBtn->setText("路线查询");
    queryBtn->setProperty("pname", "line");
    queryBtn->setStyleSheet("background-color: #736FC9;");
    m_layout->addWidget(queryBtn);
    m_btnGroup->addButton(queryBtn, lineNum);

    m_layout->setContentsMargins(42, 60, 42, 1);
    m_layout->setSpacing(60);
    m_layout->addSpacerItem(new QSpacerItem(5, 100, QSizePolicy::Minimum, QSizePolicy::Expanding));

    connect(m_btnGroup, &QButtonGroup::idClicked,
            this, &LineWidget::onLineChange);
    m_btnGroup->setExclusive(true);

    ui->lineFrame1->setLayout(m_layout);

    btnAll->setChecked(true);
    onLineChange(LINE_ALL_ID);
}

void LineWidget::onLineQuery(QString info)
{
    ui->textBrowser->clear();
    ui->textBrowser->setText(info);
}

void LineWidget::onLineChange(int id)
{
    QString styleStr = "border-image: url(images/info/line00.jpg);";
    if (id == LINE_ALL_ID) {
        ui->widget_query->hide();
        ui->lineFrame3->show();
        ui->nameLabel->setText("南昌轨道交通运营线路图");
//        QString path = "images/info/line00.jpg";
//        showImage(path);
        ui->lineFrame3->setStyleSheet(styleStr);
        return;
    }

    int size = m_lineInfoLst.size();
    if (id == size) {
        ui->nameLabel->setText("路线查询");
        ui->lineFrame3->hide();
        ui->widget_query->show();
        return;
    }

    if (id < 0 || id > m_lineInfoLst.size()) {
        return;
    }

    ui->widget_query->hide();
    ui->lineFrame3->show();
    ui->nameLabel->setText(m_lineInfoLst[id]->getName() + "线路图");

//    QString path = "images/info/"+ m_lineInfoLst[id]->getPicPath();
//    showImage(path);
    styleStr = "border-image: url(images/info/"+ m_lineInfoLst[id]->getPicPath() + ");";

    qDebug() << "线路图  " << m_lineInfoLst[id]->getPicPath();
    ui->lineFrame3->setStyleSheet(styleStr);
}

bool LineWidget::showData()
{
    ui->textBrowser->setText("");

    onReadLines();
}

void LineWidget::lineQuery()
{
    QString startCode = ui->comboBox_station_s->currentData().toString();
    QString endCode = ui->comboBox_station_e->currentData().toString();

    HttpTool::getThis()->requestPrice(startCode, endCode);
}


void LineWidget::onStartLineSelect(QString lineName) {
    QList<LineStations*> lines = DataCenter::getThis()->getLineStations();

    for(LineStations* line:lines) {
        QString name = line->getName();

        if (name != lineName) {
            continue;
        }

        ui->comboBox_station_s->clear();
        QList<Station*> stationList = line->stationList();
        for (Station* station:stationList) {
            QString name = station->name();
            QString code = station->code();

            ui->comboBox_station_s->addItem(name, code);
        }
    }
}


void LineWidget::onEndLineSelect(QString lineName)
{
    QList<LineStations*> lines = DataCenter::getThis()->getLineStations();

    for(LineStations* line:lines) {
        QString name = line->getName();

        if (name != lineName) {
            continue;
        }

        ui->comboBox_station_e->clear();
        QList<Station*> stationList = line->stationList();
        for (Station* station:stationList) {
            QString name = station->name();
            QString code = station->code();

            ui->comboBox_station_e->addItem(name, code);
        }
    }
}



void LineWidget::showImage(QString lineImagePath)
{
//    ui->label_image->setFixedSize(ui->lineFrame3->width(), ui->lineFrame3->height());

//    QPixmap bgImage(lineImagePath);

//    if (bgImage.isNull())
//    {
//        ui->label_image->setStyleSheet("background-color:black");
//    }
//    else
//    {

////            case 居中:
////                ui->label_image->setStyleSheet("background-color:black;background-image:url(1.jpg);background-position:center;background-repeat:no-repeat;");
////                break;

////            case 缩放:
//                ui->label_image->setStyleSheet("background-color:black");
//                ui->label_image->setAlignment(Qt::AlignCenter);
//                ui->label_image->clear();
//                ui->label_image->setPixmap(bgImage.scaled(this->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
////                break;

////            case 拉伸:
////                ui->label_image->setStyleSheet("background-color:black");
////                ui->label_image->setPixmap(bgImage.scaled(this->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
////                break;

////            case 平铺:
////                ui->label_image->setStyleSheet("background-color:black;background-image:url(1.jpg);background-position:top left;background-repeat:repeat-xy;");
////                break;

//    }
//    bgImage.detach();

}
