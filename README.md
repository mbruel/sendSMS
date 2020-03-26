# sendSMS
Qt app to send SMS on Android devices (support multi-part messages).

All the sending is made in a **custom Activity** in Java inheriting from QtActivity, cf MyActivity.java
The Android SMS sending implementation has been following [Supertos article](http://supertos.free.fr/supertos.php?page=1198)

C++ code use JNI via **QtAndroid::androidActivity().callMethod**
BroadcastReceiver from Android uses **JNI native calls** to notify back the C++ objects

Correspondance between Java native functions and C++ is done in **native.cpp** ([following KDAB example](https://www.kdab.com/qt-android-episode-5/))

From a new project (QML project), you must Create the Android Templates (QtCreator -> Project -> Build -> Android -> Create Templates)
Once done:
   - open the AndroidManifest.xml within QtCreator to **add the SEND_SMS permission**
   - edit AndroidManifest.xml with a text editor to change the **manifest package name** (line 2) but also the **activity name** that should be MyActivity (line 16)

Appart from that, calling Android is not that difficult ;)

![sendSMS on Xiaomi Redmi](https://raw.githubusercontent.com/mbruel/sendSMS/master/pics/sendSMS.png)

Cheers!
