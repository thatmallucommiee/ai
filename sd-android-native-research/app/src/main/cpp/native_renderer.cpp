#include <jni.h>
#include <GLES3/gl3.h>
#include <android/log.h>
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstring>

#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, "SDNativeResearch", __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, "SDNativeResearch", __VA_ARGS__)

static int gWidth = 1;
static int gHeight = 1;
static GLuint gProgram = 0;
static GLuint gVbo = 0;
static GLuint gTexture = 0;
static GLint gPosition = -1;
static GLint gUv = -1;
static GLint gTint = -1;
static GLint gSampler = -1;
static bool gReady = false;

static GLuint compileShader(GLenum type, const char* source) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);
    GLint ok = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &ok);
    if (!ok) {
        char log[1024] = {};
        glGetShaderInfoLog(shader, sizeof(log), nullptr, log);
        LOGE("Shader compile failed: %s", log);
        glDeleteShader(shader);
        return 0;
    }
    return shader;
}

static bool initGraphics() {
    const char* vs =
        "#version 300 es\n"
        "layout(location=0) in vec2 aPos; layout(location=1) in vec2 aUV;\n"
        "out vec2 vUV; void main(){ vUV=aUV; gl_Position=vec4(aPos,0.0,1.0); }";
    const char* fs =
        "#version 300 es\n"
        "precision mediump float; in vec2 vUV; uniform sampler2D uTex; "
        "uniform vec3 uTint; out vec4 frag; "
        "void main(){ vec4 c=texture(uTex,vUV); frag=vec4(c.rgb*uTint,c.a); }";
    GLuint v = compileShader(GL_VERTEX_SHADER, vs);
    GLuint f = compileShader(GL_FRAGMENT_SHADER, fs);
    if (!v || !f) return false;
    gProgram = glCreateProgram();
    glAttachShader(gProgram, v);
    glAttachShader(gProgram, f);
    glLinkProgram(gProgram);
    glDeleteShader(v);
    glDeleteShader(f);
    GLint linked = 0;
    glGetProgramiv(gProgram, GL_LINK_STATUS, &linked);
    if (!linked) {
        char log[1024] = {};
        glGetProgramInfoLog(gProgram, sizeof(log), nullptr, log);
        LOGE("Program link failed: %s", log);
        return false;
    }

    // Interleaved position/UV vertices for a textured triangle.
    const GLfloat vertices[] = {
        -0.72f,-0.62f, 0.0f,0.0f,
         0.72f,-0.62f, 1.0f,0.0f,
         0.00f, 0.72f, 0.5f,1.0f
    };
    glGenBuffers(1, &gVbo);
    glBindBuffer(GL_ARRAY_BUFFER, gVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Small procedural checker texture; no copyrighted game assets.
    const GLubyte pixels[] = {
        235,75,45,255, 35,180,210,255,
        35,180,210,255, 235,190,55,255
    };
    glGenTextures(1, &gTexture);
    glBindTexture(GL_TEXTURE_2D, gTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 2, 2, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

    gPosition = glGetAttribLocation(gProgram, "aPos");
    gUv = glGetAttribLocation(gProgram, "aUV");
    gTint = glGetUniformLocation(gProgram, "uTint");
    gSampler = glGetUniformLocation(gProgram, "uTex");
    gReady = gPosition >= 0 && gUv >= 0 && gTint >= 0 && gSampler >= 0;
    LOGI("Independent GLES3 test: shader=%s texture=%s", gReady ? "ready" : "failed", gTexture ? "created" : "failed");
    return gReady && gTexture != 0 && gVbo != 0;
}

extern "C" JNIEXPORT void JNICALL
Java_org_sdandroid_nativeport_MainActivity_nativeInit(JNIEnv*, jclass) {
    const GLubyte* version = glGetString(GL_VERSION);
    const GLubyte* renderer = glGetString(GL_RENDERER);
    LOGI("OpenGL ES version: %s", version ? reinterpret_cast<const char*>(version) : "unknown");
    LOGI("GPU renderer: %s", renderer ? reinterpret_cast<const char*>(renderer) : "unknown");
    glDisable(GL_DEPTH_TEST);
    initGraphics();
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
    const float pulse = 0.78f + 0.22f * std::sin(seconds * 1.4f);
    glClearColor(0.025f, 0.045f, 0.075f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    if (!gReady) return;
    glUseProgram(gProgram);
    glBindBuffer(GL_ARRAY_BUFFER, gVbo);
    glEnableVertexAttribArray(static_cast<GLuint>(gPosition));
    glEnableVertexAttribArray(static_cast<GLuint>(gUv));
    glVertexAttribPointer(static_cast<GLuint>(gPosition), 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), nullptr);
    glVertexAttribPointer(static_cast<GLuint>(gUv), 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), reinterpret_cast<void*>(2 * sizeof(GLfloat)));
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, gTexture);
    glUniform1i(gSampler, 0);
    glUniform3f(gTint, pulse, pulse, 1.0f);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glDisableVertexAttribArray(static_cast<GLuint>(gPosition));
    glDisableVertexAttribArray(static_cast<GLuint>(gUv));
}

extern "C" JNIEXPORT jboolean JNICALL
Java_org_sdandroid_nativeport_MainActivity_nativeCheckPrivateFile(JNIEnv* env, jclass, jstring path) {
    const char* utf = env->GetStringUTFChars(path, nullptr);
    if (!utf) return JNI_FALSE;
    FILE* file = std::fopen(utf, "rb");
    env->ReleaseStringUTFChars(path, utf);
    if (!file) {
        LOGE("Could not open app-private test file");
        return JNI_FALSE;
    }
    char buffer[64] = {};
    const size_t count = std::fread(buffer, 1, sizeof(buffer) - 1, file);
    std::fclose(file);
    const bool ok = count > 0 && std::strstr(buffer, "SD_NATIVE_PRIVATE_IO_OK") != nullptr;
    LOGI("App-private file read test: %s (%zu bytes)", ok ? "PASS" : "FAIL", count);
    return ok ? JNI_TRUE : JNI_FALSE;
}

extern "C" JNIEXPORT void JNICALL
Java_org_sdandroid_nativeport_MainActivity_nativeTouch(JNIEnv*, jclass, jfloat x, jfloat y, jint action) {
    LOGI("Touch action=%d at %.1f, %.1f", action, x, y);
}
