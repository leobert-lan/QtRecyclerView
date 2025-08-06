//
// Created by Lenovo on 2025/8/5.
//

#ifndef RECYCLERCACHEPOOL_H
#define RECYCLERCACHEPOOL_H

#include <QMap>
#include <QVector>
#include "ViewHolder.h"

class RecyclerCachePool {
public:
    void putRecycledView(const QString& type, ViewHolder* view) {
        pool[type].append(view);
        view->hide();
    }

    ViewHolder* getRecycledView(const QString& type) {
        if (pool.contains(type) && !pool[type].isEmpty()) {
            return pool[type].takeLast();
        }
        return nullptr;
    }

private:
    QMap<QString, QVector<ViewHolder*>> pool;
};

#endif //RECYCLERCACHEPOOL_H
