#include "customplaintextedit.h"

CustomPlainTextEdit::CustomPlainTextEdit(QWidget *parent)
    : QPlainTextEdit(parent)
{

}

CustomPlainTextEdit::~CustomPlainTextEdit()
{

}


void CustomPlainTextEdit::keyPressEvent(QKeyEvent *event)
{
    emit keyInput();

    QPlainTextEdit::keyPressEvent(event);
}

void CustomPlainTextEdit::keyReleaseEvent(QKeyEvent *event)
{
    QPlainTextEdit::keyReleaseEvent(event);
}
