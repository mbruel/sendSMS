#include "SmsSender.h"
#include <QDebug>
SmsSender::SmsSender():
    QObject(), Singleton<SmsSender>(),
    _nextMsgId(0), _sendingMsgs(),
    _nbMsgs(0), _nbSent(0), _nbDelivered(0), _nbErrors(0)
{}


SmsSender::~SmsSender()
{
    qDeleteAll(_sendingMsgs);
}


#if defined (Q_OS_ANDROID)
#include <QtAndroid>
#include <QAndroidJniObject>
#endif
void SmsSender::sendText(const QString &destMobile, const QString &msg)
{
#ifdef __DEBUG__
    LogService::sendLog("in da cpp: sending text to "+destMobile+ ": " +msg);
#endif
    if (msg.isEmpty())
    {
        _error("SmsSender won't send an empty msg...");
        return;
    }

#if defined (Q_OS_ANDROID)
    auto  result = QtAndroid::checkPermission(QString("android.permission.SEND_SMS"));
    if(result == QtAndroid::PermissionResult::Denied){
        _log("don't have android.permission.SEND_SMS");

        QtAndroid::PermissionResultMap resultHash = QtAndroid::requestPermissionsSync(QStringList({"android.permission.SEND_SMS"}));
        if(resultHash["android.permission.SEND_SMS"] == QtAndroid::PermissionResult::Denied)
        {
            _error("couldn't get android.permission.SEND_SMS");
            return;
        }
    }

    QAndroidJniObject myPhoneNumber = QAndroidJniObject::fromString(destMobile);
    QAndroidJniObject myTextMessage = QAndroidJniObject::fromString(msg);

    jint msgId = ++_nextMsgId;
    ++_nbMsgs;
    _sendingMsgs.insert(msgId, new SmsStatus(msgId));
    QtAndroid::androidActivity().callMethod<void>("sendMessage",
                                                  "(ILjava/lang/String;Ljava/lang/String;)V",
                                                  msgId,
                                                  myPhoneNumber.object<jstring>(),
                                                  myTextMessage.object<jstring>());

#else
    Q_UNUSED(destMobile)
    Q_UNUSED(msg)
    _error("Sending text is only working on an Android device!...");
#endif
}

void SmsSender::msgParts(int msgId, int nbParts)
{
    _sendingMsgs[msgId]->nbParts = nbParts;
    _log(tr("Message #%1 has %2 parts").arg(msgId).arg(nbParts));
}

void SmsSender::msgSent(int msgId, bool success)
{
    if (success)
        ++_sendingMsgs[msgId]->nbSent.first;
    else
        ++_sendingMsgs[msgId]->nbSent.second;

    if (_sendingMsgs[msgId]->allPartsSent())
    {
        _sendingMsgs[msgId]->state = MSG_STATE::SENT;
        QString msgStatus;
        int nbMsgErrors = _sendingMsgs[msgId]->nbSent.second;
        if (nbMsgErrors == 0)
        {
            ++_nbSent;
            msgStatus = tr("successfully");
        }
        else
        {
            ++_nbErrors;
            msgStatus = tr("with %1 errors").arg(nbMsgErrors);
        }

        _log(tr("Message #%1 has been sent %2").arg(msgId).arg(msgStatus));
#if defined (Q_OS_ANDROID)
        QAndroidJniObject msgSent = QAndroidJniObject::fromString("MSG SENT");
        QtAndroid::androidActivity().callMethod<void>("popup",
                                                      "(Ljava/lang/String;)V",
                                                      msgSent.object<jstring>());
#endif

    }
}

void SmsSender::msgDelivered(int msgId, bool success)
{
    if (success)
        ++_sendingMsgs[msgId]->nbDelivered.first;
    else
        ++_sendingMsgs[msgId]->nbDelivered.second;

    if (_sendingMsgs[msgId]->allPartsDelivered())
    {
        _sendingMsgs[msgId]->state = MSG_STATE::DELIVERED;
        QString msgStatus;
        int nbMsgErrors = _sendingMsgs[msgId]->nbDelivered.second;
        if (nbMsgErrors == 0)
        {
            ++_nbDelivered;
            msgStatus = tr("successfully");
        }
        else
        {
            ++_nbErrors;
            msgStatus = tr("with %1 errors").arg(nbMsgErrors);
        }

        _log(tr("Message #%1 has been delivered %2").arg(msgId).arg(msgStatus));
#if defined (Q_OS_ANDROID)
        QAndroidJniObject msgSent = QAndroidJniObject::fromString("MSG DELIVERED");
        QtAndroid::androidActivity().callMethod<void>("popup",
                                                      "(Ljava/lang/String;)V",
                                                      msgSent.object<jstring>());
#endif
    }
}

void SmsSender::_log(const QString &txt)
{
    qDebug() << txt;
    emit log(txt);
}

void SmsSender::_error(const QString &txt)
{
    qCritical() << txt;
    emit error(txt);
}
