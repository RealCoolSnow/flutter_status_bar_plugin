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

#define W 100
#define H 20

namespace
{

  class FlutterStatusBarPlugin : public flutter::Plugin
  {
  public:
    static void RegisterWithRegistrar(flutter::PluginRegistrarWindows *registrar);

    FlutterStatusBarPlugin();

    virtual ~FlutterStatusBarPlugin();

  private:
    HWND m_hWnd = NULL;
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

LRESULT CALLBACK WindowProc(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
  switch (message)
  {
  case WM_CREATE:
  {
    //CreateWindow(L"edit", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER, 0, 30, 73, 20, window, NULL, NULL, NULL);
    break;
  }
  case WM_PAINT:
  {
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(window, &ps);
    TextOut(hdc, 0, 0, L"abc", 3);
    EndPaint(window, &ps);
    break;
  }
  case WM_NCHITTEST:
  {
    LRESULT hit = DefWindowProc(window, message, wparam, lparam);
    if (hit == HTCLIENT)
    {
      hit = HTCAPTION;
    }
    return hit;
  }
  case WM_CLOSE:
    PostQuitMessage(0);
    break;
  default:
    return DefWindowProcA(window, message, wparam, lparam);
  }
  return 0;
}
bool FlutterStatusBarPlugin::ShowStatusBar(const flutter::MethodCall<flutter::EncodableValue> &method_call)
{
  if (m_hWnd == NULL)
  {
    WNDCLASSA windowClass = {0};
    windowClass.lpszClassName = "flutter_status_bar_wnd";
    windowClass.lpfnWndProc = WindowProc;
    windowClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    windowClass.style = CS_HREDRAW | CS_VREDRAW;

    RegisterClassA(&windowClass);

    RECT R = {0, 0, W, H};
    int width = R.right - R.left;
    int height = R.bottom - R.top;
    // HMODULE hInstance = GetModuleHandle(NULL);
    m_hWnd = CreateWindowExW(
        WS_EX_TOPMOST,
        L"flutter_status_bar_wnd",
        L"flutter_status_bar_wnd",
        WS_POPUPWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        width, height,
        0, 0,
        NULL, windowClass.hInstance);

    LONG lStyle = GetWindowLong(m_hWnd, GWL_STYLE);
    lStyle &= ~(WS_CAPTION | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU);
    SetWindowLong(m_hWnd, GWL_STYLE, lStyle);
  }
  ShowWindow(m_hWnd, SW_SHOW);
  UpdateWindow(m_hWnd);
  return true;
}

bool FlutterStatusBarPlugin::HideStatusBar()
{
  if (m_hWnd != NULL)
  {
    DestroyWindow(m_hWnd);
    m_hWnd = NULL;
  }
  return true;
}

bool FlutterStatusBarPlugin::IsShown()
{
  return m_hWnd != NULL;
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
