
# Zygisk SSAID/IMEI/MAC Spoofer..

A Magisk Zygisk module to spoof SSAID, IMEI, and MAC address per app package.

## 📦 Structure

- `zygisk/libssaidspoof.so` - native compiled library (build output)
- `config.txt` - spoof rules (package:field=value)
- `native/` - native source and CMake for Android NDK

## 🛠️ Build Instructions

### Requirements:
- Android NDK (r25 or newer)
- CMake

### Build:

```bash
cd native
mkdir build && cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=$NDK/build/cmake/android.toolchain.cmake \
         -DANDROID_ABI=arm64-v8a \
         -DANDROID_PLATFORM=android-29
make
```

Copy `libssaidspoof.so` to the `zygisk/` directory, zip the repo, and flash in Magisk.

## 🔧 config.txt Format

```
com.example.app:ssaid=0123456789abcdef
com.example.app:imei=865438021234567
com.example.app:mac=00:11:22:33:44:55
```

You can spoof only specific fields by leaving others out.
