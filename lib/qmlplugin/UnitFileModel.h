//
// Created by mjansen on 2/19/20.
//
#ifndef SYSTEMD_KDE_UNITFILEMODEL_H
#define SYSTEMD_KDE_UNITFILEMODEL_H

#include "systemd-qt_export.h"
#include <QtCore/QAbstractListModel>
#include <Manager.h>

class SYSTEMD_QT_EXPORT UnitFileModel : public QAbstractListModel {
Q_OBJECT

public:

    enum Roles {
        Path = Qt::UserRole + 1,
        Name,
        State,
    };
    Q_ENUM(Roles);

    explicit UnitFileModel(Systemd::Manager *manager, QObject *parent = nullptr);

    ~UnitFileModel() override;

    QVariant data(const QModelIndex &index, int role) const override;

    QHash<int, QByteArray> roleNames() const override;

    int rowCount(const QModelIndex &parent) const override;


public slots:

    void unitFilesChanged();

private:

    class UnitFileModelPrivate *d_ptr;
    Q_DECLARE_PRIVATE(UnitFileModel);
};


#endif //SYSTEMD_KDE_UNITFILEMODEL_H
