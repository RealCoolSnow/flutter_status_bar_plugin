#include "include/flutter_status_bar/flutter_status_bar_plugin.h"

// This must be included before many other Windows headers.
#include <windows.h>

// For getPlatformVersion; remove unless needed for your plugin implementation.
#include <VersionHelpers.h>

#include <flutter/method_channel.h>
#include <flutter/plugin_registrar_windows.h>
#include <flutter/standard_method_codec.h>

#include <map>
#include <memory>
#include <sstream>

namespace
{

  class FlutterStatusBarPlugin : public flutter::Plugin
  {
  public:
    static void RegisterWithRegistrar(flutter::PluginRegistrarWindows *registrar);

    FlutterStatusBarPlugin();

    virtual ~FlutterStatusBarPlugin();

  private:
    // Called when a method is called on this plugin's channel from Dart.
    void HandleMethodCall(
        const flutter::MethodCall<flutter::EncodableValue> &method_call,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);

    bool ShowStatusBar(const flutter::MethodCall<flutter::EncodableValue> &method_call);

    bool HideStatusBar();

    bool IsShown();

    bool SetStatusBarText(const flutter::MethodCall<flutter::EncodableValue> &method_call);
  };

  // static
  void FlutterStatusBarPlugin::RegisterWithRegistrar(
      flutter::PluginRegistrarWindows *registrar)
  {
    auto channel =
        std::make_unique<flutter::MethodChannel<flutter::EncodableValue>>(
            registrar->messenger(), "plugins.coolsnow/flutter_status_bar_method_channel",
            &flutter::StandardMethodCodec::GetInstance());

    auto plugin = std::make_unique<FlutterStatusBarPlugin>();

    channel->SetMethodCallHandler(
        [plugin_pointer = plugin.get()](const auto &call, auto result) {
          plugin_pointer->HandleMethodCall(call, std::move(result));
        });

    registrar->AddPlugin(std::move(plugin));
  }

  FlutterStatusBarPlugin::FlutterStatusBarPlugin() {}

  FlutterStatusBarPlugin::~FlutterStatusBarPlugin() {}

  void FlutterStatusBarPlugin::HandleMethodCall(
      const flutter::MethodCall<flutter::EncodableValue> &method_call,
      std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
  {
    if (method_call.method_name().compare("getPlatformVersion") == 0)
    {
      std::ostringstream version_stream;
      version_stream << "Windows ";
      if (IsWindows10OrGreater())
      {
        version_stream << "10+";
      }
      else if (IsWindows8OrGreater())
      {
        version_stream << "8";
      }
      else if (IsWindows7OrGreater())
      {
        version_stream << "7";
      }
      result->Success(flutter::EncodableValue(version_stream.str()));
    }
    else if (method_call.method_name().compare("showStatusBar") == 0)
    {
      result->Success(this->ShowStatusBar(method_call));
    }
    else if (method_call.method_name().compare("hideStatusBar") == 0)
    {
      result->Success(this->HideStatusBar());
    }
    else if (method_call.method_name().compare("setStatusBarText") == 0)
    {
      result->Success(this->SetStatusBarText(method_call));
    }
    else if (method_call.method_name().compare("isShown") == 0)
    {
      result->Success(this->IsShown());
    }
    else
    {
      result->NotImplemented();
    }
  }
} // namespace

bool FlutterStatusBarPlugin::ShowStatusBar(const flutter::MethodCall<flutter::EncodableValue> &method_call)
{
  return true;
}

bool FlutterStatusBarPlugin::HideStatusBar()
{
  return true;
}

bool FlutterStatusBarPlugin::IsShown()
{
  return true;
}

bool FlutterStatusBarPlugin::SetStatusBarText(const flutter::MethodCall<flutter::EncodableValue> &method_call)
{
  return true;
}

void FlutterStatusBarPluginRegisterWithRegistrar(
    FlutterDesktopPluginRegistrarRef registrar)
{
  FlutterStatusBarPlugin::RegisterWithRegistrar(
      flutter::PluginRegistrarManager::GetInstance()
          ->GetRegistrar<flutter::PluginRegistrarWindows>(registrar));
}
