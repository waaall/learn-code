# learn Dart/Flutter


## 参考
* [官方文档](https://flutter.cn/docs)
* [样例应用](https://flutter.github.io/samples/#)

## 语言细节
* [dart官方教程](https://dart.cn/guides/language/language-tour)


## Flutter-App

* [first-flutter-app](https://codelabs.developers.google.com/codelabs/first-flutter-app-pt2#2)

```bash
# 这等同于再vs code的command palette中搜索flutter: new project
flutter create your_app_name
cd your_app_name

```

### package设置

[dart-package网站](https://pub.flutter-io.cn)

[将 package 依赖添加到应用: ](https://flutter.cn/docs/development/packages-and-plugins/using-packages)

> To add the package, css_colors, to an app:

1. 添加依赖

打开应用文件夹下的 pubspec.yaml 文件，然后在 pubspec.yaml 下添加 css_colors:。

Open the pubspec.yaml file located inside the app folder, and add css_colors: under dependencies.

2. 安装

在命令行中运行：flutter pub get

From the terminal: Run flutter pub get

或者

在 Android Studio/IntelliJ 中点击 pubspec.yaml 文件顶部操作功能区的 Packages get

From Android Studio/IntelliJ: Click Packages get in the action ribbon at the top of pubspec.yaml.

在 VS Code 中点击位于 pubspec.yaml 文件顶部操作功能区右侧的 Get Packages

3. 导入

在 Dart 代码中添加相关的 import 语句。

如果有必要，停止并重启应用

如果 package 内有特定平台的代码（Android 的 Java/Kotlin, iOS 的 Swift/Objective-C），代码必须内置到你的应用内。热重载和热重启只对 package 的 Dart 代码执行此操作，所以你需要完全重启应用以避免使用 package 时出现 MissingPluginException 错误。

If the package brings platform-specific code (Kotlin/Java for Android, Swift/Objective-C for iOS), that code must be built into your app. Hot reload and hot restart only update the Dart code, so a full restart of the app might be required to avoid errors like MissingPluginException when using the package.

对于这些步骤，Pub 上任何 package 页面的 Installing tab 选项卡都是一个很方便的参考。

完整示例，参阅下面的 css_colors 示例 。


### [网页App实践](https://flutter.cn/docs/get-started/codelab-web)

