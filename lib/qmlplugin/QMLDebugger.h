//
// Created by mjansen on 2/16/20.
//

#ifndef SYSTEMD_KDE_QMLDEBUGGER_H
#define SYSTEMD_KDE_QMLDEBUGGER_H

#include <QtCore/QString>
#include <QtQuick/QQuickItem>

class QMLDebugger : public QObject {
    Q_OBJECT

public:
    Q_INVOKABLE static QString properties(QObject *item, bool linebreak = true);

};


#endif //SYSTEMD_KDE_QMLDEBUGGER_H
