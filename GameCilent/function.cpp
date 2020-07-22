#include "function.h"
#include <QLabel>

Function::Function()
{

}

void Function::initLabel(QLabel *label)
{
    label->setStyleSheet("QLabel{color:rgba(0, 0, 0, 255); "
                         "font-size:14pt;"
                         "font-family:KaiTi}");
}
