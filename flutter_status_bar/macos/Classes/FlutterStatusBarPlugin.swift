import Cocoa
import FlutterMacOS

public class FlutterStatusBarPlugin: NSObject, FlutterPlugin, FlutterStreamHandler {
  private var eventSink: FlutterEventSink?
  var statusBarItem: NSStatusItem!

  public static func register(with registrar: FlutterPluginRegistrar) {
    let channel = FlutterMethodChannel(name: "plugins.coolsnow/flutter_status_bar_method_channel", binaryMessenger: registrar.messenger)
    let event = FlutterEventChannel(name: "plugins.coolsnow/flutter_status_bar_event_channel", binaryMessenger: registrar.messenger)
    
    let instance = FlutterStatusBarPlugin()

    event.setStreamHandler(instance)
    registrar.addMethodCallDelegate(instance, channel: channel)
  }

  public func handle(_ call: FlutterMethodCall, result: @escaping FlutterResult) {
    switch call.method {
    case "showStatusBar":
      showStatusBar(call, result: result)
    case "hideStatusBar":
      hideStatusBar(call, result: result)
    case "setStatusBarText":
      setStatusBarText(call, result: result)
    case "isShown":
      result(self.statusBarItem != nil)
    default:
      result(FlutterMethodNotImplemented)
    }
  }
  private func showStatusBar(_ call: FlutterMethodCall, result: @escaping FlutterResult) {
    self.statusBarItem = NSStatusBar.system.statusItem(withLength: CGFloat(NSStatusItem.variableLength))
    if let button = self.statusBarItem.button {
        let text = call.arguments as! String
        statusBarItem.button?.title = text
        button.action = #selector(showAppWindows(_:))
    }
    result(true)
  }

  private func hideStatusBar(_ call: FlutterMethodCall, result: @escaping FlutterResult) {
    if self.statusBarItem != nil {
      NSStatusBar.system.removeStatusItem(statusBarItem)
    }
    self.statusBarItem = nil
    result(true)
  }
  
  private func setStatusBarText(_ call: FlutterMethodCall, result: @escaping FlutterResult) {
    if self.statusBarItem != nil {
      let text = call.arguments as! String
      statusBarItem.button?.title = text
      result(true)
    } else {
      result(false)
    }
  }

  @objc func showAppWindows(_ sender: AnyObject?) {
    
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
