#ifndef IVW_CONSOLEWIDGET_H
#define IVW_CONSOLEWIDGET_H

#include <inviwo/qt/editor/inviwoqteditordefine.h>
#include <QTextEdit>

#include <inviwo/core/util/logdistributor.h>
#include <inviwo/qt/editor/inviwodockwidget.h>

namespace inviwo {

class IVW_QTEDITOR_API ConsoleWidget : public InviwoDockWidget, public Logger {

public:
    ConsoleWidget(QWidget* parent);
    ~ConsoleWidget();

    void log(std::string logSource, unsigned int logLevel, const char* fileName, const char* functionName, int lineNumber, std::string logMsg);

private:
    QTextEdit* textField_;
};

} // namespace

#endif // IVW_CONSOLELISTWIDGET_H