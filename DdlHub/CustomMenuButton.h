#ifndef CUSTOMMENUBUTTON_H
#define CUSTOMMENUBUTTON_H

#include <QPushButton>

class CustomMenuButton : public QPushButton {
    Q_OBJECT
public:
    explicit CustomMenuButton(QWidget *parent = nullptr);

    void paintEvent(QPaintEvent *event) override;
};

#endif // CUSTOMMENUBUTTON_H
