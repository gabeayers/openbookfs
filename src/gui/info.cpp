#include "info.h"

Info::Info(QWidget *parent) :
    QPushButton(parent)
{
    QPixmap pixmap("/Users/ga22612/Code/Openbookfs/openbookfs/src/gui/i_icon.png");
    QIcon ButtonIcon(pixmap);
    this->setIcon(ButtonIcon);
    this->setIconSize(pixmap.rect().size());
    this->setFlat(true);

    i = new QWidget(parent);

    i->setGeometry(400,0,400,300);
    i->setStyleSheet("background:#fff");
    QVBoxLayout *layout = new QVBoxLayout();
    layout->setAlignment(Qt::AlignTop);
    i->setLayout(layout);
    i->setVisible(false);

    text = new QLabel();
    text->setWordWrap(true);

    layout->addWidget(text);

}
void Info::enterEvent(QEvent * e)
{
    i->setVisible(true);

    QPushButton::enterEvent(e);
}

void Info::leaveEvent(QEvent * e)
{

    i->setVisible(false);
    QPushButton::leaveEvent(e);
}
