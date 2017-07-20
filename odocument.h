#ifndef ODOCUMENT_H
#define ODOCUMENT_H

#include <QDialog>

namespace Ui {
class oDocument;
}

class oDocument : public QDialog
{
    Q_OBJECT

public:
    explicit oDocument(QWidget *parent = 0);
    ~oDocument();

private:
    Ui::oDocument *ui;
};

#endif // ODOCUMENT_H
