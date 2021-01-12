import Cocoa
import FlutterMacOS

public class FlutterStatusBarPlugin: NSObject, FlutterPlugin, FlutterStreamHandler {
  private var eventSink: FlutterEventSink?

  public static func register(with registrar: FlutterPluginRegistrar) {
    let channel = FlutterMethodChannel(name: "plugins.coolsnow/flutter_status_bar_method_channel", binaryMessenger: registrar.messenger)
    let event = FlutterEventChannel(name: "plugins.coolsnow/flutter_status_bar_event_channel", binaryMessenger: registrar.messenger)
    
    let instance = FlutterStatusBarPlugin()

    event.setStreamHandler(instance)
    registrar.addMethodCallDelegate(instance, channel: channel)
  }

  public func handle(_ call: FlutterMethodCall, result: @escaping FlutterResult) {
    switch call.method {
    case "getPlatformVersion":
      result("macOS " + ProcessInfo.processInfo.operatingSystemVersionString)
    default:
      result(FlutterMethodNotImplemented)
    }
  }
  public func onListen(withArguments arguments: Any?, eventSink events: @escaping FlutterEventSink) -> FlutterError? {
    eventSink = events

    return nil
  }

  public func onCancel(withArguments arguments: Any?) -> FlutterError? {
    eventSink = nil
    
    return nil
  }
}
