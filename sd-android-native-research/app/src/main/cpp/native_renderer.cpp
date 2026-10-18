#include <jni.h>
#include <GLES3/gl3.h>
#include <android/log.h>
#include <algorithm>
#include <cmath>

#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, "SDNativeResearch", __VA_ARGS__)

static int gWidth = 1;
static int gHeight = 1;

extern "C" JNIEXPORT void JNICALL
Java_org_sdandroid_nativeport_MainActivity_nativeInit(JNIEnv*, jclass) {
    const GLubyte* version = glGetString(GL_VERSION);
    const GLubyte* renderer = glGetString(GL_RENDERER);
    LOGI("OpenGL ES version: %s", version ? reinterpret_cast<const char*>(version) : "unknown");
    LOGI("GPU renderer: %s", renderer ? reinterpret_cast<const char*>(renderer) : "unknown");
    glDisable(GL_DEPTH_TEST);
}

extern "C" JNIEXPORT void JNICALL
Java_org_sdandroid_nativeport_MainActivity_nativeResize(JNIEnv*, jclass, jint w, jint h) {
    gWidth = std::max(1, static_cast<int>(w));
    gHeight = std::max(1, static_cast<int>(h));
    glViewport(0, 0, gWidth, gHeight);
    LOGI("Surface resized: %dx%d", gWidth, gHeight);
}

extern "C" JNIEXPORT void JNICALL
Java_org_sdandroid_nativeport_MainActivity_nativeRender(JNIEnv*, jclass, jfloat seconds) {
    const float pulse = 0.5f + 0.5f * std::sin(seconds * 0.7f);
    glClearColor(0.025f + pulse * 0.025f, 0.045f, 0.075f + pulse * 0.04f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

extern "C" JNIEXPORT void JNICALL
Java_org_sdandroid_nativeport_MainActivity_nativeTouch(JNIEnv*, jclass, jfloat x, jfloat y, jint action) {
    LOGI("Touch action=%d at %.1f, %.1f", action, x, y);
}
