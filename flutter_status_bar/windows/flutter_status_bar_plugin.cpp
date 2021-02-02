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
#define _CRT_SECURE_NO_WARNINGS

#define GWL_HWNDPARENT (-8)

#define WIN_WIDTH 180
#define WIN_HEIGHT 24
#define WIN_BOTTOM 100

std::wstring m_text = L"...";

namespace
{
  using std::string;
  using std::wstring;

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
    TextOut(hdc, 0, 0, m_text.c_str(), static_cast<int>(m_text.size()));
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

RECT GetSafeDesktopRect()
{
  RECT desktopRect;
  HWND hwnd = GetDesktopWindow();
  GetWindowRect(hwnd, &desktopRect);

  return desktopRect;
}
std::string GetTextArgument(const flutter::MethodCall<> &method_call)
{
  std::string text;
  const auto *arguments = std::get_if<flutter::EncodableMap>(method_call.arguments());
  if (arguments)
  {
    auto url_it = arguments->find(flutter::EncodableValue("text"));
    if (url_it != arguments->end())
    {
      text = std::get<std::string>(url_it->second);
    }
  }
  return text;
}

wstring string2wstring(string str)
{
  wstring result;
  int len = MultiByteToWideChar(CP_ACP, 0, str.c_str(), static_cast<int>(str.size()), NULL, 0);
  TCHAR *buffer = new TCHAR[len + 1];
  MultiByteToWideChar(CP_ACP, 0, str.c_str(), static_cast<int>(str.size()), buffer, len);
  buffer[len] = '\0';
  result.append(buffer);
  delete[] buffer;
  return result;
}

bool FlutterStatusBarPlugin::ShowStatusBar(const flutter::MethodCall<flutter::EncodableValue> &method_call)
{
  std::string text = GetTextArgument(method_call);
  if (!text.empty())
  {
    m_text = string2wstring(text);
  }
  if (m_hWnd == NULL)
  {
    WNDCLASSA windowClass = {0};
    windowClass.lpszClassName = "flutter_status_bar_wnd";
    windowClass.lpfnWndProc = WindowProc;
    windowClass.hbrBackground = 0;
    windowClass.style = CS_HREDRAW | CS_VREDRAW;

    RegisterClassA(&windowClass);

    RECT desktopRect = GetSafeDesktopRect();
    RECT rect = {desktopRect.right - WIN_WIDTH, desktopRect.bottom - WIN_HEIGHT - WIN_BOTTOM, desktopRect.right, desktopRect.bottom - WIN_BOTTOM};
    // HMODULE hInstance = GetModuleHandle(NULL);
    m_hWnd = CreateWindowExW(
        WS_EX_TOPMOST | WS_EX_TRANSPARENT,
        L"flutter_status_bar_wnd",
        L"flutter_status_bar_wnd",
        WS_POPUPWINDOW, rect.left, rect.top,
        rect.right - rect.left, rect.bottom - rect.top,
        0, 0,
        NULL, windowClass.hInstance);

    SetWindowLong(m_hWnd, GWL_EXSTYLE, GetWindowLong(m_hWnd, GWL_EXSTYLE) & ~WS_EX_APPWINDOW | WS_EX_TOOLWINDOW);
    //SetWindowPos(m_hWnd, NULL, 0, 0, 0, 0, SWP_FRAMECHANGED);
    // COLORREF maskColor = RGB(111, 111, 111);
    // SetLayeredWindowAttributes(m_hWnd, 0, 180, LWA_ALPHA);
    // SetLayeredWindowAttributes(m_hWnd, maskColor, 255, 1);
    // SetLayeredWindowAttributes(m_hWnd, 0, 20, LWA_ALPHA);
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
  if (m_hWnd != NULL)
  {
    std::string text = GetTextArgument(method_call);
    if (!text.empty())
    {
      m_text = string2wstring(text);
    }
    UpdateWindow(m_hWnd);
  }
  return true;
}

void FlutterStatusBarPluginRegisterWithRegistrar(
    FlutterDesktopPluginRegistrarRef registrar)
{
  FlutterStatusBarPlugin::RegisterWithRegistrar(
      flutter::PluginRegistrarManager::GetInstance()
          ->GetRegistrar<flutter::PluginRegistrarWindows>(registrar));
}
