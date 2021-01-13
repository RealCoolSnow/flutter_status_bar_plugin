Status bar plugin for Flutter Desktop.
# Usage
In pubspec.yaml dependencies:
```
  flutter_status_bar:
    git:
      url: https://github.com/RealCoolSnow/flutter_status_bar_plugin.git
      path: flutter_status_bar
```
# API
1. show status bar
```
FlutterStatusBar.showStatusBar();
```
2. hide status bar
```
FlutterStatusBar.hideStatusBar();
```
2. set status bar text
```
FlutterStatusBar.setStatusBarText('hello');
```