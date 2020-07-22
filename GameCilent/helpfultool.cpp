#include "helpfultool.h"

HelpfulTool::HelpfulTool()
{

}

void HelpfulTool::setAlpha(QImage &image, int alpha)
{
    for (int y = 0; y < image.height(); ++y)
    {
        QRgb *row = (QRgb*)image.scanLine(y);
        for (int x = 0; x < image.width(); ++x)
        {
            ((unsigned char*)&row[x])[3] = (unsigned char)alpha;
        }
    }
}
