#include "gui_handler.h"

GUIHandler *GUIHandler::_handler = nullptr;

void GUIHandler::info(const QString &msg)
{
    emit infoMsg(msg);
}

void GUIHandler::warn(const QString &msg)
{
    emit warnMsg(msg);
}

void GUIHandler::err(const QString &msg)
{
    emit errMsg(msg);
}

void GUIHandler::updateProgress(double progress)
{
    emit updateProgressMsg(progress);
}
