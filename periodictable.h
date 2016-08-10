#ifndef PeriodicTable_H
#define PeriodicTable_H

#include <QWidget>
#include <QGridLayout>
#include <QPushButton>
#include <QButtonGroup>

namespace Ui {
class PeriodicTable;
}

class PeriodicTable : public QWidget
{
    Q_OBJECT

public:
    explicit PeriodicTable(QWidget *parent = 0);
    ~PeriodicTable();

public slots:
    void buttonPressed(int);

signals:
    void elementChosen(QString Element);
    void back(int);

private:
    Ui::PeriodicTable *ui;
    const QString elements[118] = {"H","He"
                   ,"Li","Be","B","C","N","O","F","Ne"
                   ,"Na","Mg","Al","Si","P","S","Cl","Ar"
                   ,"K","Ca","Sc","Ti","V","Cr","Mn","Fe","Co","Ni","Cu","Zn","Ga","Ge","As","Se","Br","Kr"
                  ,"Rb","Sr","Y","Zr","Nb","Mo","Tc","Ru","Rh","Pd","Ag","Cd","In","Sn","Sb","Te","I","Xe"
                  ,"Cs","Ba","La","Ce","Pr","Nd","Pm","Sm","Eu","Gb","Tb","Dy","Ho","Er","Tm","Yb","Lu","Hf","Ta","W","Re","Os","Ir","Pt","Au","Hg","Tl","Pb","Bi","Po","At","Rn"
                  ,"Fr","Ra","Ac","Th","Pa","U","Np","Pu","Am","Cm","Bk","Cf","Es","Fm","Md","No","Lr","Rf","Db","Sg","Bh","Hs","Mt","Ds","Rg","Cn","Uut","Fl","Uup","Lv","Uus","UUo"};
    QGridLayout* generatePeriodicTable();
    QPushButton* elementButton(int i);
    QButtonGroup* buttonTable;
private slots:
    void back();

};

#endif // PeriodicTable_H
