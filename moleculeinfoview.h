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
signals:
    void toEditor(int);
public slots:
    void fillInfoTable(std::vector<std::pair<std::string,std::string> >);

private:
    Ui::MoleculeInfoView *ui;
private slots:
    void toEditor();
};

#endif // MOLECULEINFOVIEW_H
