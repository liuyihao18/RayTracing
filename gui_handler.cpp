#include "gui_handler.h"

GUIHandler *GUIHandler::_handler = nullptr;

void GUIHandler::info(const QString &msg) const
{
    emit infoMsg(msg);
}

void GUIHandler::warn(const QString &msg) const
{
    emit warnMsg(msg);
}

void GUIHandler::err(const QString &msg) const
{
    emit errMsg(msg);
}

void GUIHandler::updateProgress(double progress) const
{
    emit updateProgressMsg(progress);
}


