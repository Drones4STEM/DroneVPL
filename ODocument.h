#ifndef AWINDOW_H
#define AWINDOW_H

#include <QDialog>

namespace Ui {
class aWindow;
}

class aWindow : public QDialog
{
    Q_OBJECT

public:
    explicit aWindow(QWidget *parent = 0);
    void on_actionNew_Window_triggered();
    ~aWindow();

private:
    Ui::aWindow *ui;
    QDialog *aDialog;


};

#endif // AWINDOW_H
