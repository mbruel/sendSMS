#include <jni.h>
#include <QDebug>
#include "SmsSender.h"


static void smsNbParts(JNIEnv * /*env*/, jobject /*obj*/, jint msgId, jint nbParts)
{
    qDebug() << "[QT native] sms #" << msgId << " has parts: " << nbParts;
    SmsSender::getInstance()->msgParts(msgId, nbParts);
}

static void smsSent(JNIEnv * /*env*/, jobject /*obj*/, jint msgId, jint res)
{
    qDebug() << "[QT native] smsSent #" << msgId << " res: " << res;
    SmsSender::getInstance()->msgSent(msgId, res == -1);
}

static void smsDelivered(JNIEnv * /*env*/, jobject /*obj*/, jint msgId, jint res)
{
    qDebug() << "[QT native] smsDelivered #" << msgId << " res: " << res;
    SmsSender::getInstance()->msgDelivered(msgId, res == -1);
}

//create a vector with all our JNINativeMethod(s)
static JNINativeMethod methods[] = {
    {"smsNbParts",   "(II)V", (void *)smsNbParts},
    {"smsSent",      "(II)V", (void *)smsSent},
    {"smsDelivered", "(II)V", (void *)smsDelivered},
};

// this method is called automatically by Java after the .so file is loaded
JNIEXPORT jint JNI_OnLoad(JavaVM* vm, void* /*reserved*/)
{
    JNIEnv* env;
    // get the JNIEnv pointer.
    if (vm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6) != JNI_OK)
      return JNI_ERR;

    // search for Java class which declares the native methods
    jclass javaClass = env->FindClass("com/mbruel/test/NativeFunctions");
    if (!javaClass)
      return JNI_ERR;

    // register our native methods
    if (env->RegisterNatives(javaClass, methods,
                          sizeof(methods) / sizeof(methods[0])) < 0) {
      return JNI_ERR;
    }

    return JNI_VERSION_1_6;
}
