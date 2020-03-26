#ifndef SMSSENDER_H
#define SMSSENDER_H
#include "Singleton.h"
#include <QObject>
#include <QMap>

class SmsSender : public QObject, public Singleton<SmsSender>
{
    Q_OBJECT
    friend class Singleton<SmsSender>;

    enum class MSG_STATE : char {SENDING = 0, SENT, DELIVERED, ERROR, CANCELLED};
    struct SmsStatus
    {
        const int msgId;
        int       nbParts;
        MSG_STATE state;

        QPair<int, int> nbSent; //!< first is for OK, second for Errors
        QPair<int, int> nbDelivered;

        bool allPartsSent() const      {return nbSent.first + nbSent.second == nbParts;}
        bool allPartsDelivered() const {return nbDelivered.first + nbDelivered.second == nbParts;}

        SmsStatus(int msgId_):
            msgId(msgId_), nbParts(0), state(MSG_STATE::SENDING),
            nbSent(0, 0), nbDelivered(0, 0){}
    };

private:
    int _nextMsgId;
    QMap<int, SmsStatus*>  _sendingMsgs;
    long _nbMsgs;
    long _nbSent;
    long _nbDelivered;
    long _nbErrors;

signals:
    void error(const QString &txt);
    void log(const QString &txt);

public:
    ~SmsSender();

    Q_INVOKABLE void sendText(const QString &destMobile, const QString &msg);

    void msgParts(int msgId, int nbParts);
    void msgSent(int msgId, bool success);
    void msgDelivered(int msgId, bool success);

private:
    void _log(const QString &txt);
    void _error(const QString &txt);

private:
    SmsSender();
};
#endif // SMSSENDER_H
