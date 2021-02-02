// You have generated a new plugin project without
// specifying the `--platforms` flag. A plugin project supports no platforms is generated.
// To add platforms, run `flutter create -t plugin --platforms <platforms> .` under the same
// directory. You can also find a detailed instruction on how to add platforms in the `pubspec.yaml` at https://flutter.dev/docs/development/packages-and-plugins/developing-packages#plugin-platforms.

import 'dart:async';
import 'dart:io';

import 'package:flutter/services.dart';

class FlutterStatusBar {
  static const MethodChannel _methodChannel =
      const MethodChannel('plugins.coolsnow/flutter_status_bar_method_channel');
  static const EventChannel _eventChannel =
      const EventChannel('plugins.coolsnow/flutter_status_bar_event_channel');

  static Future<bool> showStatusBar(String text) async {
    final bool result = await (Platform.isWindows
        ? _methodChannel.invokeMethod('showStatusBar', {'text': text})
        : _methodChannel.invokeMethod('showStatusBar', text));
    return result;
  }

  static Future<bool> hideStatusBar() async {
    final bool result = await _methodChannel.invokeMethod('hideStatusBar');
    return result;
  }

  static Future<bool> setStatusBarText(String text) async {
    final bool result = await (Platform.isWindows
        ? _methodChannel.invokeMethod('setStatusBarText', {'text': text})
        : _methodChannel.invokeMethod('setStatusBarText', text));
    return result;
  }

  static Future<bool> isShown() async {
    final bool result = await _methodChannel.invokeMethod('isShown');
    return result;
  }
}
