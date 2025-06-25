
#include <zygisk.hpp>
#include <jni.h>
#include <android/log.h>
#include <cstring>
#include <string>
#include <map>
#include <fstream>
#include "xhook.h"

#define LOG_TAG "SpooferZ"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)

static std::string current_pkg;
static std::string spoofed_ssaid, spoofed_imei, spoofed_mac;
static std::map<std::string, std::tuple<std::string, std::string, std::string>> spoof_map;

static const char *(*orig__android_id)(void *, const char *) = nullptr;
static const char *(*orig_getImei)(void *, int) = nullptr;
static const char *(*orig_getMacAddress)(void *) = nullptr;

const char *hook__android_id(void *thiz, const char *key) {
    if (key && strcmp(key, "android_id") == 0) {
        LOGI("Spoofed SSAID returned: %s", spoofed_ssaid.c_str());
        return spoofed_ssaid.c_str();
    }
    return orig__android_id(thiz, key);
}

const char *hook_getImei(void *thiz, int slotId) {
    LOGI("Spoofed IMEI returned: %s", spoofed_imei.c_str());
    return spoofed_imei.c_str();
}

const char *hook_getMacAddress(void *thiz) {
    LOGI("Spoofed MAC returned: %s", spoofed_mac.c_str());
    return spoofed_mac.c_str();
}

void load_spoof_config() {
    std::ifstream fin("/data/adb/modules/zygisknext_ssaid_spoofer/spoof.conf");
    std::string pkg, ssaid, imei, mac;
    while (fin >> pkg >> ssaid >> imei >> mac) {
        spoof_map[pkg] = std::make_tuple(ssaid, imei, mac);
    }
    LOGI("Loaded spoof.conf with %zu entries", spoof_map.size());
}

class SpooferModule : public zygisk::Module {
public:
    void onLoad(zygisk::Api *api, JNIEnv *env) override {
        LOGI("Zygisk onLoad() - loading spoof config");
        load_spoof_config();
    }

    void preAppSpecialize(const zygisk::AppSpecializeArgs *args) override {
        current_pkg = args->package_name;
        LOGI("App launching: %s", current_pkg.c_str());

        if (spoof_map.count(current_pkg)) {
            auto &[ssaid, imei, mac] = spoof_map[current_pkg];
            spoofed_ssaid = ssaid;
            spoofed_imei = imei;
            spoofed_mac = mac;
            LOGI("Spoofing SSAID: %s, IMEI: %s, MAC: %s", ssaid.c_str(), imei.c_str(), mac.c_str());

            xhook_register(".*\\.so$", "getString", (void *)hook__android_id, (void **)&orig__android_id);
            xhook_register(".*\\.so$", "getImei", (void *)hook_getImei, (void **)&orig_getImei);
            xhook_register(".*\\.so$", "getMacAddress", (void *)hook_getMacAddress, (void **)&orig_getMacAddress);
            xhook_refresh(0);
        }
    }
};
REGISTER_ZYGISK_MODULE(SpooferModule)
