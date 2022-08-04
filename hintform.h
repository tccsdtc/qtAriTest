#ifndef HINTFORM_H
#define HINTFORM_H

#include <QWidget>

namespace Ui {
class HintForm;
}

class HintForm : public QWidget
{
    Q_OBJECT

public:
    explicit HintForm(QWidget *parent = nullptr);
    ~HintForm();

private:
    Ui::HintForm *ui;
};

#endif // HINTFORM_H
