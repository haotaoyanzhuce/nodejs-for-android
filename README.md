# nodejs-for-android
将nodejs打包在android app中，可在app中调用本地node，适合离线做android应用，配合vue使用。nodejs版本20.16

################

使用：
使用时将\app\src\main\cpp\libnode\bin\arm64-v8和\app\src\main\assets下的压缩包进行解压，在android studio中直接运行即可。

简述：
libnode.so为nodejs源码与NDK在ubuntu x64平台进行交叉编译得出。
不包含动态库的第三方库大部分可以在android平台上直接使用。
包含动态库的三方库需重新编译。（动态库无法直接使用linux arm64版本的库）
在demo阶段也有其他使用方式。
    React Native 插件
    Cordova 插件

第三方库编译：
编译时可能借助prebuild-for-nodejs-mobile与ndk方式实现构建


################
记录：
在android native c++工程中已将nodejs的log重定向到logcat中。
利用asstes目录将文件复制到指定路径，部分格式文件可能无法安装，例如.splite。可以使用压缩包的方式，安装后解压到指定路径。
demo阶段使用android设备可用，后续在使用redmi时出现过文件复制报错问题，


