
#pragma once
#include <jni.h>
#include <string>

namespace zygisk {

class Api;
class AppSpecializeArgs {
public:
    const char *package_name;
};

class Module {
public:
    virtual void onLoad(Api *api, JNIEnv *env) {}
    virtual void preAppSpecialize(const AppSpecializeArgs *args) {}
    virtual void postAppSpecialize(const AppSpecializeArgs *args) {}
    virtual ~Module() = default;
};

} // namespace zygisk

#define REGISTER_ZYGISK_MODULE(CLASS)     extern "C" [[gnu::visibility("default")]] void zygisk_module_register(zygisk::Module **mod) {         static CLASS instance;         *mod = &instance;     }
