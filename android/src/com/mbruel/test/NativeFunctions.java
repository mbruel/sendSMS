package com.mbruel.test;

public class NativeFunctions {
    public static native void smsNbParts(int msgId, int nbPart);
    public static native void smsSent(int msgId, int res);
    public static native void smsDelivered(int msgId, int res);
}
