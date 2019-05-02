#include "periodictable.h"
#include <qdebug.h>

PeriodicTable::PeriodicTable(QWidget *parent) :
    QWidget(parent)
{
    buttonTable=new QButtonGroup();
    generatePeriodicTable();
    connect(buttonTable,SIGNAL(buttonClicked(int)) ,this,SLOT(buttonPressed(int)));
    connect(this,SIGNAL(elementChosen(QString)),this,SLOT(back()));
}


PeriodicTable::~PeriodicTable()
{
    delete buttonTable;
}
QGridLayout* PeriodicTable::generatePeriodicTable(){
    QGridLayout* perTable=new QGridLayout(this);
    setLayout(perTable);
    perTable->setSpacing(0);
    //Orbital numbers
    const int s=2;
    const int p=6;
    const int d=10;
    const int f=14;

    int period=1;
    int group=0;
    for (int i=0;i<118;i++){
        perTable->addWidget(elementButton(i),period,group);

        //Switch case for rows
        switch (period) {
        case 1:
            if(i==s-1){
                period++;
                group=0;
            }
            else {
                group=17;
            }
            break;
        case 2: case 3:
            if(i==s*period+p*(period-1)-1){
                period++;
                group=0;
            }
            else {
                group++;
                if(group==2)
                    group+=10;
            }
            break;
        case 4: case 5:
            if(i==s*period+p*(period-1)+d*(period-3)-1){
                period++;
                group=0;
            }
            else{
                group = (i-18)%18+1;
            }
            break;
        case 6: case 7:
            if(i==s*period+p*(period-1)+d*(period-3)+f*(period-5) -1){
                period++;
                group=0;
            }
            else
                if(i<57){
                    group = (i-18)%18+1;
                }
                else if (i<89 && i>85){
                    group = i-85;
                }
                else if (i<71 || (i<103&&i>85) ){

                    //lantanoids and actinoids are added to bottom of table
                    for(int j=0;j<f;j++){
                        perTable->addWidget(elementButton(i),period+2,j+2);
                        i++;
                    }
                    i--;
                }
                else if (i<85){
                    group = i-67;
                }
                else{
                    group = i-99;
                }
            break;
        default:
            break;
        }
        //period++;
    }
    perTable->setSizeConstraint(QLayout::SetNoConstraint);
    for (int i=0;i<perTable->columnCount();i++) {
        perTable->setColumnMinimumWidth(i,1);
    }
    for (int i=0;i<perTable->rowCount();i++) {
        perTable->setRowMinimumHeight(i,1);
    }

    qDebug()<<"PERTABLE"<<perTable->sizeHint();

    return perTable;
}
QPushButton* PeriodicTable::elementButton(int i){
    QPushButton* but=new QPushButton(elements[i]);// QString().number(i));
    buttonTable->addButton(but,i);
    return but;
}
void PeriodicTable::buttonPressed(int i){
    emit elementChosen(elements[i]);
}
void PeriodicTable::back(){
    emit back(0);
}
