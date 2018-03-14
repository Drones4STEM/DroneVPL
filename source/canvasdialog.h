#ifndef CANVASDIALOG_H
#define CANVASDIALOG_H
#include <QDialog>
#include "ui_canvasdialog.h"
class QGraphicsView;

/*******************************************************************
 * Class name: CanvasDialog
 * Base class: QDialog, Ui::canvasdialog
 * Description: This is the declaration of class CanvasDialog.
 ******************************************************************/
class CanvasDialog : public QDialog, public Ui::Dialog
{
    Q_OBJECT
public:
    CanvasDialog(QGraphicsView *aview,QWidget *parent=0);
private slots:
    void on_buttonBox_accepted();
private:
    QGraphicsView *view;

};

#endif // CANVASDIALOG_H
