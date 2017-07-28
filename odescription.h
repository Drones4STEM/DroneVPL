#ifndef ODESCRIPTION_H
#define ODESCRIPTION_H

#include <QDialog>

namespace Ui {
class ODescription;
}

class ODescription : public QDialog
{
    Q_OBJECT

public:
    explicit ODescription(QWidget *parent = 0);
    ~ODescription();
    void on_actionNew_Window_triggered();

private:
    Ui::ODescription *ui;
    QDialog *aDescription;
};

#endif // ODESCRIPTION_H
