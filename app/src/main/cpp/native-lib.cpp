#include "native-lib.h"
#include <jni.h>
#include <string>
#include <stdlib.h>


static AudioPlayerImpl *example = NULL;

extern "C"
JNIEXPORT jstring
JNICALL
Java_com_asuscomm_yangyinetwork_player_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}


extern "C"
JNIEXPORT void
Java_com_asuscomm_yangyinetwork_player_MainActivity_audioStart(JNIEnv *javaEnvironment, jobject __unused obj, jint samplerate, jint buffersize, jstring apkPath, jint fileOffset, jint fileLength) {
    const char *path = javaEnvironment->GetStringUTFChars(apkPath, JNI_FALSE);
    example = new AudioPlayerImpl((unsigned int)samplerate, (unsigned int)buffersize, path, fileOffset, fileLength);
    javaEnvironment->ReleaseStringUTFChars(apkPath, path);
}


AudioPlayerImpl::AudioPlayerImpl(unsigned int samplerate, unsigned int buffersize, const char *path, int fileOffset, int fileLength) {
    stereoBuffer = (float *)memalign(16, (buffersize + 16) * sizeof(float) * 2);

    player = new SuperpoweredAdvancedAudioPlayer(&player , playerEventCallback, samplerate, 0);

    player->open(path, fileOffset, fileLength);

    audioSystem = new SuperpoweredAndroidAudioIO(samplerate, buffersize, false, true, audioProcessing, this, -1, SL_ANDROID_STREAM_MEDIA, buffersize * 2);
}