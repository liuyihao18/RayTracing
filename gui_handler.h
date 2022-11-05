#ifndef GUI_HANDLER_H
#define GUI_HANDLER_H
#include <QObject>

class GUIHandler : public QObject
{
    Q_OBJECT

public:
    static GUIHandler *Inst()
    {
        if (!_handler)
        {
            _handler = new GUIHandler();
        }
        return _handler;
    }

private:
    GUIHandler(QObject *parent = nullptr) : QObject(parent) {}
    static GUIHandler *_handler;

signals:
    void infoMsg(QString);
    void warnMsg(QString);
    void errMsg(QString);
    void updateProgressMsg(double progress);

public:
    void info(const QString &msg);
    void warn(const QString &msg);
    void err(const QString &msg);
    void updateProgress(double progress);
};

#endif // ALERTHANDLER_H
