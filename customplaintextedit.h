#ifndef CUSTOMPLAINTEXTEDIT_H
#define CUSTOMPLAINTEXTEDIT_H

#include <QPlainTextEdit>

class CustomPlainTextEdit : public QPlainTextEdit
{
    Q_OBJECT
public:
    CustomPlainTextEdit(QWidget *parent = nullptr);
    ~CustomPlainTextEdit();

signals:
    void keyInput();

    // QWidget interface
protected:
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
};

#endif // CUSTOMPLAINTEXTEDIT_H
