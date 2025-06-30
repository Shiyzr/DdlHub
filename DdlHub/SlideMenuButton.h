#ifndef SLIDEMENUBUTTON_H
#define SLIDEMENUBUTTON_H

#include <QPushButton>

class SlideMenuButton : public QPushButton
{
public:
    // SlideMenuButton();

    explicit SlideMenuButton(QWidget *parent = nullptr);
    SlideMenuButton(const QString &text, QWidget *parent = nullptr);

    void paintEvent(QPaintEvent *event) override;
    void enterEvent(QEnterEvent *event) override;
    void leaveEvent(QEvent *event) override;

    bool m_hovered;
};

#endif // SLIDEMENUBUTTON_H
