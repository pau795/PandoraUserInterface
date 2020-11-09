#include "consoleWidget.h"
#include <iostream>
using namespace std;

ConsoleWidget::ConsoleWidget(QWidget* parent): QTextEdit(parent)
{

}

void ConsoleWidget::addLine(QString s){
        s.remove("\x1b(B", Qt::CaseInsensitive);

        Utils::AnsiEscapeCodeHandler ansi_handler;

        FormattedTextList result = ansi_handler.parseText (Utils::FormattedText(s, this->currentCharFormat()));

        foreach(Utils::FormattedText ft, result){
            this->setCurrentCharFormat(ft.format);
            this->insertPlainText(ft.text);
        }
        this->insertPlainText("\n");
        this->verticalScrollBar()->setValue(this->verticalScrollBar()->maximum());
}

