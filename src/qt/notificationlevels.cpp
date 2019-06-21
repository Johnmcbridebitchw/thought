#include "notificationlevels.h"

NotificationLevels::NotificationLevels(QObject *parent):
        QAbstractListModel(parent),
        levellist(availableLevels())
{
}

QList<NotificationLevels::Level> NotificationLevels::availableLevels()
{
    QList<NotificationLevels::Level> levellist;
    levellist.append(None);
    levellist.append(NonMining);
    levellist.append(All);
    return levellist;
}

bool NotificationLevels::valid(int level)
{
    switch(level)
    {
    case None:
    case All:
    case NonMining:
        return true;
    default:
        return false;
    }
}

QString NotificationLevels::name(int level)
{
    switch(level)
    {
    case None: return QString("None");
    case All: return QString("All");
    case NonMining: return QString::fromUtf8("Non-mining");
    default: return QString("???");
    }
}

QString NotificationLevels::description(int level)
{
    switch(level)
    {
    case None: return QString("No transaction notifications");
    case All: return QString("All transaction notifications");
    case NonMining: return QString("All transaction notifications except for mined transactions");
    default: return QString("???");
    }
}

int NotificationLevels::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return levellist.size();
}

QVariant NotificationLevels::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    if(row >= 0 && row < levellist.size())
    {
        Level level = levellist.at(row);
        switch(role)
        {
        case Qt::EditRole:
        case Qt::DisplayRole:
            return QVariant(name(level));
        case Qt::ToolTipRole:
            return QVariant(description(level));
        case LevelRole:
            return QVariant(static_cast<int>(level));
        }
    }
    return QVariant();
}
