#include <jni.h>
#include <android/bitmap.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include "stoneball.h"
#include <dlfcn.h>
#include <assert.h>
#include <string.h>
#include "Global.h"
#include "CommonUtil.h"
#include "BitmapIds.h"

char callProfileActivity = 'n';
char callAboutActivity = 'n';
//char profileName[15] = {'\0'};
//const char* getProfileName() {return profileName;}

/* simple stats management */
typedef struct {
    double  renderTime;
    double  frameTime;
} FrameStats;

#define  MAX_FRAME_STATS  200
#define  MAX_PERIOD_MS    1500

typedef struct {
    double  firstTime;
    double  lastTime;
    double  frameTime;
    double  avgFps;

    int         firstFrame;
    int         numFrames;
    FrameStats  frames[ MAX_FRAME_STATS ];
} Stats;

static void
stats_init( Stats*  s )
{
    s->lastTime = now_ms();
    s->firstTime = 0.;
    s->firstFrame = 0;
    s->numFrames  = 0;
}

static void
stats_startFrame( Stats*  s )
{
    s->frameTime = now_ms();
}

static void
stats_endFrame( Stats*  s )
{
    double now = now_ms();
    double renderTime = now - s->frameTime;
    double frameTime  = now - s->lastTime;
    int nn;

    if (now - s->firstTime >= MAX_PERIOD_MS) {
        if (s->numFrames > 0) {
            double minRender, maxRender, avgRender;
            double minFrame, maxFrame, avgFrame;
            int count;

            nn = s->firstFrame;
            minRender = maxRender = avgRender = s->frames[nn].renderTime;
            minFrame  = maxFrame  = avgFrame  = s->frames[nn].frameTime;
            for (count = s->numFrames; count > 0; count-- ) {
                nn += 1;
                if (nn >= MAX_FRAME_STATS)
                    nn -= MAX_FRAME_STATS;
                double render = s->frames[nn].renderTime;
                if (render < minRender) minRender = render;
                if (render > maxRender) maxRender = render;
                double frame = s->frames[nn].frameTime;
                if (frame < minFrame) minFrame = frame;
                if (frame > maxFrame) maxFrame = frame;
                avgRender += render;
                avgFrame  += frame;
            }
            avgRender /= s->numFrames;
            avgFrame  /= s->numFrames;

            /*
            LOGI("frame/s (avg,min,max) = (%.1f,%.1f,%.1f) "
                 "render time ms (avg,min,max) = (%.1f,%.1f,%.1f)\n",
                 1000./avgFrame, 1000./maxFrame, 1000./minFrame,
                 avgRender, minRender, maxRender);
            */
            //s->avgFps = 1000./avgFrame;
            s->avgFps = s->numFrames * 1000. / (now - s->firstTime);
        }
        s->numFrames  = 0;
        s->firstFrame = 0;
        s->firstTime  = now;
    }

    nn = s->firstFrame + s->numFrames;
    if (nn >= MAX_FRAME_STATS)
        nn -= MAX_FRAME_STATS;

    s->frames[nn].renderTime = renderTime;
    s->frames[nn].frameTime  = frameTime;

    if (s->numFrames < MAX_FRAME_STATS) {
        s->numFrames += 1;
    } else {
        s->firstFrame += 1;
        if (s->firstFrame >= MAX_FRAME_STATS)
            s->firstFrame -= MAX_FRAME_STATS;
    }

    s->lastTime = now;
}

jclass cls = 0;
jmethodID mid_setAvgFps = 0;
jmethodID mid_playSoundEffect = 0;
jmethodID mid_setAudioVolumes = 0;
jmethodID mid_saveSettings = 0;
jmethodID mid_callProfileActivity = 0;
jmethodID mid_callAboutActivity = 0;
JNIEnv * globalEnv = 0;
jobject globalStoneballView;
jmethodID mid_drawGameText = 0;
jmethodID mid_gameTextSize = 0;
jmethodID mid_gameFontBold = 0;
jmethodID mid_openBrowser = 0;

bool fillJavaFunctionPointers(JNIEnv * env, jobject stoneballView) {
    bool forceUpdate = (globalEnv != env || globalStoneballView != stoneballView);

    if(forceUpdate) {
        globalEnv = env;
        globalStoneballView = stoneballView;
        cls = (*env)->GetObjectClass(env, stoneballView);
    }

    if(forceUpdate || !mid_setAvgFps) {
        // type signatures https://docs.oracle.com/javase/1.5.0/docs/guide/jni/spec/types.html#wp276
        mid_setAvgFps = (*env)->GetMethodID(env, cls, "setAvgFps", "(J)V");

        if (mid_setAvgFps == 0) {
            LOGE("Method setAvgFps not found");
            return false;
        }
    }

    if(forceUpdate || !mid_playSoundEffect) {
        mid_playSoundEffect = (*env)->GetMethodID(env, cls, "playSoundEffect", "(Ljava/lang/String;)V");
        if(mid_playSoundEffect == 0) {
            LOGE("Method playSoundEffect not found");
            return false;
        }
    }

    // set music and effect volumes
    if(forceUpdate || !mid_setAudioVolumes) {
        mid_setAudioVolumes = (*env)->GetMethodID(env, cls, "setAudioVolumes", "(II)V");
        if(mid_setAudioVolumes == 0) {
            LOGE("Method setAudioVolumes not found");
            return false;
        }
    }

    //cls = (*env)->GetObjectClass(env, stoneballView);
    // set music and effect volumes
    if(forceUpdate || !mid_saveSettings) {
        mid_saveSettings = (*env)->GetMethodID(env, cls, "saveSettings", "(I[IZ)V");
        if(mid_saveSettings == 0) {
            LOGE("Method saveSettings not found");
            return false;
        }
    }

    //cls = (*env)->GetObjectClass(env, stoneballView);
    if(forceUpdate || !mid_callProfileActivity) {
        mid_callProfileActivity = (*env)->GetMethodID(env, cls, "openProfileActivity", "()V");
        if(mid_callProfileActivity == 0) {
            LOGE("Method openProfileActivity not found");
            return false;
        }
    }

    if(forceUpdate || !mid_drawGameText) {
        mid_drawGameText = (*env)->GetMethodID(env, cls, "drawGameText", "(IILjava/lang/String;ZIIII)V");
        if(mid_drawGameText == 0) {
            LOGE("Method drawGameText not found");
            return false;
        }
    }

    if(forceUpdate || !mid_gameTextSize) {
        mid_gameTextSize = (*env)->GetMethodID(env, cls, "gameTextSize", "(Ljava/lang/String;I)[I");
        if(mid_gameTextSize == 0) {
            LOGE("Method gameTextSize not found");
            return false;
        }
    }
    if(forceUpdate || !mid_gameFontBold) {
        mid_gameFontBold = (*env)->GetMethodID(env, cls, "gameFontBold", "(Z)V");
        if(mid_gameFontBold == 0) {
            LOGE("Method gameFontBold not found");
            return false;
        }
    }
    if(forceUpdate || !mid_openBrowser) {
        mid_openBrowser = (*env)->GetMethodID(env, cls, "openBrowser", "(Ljava/lang/String;)V");
        if(mid_openBrowser == 0) {
            LOGE("Method openBrowser not found");
            return false;
        }
    }

    return true;
}

JNIEXPORT void JNICALL Java_com_stoneball_StoneballView_renderStoneball(JNIEnv * env, jobject jobj, jobject bitmap, jobject stoneballView)
{
    AndroidBitmapInfo  info;
    void*              pixels;
    int                ret;
    static Stats       stats;
    static int         init;
    static int lowFps = false;

    Unused(0, jobj);
    if ((ret = AndroidBitmap_getInfo(env, bitmap, &info)) < 0) {
        LOGE("AndroidBitmap_getInfo() failed ! error=%d", ret);
        return;
    }

    if (info.format != ANDROID_BITMAP_FORMAT_RGB_565) {
        LOGE("Bitmap format is not RGB_565!");
        return;
    }

    if ((ret = AndroidBitmap_lockPixels(env, bitmap, &pixels)) < 0) {
        LOGE("AndroidBitmap_lockPixels() failed ! error=%d", ret);
    }

    fillJavaFunctionPointers(env, stoneballView);

    update_now();
    if (!init) {
        setScreenSize(info.width, info.height);
        stats_init(&stats);
        init = 1;
    }

    stats_startFrame(&stats);

    /* Now fill the values with a nice little stoneball */
    updateWorld(stats.frameTime);
    drawWorld(&info, pixels);
    // sleep, just because the framerate slows down on prestigo :D

    if(lowFps || (stats.avgFps > 1.e-5 && stats.avgFps < 30.)) {
        usleep(2e+3);
        lowFps = true;
    }
    AndroidBitmap_unlockPixels(env, bitmap);

    stats_endFrame(&stats);

    (*env)->CallVoidMethod(env, stoneballView, mid_setAvgFps, (jlong)stats.avgFps);

    jstring effectName = (*env)->NewStringUTF(env, getSoundEffect());
    (*env)->CallVoidMethod(env, stoneballView, mid_playSoundEffect, effectName);
    (*env)->DeleteLocalRef(env, effectName);

    (*env)->CallVoidMethod(env, stoneballView, mid_setAudioVolumes, musicVolume(), effectVolume());

    if(hasSettingsBeenUpdated()) {
        // LevelFactory::GameLevel::LEVEL_PLAY_WITH_FRIEND_1 == 30
        jintArray jStatuses = (*env)->NewIntArray(env, 30);
        jint *body = (*env)->GetIntArrayElements(env, jStatuses, 0);
        for(int i = 0; i < 30; ++i) {
            body[i] = levelStatuses()[i];
        }
        (*env)->ReleaseIntArrayElements(env, jStatuses, body, 0);
        (*env)->CallVoidMethod(env, stoneballView, mid_saveSettings, languageValue(), jStatuses, privacyPolicyAccepted());
    }

    if(callProfileActivity == 'y') {
        callProfileActivity = 'n';
        (*env)->CallVoidMethod(env, stoneballView, mid_callProfileActivity);
    }

    if(callAboutActivity == 'y') {
        callAboutActivity = 'n';
        cls = (*env)->GetObjectClass(env, stoneballView);
        if(!mid_callAboutActivity) {
            mid_callAboutActivity = (*env)->GetMethodID(env, cls, "openAboutActivity", "()V");
            if(mid_callAboutActivity == 0) {
                LOGE("Method callAboutActivity not found");
                return;
            }
        }
        (*env)->CallVoidMethod(env, stoneballView, mid_callAboutActivity);
    }
}

JNIEXPORT void JNICALL Java_com_stoneball_StoneballView_initNativeBitmap(JNIEnv * env, jobject  obj, jobject bitmap,
             jint bitmapId, jint type) {
    int   ret;
    void* pixels;
    AndroidBitmapInfo bitmapInfo;
    Unused(0, obj);
    if ((ret = AndroidBitmap_getInfo(env, bitmap, &bitmapInfo)) < 0) {
        LOGE("AndroidBitmap_getInfo() failed ! error=%d", ret);
        return;
    }

    if (bitmapInfo.format != ANDROID_BITMAP_FORMAT_RGB_565) {
        LOGE("Bitmap format is not RGB_565!");
        return;
    }

     if ((ret = AndroidBitmap_lockPixels(env, bitmap, &pixels)) < 0) {
         LOGE("AndroidBitmap_lockPixels() failed ! error=%d", ret);
     }

    storeBitmap(pixels, bitmapId, bitmapInfo.width, bitmapInfo.height, bitmapInfo.stride, type);

    AndroidBitmap_unlockPixels(env, bitmap);

    //LOGI("Background info has been saved");
}

JNIEXPORT void JNICALL Java_com_stoneball_StoneballView_setLogPath(JNIEnv * env, jobject  obj, jstring path) {
    Unused(0, obj);

    // convert Java string to UTF-8
    const char *utf8Path = (*env)->GetStringUTFChars(env, path, NULL);
    setLogFilePath(utf8Path);
    (*env)->ReleaseStringUTFChars(env, path, utf8Path);
    LOGI("DBG Log path: %s", utf8Path);
}

JNIEXPORT void JNICALL Java_com_stoneball_StoneballView_calcZoomRate(JNIEnv * env, jobject  obj, jint actualWidth, jint actualHeight, jint designWidth, jint designHeight) {
    Unused(0, env, obj);
    //LOGI("Width zoom rate: %f, height zoom rate: %f", withZoomRate, heightZoomRate);
    //setZoomRate(withZoomRate, heightZoomRate);
    calcZoomRate(actualWidth, actualHeight, designWidth, designHeight);
}

JNIEXPORT void JNICALL Java_com_stoneball_StoneballView_onTouchEventNative(JNIEnv * env, jobject  obj, jint action, jdouble x, jdouble y, jobject stoneballView) {
    Unused(0, env, obj);

    fillJavaFunctionPointers(env, stoneballView);
    onTouchEvent(action, x, y, now_ms());
}

JNIEXPORT void JNICALL Java_com_stoneball_StoneballView_onTouchEventNativeDebug(JNIEnv * env, jobject  obj, jint action, jint actionMasked, jint actionIndex) {
    Unused(0, env, obj);
    debug("action: %d, actionMasked: %d, actionIndex: %d", (int)action, (int)actionMasked, (int)actionIndex);
}

JNIEXPORT void JNICALL Java_com_stoneball_StoneballView_setLogFileDescriptor(JNIEnv * env, jobject  obj, jint fd) {
    const char* test = "halihó!\n";
    Unused(0, env, obj);
    setLogFileDescriptor(fd);
    write(fd, test, strlen(test));
//    close(fd);
}

JNIEXPORT jint JNICALL Java_com_stoneball_StoneballView_getBitmapCount(JNIEnv * env, jobject  obj) {
    Unused(0, env, obj);
    return LAST_ONE;
}

JNIEXPORT void JNICALL Java_com_stoneball_Stoneball_wakeUp(JNIEnv * env, jobject  obj) {
    Unused(0, env, obj);
    wakeUp();
}

JNIEXPORT void JNICALL Java_com_stoneball_StoneballView_setPixelPerfectZoom(JNIEnv * env, jobject  obj, jboolean on) {
    Unused(0, env, obj);
    setPixelPerfectZoom(on);
}

JNIEXPORT void JNICALL Java_com_stoneball_StoneballView_syncSettingsValues(JNIEnv * env, jobject  obj, jint language, jint musicVolume, jint effectsVolume,
    jintArray levelStatus, jbyteArray profileName, jboolean privacyPolicyAccepted) {

    Unused(0, env, obj);
    //jsize len = (*env)->GetArrayLength(env, levelStatus);
    jint *body = (*env)->GetIntArrayElements(env, levelStatus, 0);

    //const jchar* jcharProfileName = (*env)->GetStringChars(env, profileName, NULL);
    //const char* utf8ProfileName = (*env)->GetStringUTFChars(env, profileName, NULL);
    const wchar_t *buf = (wchar_t*) (*env)->GetByteArrayElements(env, profileName, 0);
    int n = (*env)->GetArrayLength(env, profileName) / sizeof(wchar_t);

    syncSettingsValues(language, musicVolume, effectsVolume, body, buf, n, privacyPolicyAccepted);

    (*env)->ReleaseIntArrayElements(env, levelStatus, body, 0);
    //(*env)->ReleaseStringChars(env, profileName, jcharProfileName);
    //(*env)->ReleaseStringUTFChars(env, profileName, utf8ProfileName);
    (*env)->ReleaseByteArrayElements(env, profileName, (jbyte*) buf, 0);
}

JNIEXPORT void JNICALL Java_com_stoneball_StoneballView_syncProfileName(JNIEnv * env, jobject  obj, jbyteArray profileName) {
    Unused(0, env, obj);
    //jsize len = (*env)->GetArrayLength(env, levelStatus);
/*
    const jchar* jcharProfileName = (*env)->GetStringChars(env, profileName, NULL);
    //const char* utf8ProfileName = (*env)->GetStringUTFChars(env, profileName, NULL);
    syncProfileName((const wchar_t*)jcharProfileName);
    //syncProfileName(utf8ProfileName);

    (*env)->ReleaseStringChars(env, profileName, jcharProfileName);
    //(*env)->ReleaseStringUTFChars(env, profileName, utf8ProfileName);
    */
    const wchar_t *buf = (wchar_t*) (*env)->GetByteArrayElements(env, profileName, 0);
    int n = (*env)->GetArrayLength(env, profileName) / sizeof(wchar_t);

    // First byte is BOM (0xfeff), so we skip it, hence the "buf + 1".
    // There IS NO null-terminator.
    //std::wstring ret(buf + 1, buf + n);
    syncProfileName(buf, n);

    (*env)->ReleaseByteArrayElements(env, profileName, (jbyte*) buf, 0);
}

//void javaDrawGameText(int x, int y, const char* text, bool strikethrough, int fontSize) {
void javaDrawGameText(int x, int y, const wchar_t* text, int length, bool strikethrough, int fontSize, int r, int g, int b) {
    Unused(0, x, y, text, fontSize, strikethrough);

    if(globalEnv && length && mid_drawGameText) {
        jstring javaText = (*globalEnv)->NewString(globalEnv, (const jchar*)text, length * 2);
        //jstring javaText = (*globalEnv)->NewStringUTF(globalEnv, text);
        (*globalEnv)->CallVoidMethod(globalEnv, globalStoneballView, mid_drawGameText, x, y, javaText, strikethrough, fontSize, r, g, b);
        (*globalEnv)->DeleteLocalRef(globalEnv, javaText);
    }
}

void javaGameTextSize(const wchar_t* text, int length, int fontSize, int* width, int* height) {
    Unused(0, fontSize, width, height);
    if(globalEnv && text && length && mid_gameTextSize) {
        jstring javaText = (*globalEnv)->NewString(globalEnv, (const jchar*)text, length * 2);
        jintArray retval = (jintArray)(*globalEnv)->CallObjectMethod(globalEnv, globalStoneballView, mid_gameTextSize, javaText, fontSize);

        jint *body = (*globalEnv)->GetIntArrayElements(globalEnv, retval, 0);
        *width = body[0];
        *height = body[1];
        (*globalEnv)->ReleaseIntArrayElements(globalEnv, retval, body, 0);

        (*globalEnv)->DeleteLocalRef(globalEnv, javaText);
    }
}

void javaSetFontBold(bool bold) {
    if(globalEnv && mid_gameFontBold) {
        (*globalEnv)->CallVoidMethod(globalEnv, globalStoneballView, mid_gameFontBold, bold);
    }
}

void javaOpenWebBrowser(const char* text) {
    if(globalEnv && mid_openBrowser && text) {
        jstring javaText = (*globalEnv)->NewStringUTF(globalEnv, text);
        (*globalEnv)->CallVoidMethod(globalEnv, globalStoneballView, mid_openBrowser, javaText);
        (*globalEnv)->DeleteLocalRef(globalEnv, javaText);
    }
}
