
#include <jni.h>
#include <string>
#include <android/log.h>
#include <unistd.h>
#include <fstream>
#include <map>
#include <sstream>

#define LOG_TAG "SpooferZ"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

static std::map<std::string, std::string> ssaidMap;
static std::map<std::string, std::string> imeiMap;
static std::map<std::string, std::string> macMap;
static std::string currentPackage;

void loadConfig() {
    ssaidMap.clear();
    imeiMap.clear();
    macMap.clear();
    std::ifstream file("/data/adb/zygisk_ssaid_spoofer/config.txt");
    if (!file) {
        LOGE("Failed to open config.txt");
        return;
    }
    std::string line;
    while (std::getline(file, line)) {
        size_t eq = line.find('=');
        if (eq != std::string::npos) {
            std::string key = line.substr(0, eq);
            std::string val = line.substr(eq + 1);
            if (key.find(":ssaid") != std::string::npos) {
                ssaidMap[key.substr(0, key.find(":"))] = val;
            } else if (key.find(":imei") != std::string::npos) {
                imeiMap[key.substr(0, key.find(":"))] = val;
            } else if (key.find(":mac") != std::string::npos) {
                macMap[key.substr(0, key.find(":"))] = val;
            }
        }
    }
    LOGI("Config loaded: SSAID[%lu], IMEI[%lu], MAC[%lu]", ssaidMap.size(), imeiMap.size(), macMap.size());
}

extern "C" jint JNI_OnLoad(JavaVM* vm, void* reserved) {
    loadConfig();
    LOGI("JNI_OnLoad called - hooks would be installed here.");
    return JNI_VERSION_1_6;
}
