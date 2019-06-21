#ifdef HAVE_CONFIG_H
#include "../config/thought-config.h"
#endif

#include <QAbstractListModel>
#include <QString>

class NotificationLevels: public QAbstractListModel
{
    Q_OBJECT

public:
    explicit NotificationLevels(QObject *parent);

    enum Level
    {
        None,
        All,
        NonMining
    };

    //! @name Static API
    //! Notification level utilities
    ///@{

    //! Get list of notification levels, for drop-down box
    static QList<Level> availableLevels();
    //! Is notification level valid?
    static bool valid(int level);
    //! Short name
    static QString name(int level);
    //! Longer description
    static QString description(int level);
    ///@}

    //! @name AbstractListModel implementation
    //! List model for notification level drop-down selection box.
    ///@{
    enum RoleIndex {
        /** Level identifier */
        LevelRole = Qt::UserRole
    };
    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    ///@}

private:
    QList<NotificationLevels::Level> levellist;
};
typedef NotificationLevels::Level NotificationLevel;
