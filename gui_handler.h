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
    void infoMsg(QString) const;
    void warnMsg(QString) const;
    void errMsg(QString) const;
    void updateProgressMsg(double progress) const;

public:
    void info(const QString& msg) const;
    void warn(const QString& msg) const;
    void err(const QString& msg) const;
    void updateProgress(double progress) const;

};

#endif // ALERTHANDLER_H
