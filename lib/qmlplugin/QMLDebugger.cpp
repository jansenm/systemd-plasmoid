//
// Created by mjansen on 2/16/20.
//

#include "QMLDebugger.h"


QString QMLDebugger::properties(QObject *item, bool linebreak)
{
    const QMetaObject *meta = item->metaObject();

    QHash<QString, QVariant> list;
    for (int i = 0; i < meta->propertyCount(); i++)
    {
        QMetaProperty property = meta->property(i);
        const char* name = property.name();
        QVariant value = item->property(name);
        list[name] = value;
    }

    QString out;
    QHashIterator<QString, QVariant> i(list);
    while (i.hasNext()) {
        i.next();
        if (!out.isEmpty())
        {
            out += ", ";
            if (linebreak) out += "\n";
        }
        out.append(i.key());
        out.append(": ");
        out.append(i.value().toString());
    }
    return out;
}
