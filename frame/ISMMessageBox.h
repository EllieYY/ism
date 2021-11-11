#ifndef ISMMESSAGEBOX_H
#define ISMMESSAGEBOX_H

#include <QDialog>

namespace Ui {
class ISMMessageBox;
}

class ISMMessageBox : public QDialog
{
    Q_OBJECT

public:
    explicit ISMMessageBox(QWidget *parent = nullptr);
    ~ISMMessageBox();

    void SetMessage(const QString &msg, int type);

protected:
    void mouseMoveEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *);

private:
    void onBtnOkClicked();

private:
    QPoint mousePoint;              //鼠标拖动自定义标题栏时的坐标
    bool mousePressed;              //鼠标是否按下

private:
    Ui::ISMMessageBox *ui;
};

#endif // ISMMESSAGEBOX_H
