cp -r ../assets/* assets/
cp -r ../*.cpp .
cp -r ../include/*.hpp . 

aarch64-linux-android29-clang++ -c ../raylib/projects/VS2019-Android/raylib_android/raylib_android.NativeActivity/android_native_app_glue.c -o obj/native_app_glue.o -std=c++20 -march=armv8-a -mfix-cortex-a53-835769 -ffunction-sections -funwind-tables -fstack-protector-strong -fPIC -Wall -Wa,--noexecstack -Wformat -Werror=format-security -no-canonical-prefixes -DANDROID -DPLATFORM_ANDROID -D__ANDROID_API__=29

llvm-ar rcs obj/libnative_app_glue.a obj/native_app_glue.o

aarch64-linux-android29-clang++ -c *.cpp -I. -I../raylib/src -I../raylib/projects/VS2019-Android/raylib_android/raylib_android.NativeActivity/ -std=c++20 -march=armv8-a -mfix-cortex-a53-835769 -ffunction-sections -funwind-tables -fstack-protector-strong -fPIC -Wall -Wa,--noexecstack -Wformat -Werror=format-security -no-canonical-prefixes -DANDROID -DPLATFORM_ANDROID -D__ANDROID_API__=29 --sysroot=$ANDROID_NDK_ROOT/toolchains/llvm/prebuilt/darwin-x86_64/sysroot

mv *.o obj

aarch64-linux-android29-clang++ -o lib/arm64-v8a/libkungfu.so obj/game.o obj/main.o obj/player.o obj/sprite.o obj/stage.o obj/timer.o -shared -I. -I../raylib/src -I../raylib/projects/VS2019-Android/raylib_android/raylib_android.NativeActivity -Wl,-soname,libproject.so -Wl,--exclude-libs,libatomic.a -Wl,--build-id -Wl,--no-undefined -Wl,-z,noexecstack -Wl,-z,relro -Wl,-z,now -Wl,--warn-shared-textrel -Wl,--fatal-warnings -u ANativeActivity_onCreate -L. -Lobj -Llib/arm64-v8a -lraylib -lnative_app_glue -llog -landroid -lEGL -lGLESv2 -lOpenSLES -latomic -lc -lm -ldl

aapt package -f -m -S res -J src -M AndroidManifest.xml -I ${ANDROID_SDK_ROOT}/platforms/android-29/android.jar

javac -verbose -source 1.7 -target 1.7 -d obj -bootclasspath `/usr/libexec/java_home`/jre/lib/rt.jar -classpath ${ANDROID_SDK_ROOT}/platforms/android-29/android.jar:obj -sourcepath src src/com/ferdinandsilva/kungfu/R.java src/com/ferdinandsilva/kungfu/NativeLoader.java

zip -r obj.zip obj

d8 obj.zip --output obj.zip

unzip obj.zip

mv classes.dex dex/

aapt package -f -M AndroidManifest.xml -S res -A assets -I ${ANDROID_SDK_ROOT}/platforms/android-29/android.jar -F kungfu.unsigned.apk dex

cp $ANDROID_NDK_ROOT/toolchains/llvm/prebuilt/darwin-x86_64/sysroot/usr/lib/aarch64-linux-android/libc++_shared.so lib/arm64-v8a

aapt add kungfu.unsigned.apk lib/arm64-v8a/libkungfu.so 
aapt add kungfu.unsigned.apk lib/arm64-v8a/libc++_shared.so

zipalign -v 4 kungfu.unsigned.apk kungfu.unsigned-align.apk

apksigner sign --ks kungfu.keystore --ks-pass pass:143135Varsitarian --key-pass pass:143135Varsitarian --ks-key-alias projectKey --out kungfu.apk kungfu.unsigned-align.apk

apksigner verify -verbose kungfu.apk

adb install -r kungfu.apk
