//
// Created by mjansen on 1/29/20.
//

#ifndef PLASMA_ENGINE_SYSTEMD_QMLPLUGIN_H
#define PLASMA_ENGINE_SYSTEMD_QMLPLUGIN_H

#include <QtQml/QQmlExtensionPlugin>

#include <QDebug>

class QmlPlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QQmlExtensionInterface/1.0")
  public:
    explicit QmlPlugin(QObject* parent = Q_NULLPTR)
      : QQmlExtensionPlugin(parent)
    {}

    ~QmlPlugin(){};

    void registerTypes(const char* uri) override;
};

#endif
