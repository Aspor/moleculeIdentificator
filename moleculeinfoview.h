#ifndef MOLECULEINFOVIEW_H
#define MOLECULEINFOVIEW_H

#include <QWidget>

namespace Ui {
class MoleculeInfoView;
}

class MoleculeInfoView : public QWidget
{
    Q_OBJECT

public:
    explicit MoleculeInfoView(QWidget *parent = 0);
    ~MoleculeInfoView();

private:
    Ui::MoleculeInfoView *ui;
};

#endif // MOLECULEINFOVIEW_H
