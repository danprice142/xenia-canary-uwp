/**
 ******************************************************************************
 * Xenia : Xbox 360 Emulator Research Project                                 *
 ******************************************************************************
 * Copyright 2026 Xenia Canary. All rights reserved.                          *
 * Released under the BSD license - see LICENSE in the root for more details. *
 ******************************************************************************
 */

#include "xenia-canary-uwp/UWPUtil.h"
#include "xenia-canary-uwp/window_uwp.h"

#include "xenia/emulator.h"

#include <cstdlib>
#include <filesystem>
#include <tuple>

#pragma comment(lib, "runtimeobject.lib")

namespace UWP {

namespace {

std::string GetDesktopLocalStateRoot() {
  const char* local_app_data = std::getenv("LOCALAPPDATA");
  std::string root =
      local_app_data && local_app_data[0] ? local_app_data : ".";
  root += "\\XeniaCanaryUWPDesktop";
  std::filesystem::create_directories(root);
  return root;
}

}  // namespace

void SelectGameFromWinRT(xe::Emulator*) {}
bool HasGamePath() { return false; }
std::vector<std::tuple<std::string, std::string>> GetGames() { return {}; }
std::vector<std::string> GetPaths() { return {}; }
void SetGamePaths(std::vector<std::string>) {}
void SelectFolder(std::function<void(std::string)> callback) { callback(""); }
void SelectFile(std::function<void(std::string)> callback) { callback(""); }
void SelectFiles(std::function<void(std::vector<std::string>)> callback) {
  callback({});
}
bool TestPathPermissions(std::string) { return true; }
std::string GetLocalCache() {
  auto cache = GetDesktopLocalStateRoot() + "\\cache";
  std::filesystem::create_directories(cache);
  return cache;
}
std::string GetLocalState() { return GetDesktopLocalStateRoot(); }
int GetCoreDPI() { return 96; }
void SetAutomaticLaunch(std::string) {}
void SetDPI(int) {}
bool IsUIOpen() { return false; }
void SetUIOpen(bool) {}
void LaunchUri(const std::string&) {}
void DownloadAndExtractZip(const std::string&, const std::string&,
                           std::function<void(bool, std::string)> callback) {
  callback(false, "Downloads are only available in the UWP frontend.");
}
bool IsDownloadInProgress() { return false; }
float GetDownloadProgress() { return 0.0f; }
std::string GetTitleIdFromPath(const std::string&) { return ""; }
void DownloadPatchesForGame(const std::string&, const std::string&,
                            std::function<void(bool, std::string)> callback) {
  callback(false, "Downloads are only available in the UWP frontend.");
}
void DownloadPluginsForGame(const std::string&, const std::string&,
                            std::function<void(bool, std::string)> callback) {
  callback(false, "Downloads are only available in the UWP frontend.");
}
void DownloadConfigForGame(const std::string&, const std::string&,
                           std::function<void(bool, std::string)> callback) {
  callback(false, "Downloads are only available in the UWP frontend.");
}
std::string GetMediaIdFromPath(const std::string&) { return ""; }
void EnsureUnityMetadataFetch(const std::string&) {}
bool TryGetUnityMetadata(const std::string&, UnityGameMetadata*) {
  return false;
}
void DownloadTitleUpdatesForGame(
    const std::string&, const std::string&, const std::string&,
    std::function<void(bool, std::vector<std::string>, std::string)> callback) {
  callback(false, {}, "Downloads are only available in the UWP frontend.");
}
void ShowKeyboard() {}

}  // namespace UWP

namespace xe {
namespace ui {

UWPWindow::UWPWindow(WindowedAppContext& app_context,
                     const std::string_view title, uint32_t width,
                     uint32_t height)
    : Window(app_context, title, width, height) {}

UWPWindow::~UWPWindow() = default;
void UWPWindow::SetXInputDriver(xe::hid::xinput::XInputInputDriver* driver) {
  input_driver = driver;
}
xe::hid::xinput::XInputInputDriver* UWPWindow::xinputdriver() {
  return input_driver;
}
void UWPWindow::ClearXInputDriver() { input_driver = nullptr; }
uint32_t UWPWindow::GetLatestDpiImpl() const { return 96; }
std::unique_ptr<Surface> UWPWindow::CreateSurfaceImpl(
    Surface::TypeFlags) {
  return nullptr;
}
bool UWPWindow::OpenImpl() { return false; }
void UWPWindow::RequestCloseImpl() {}
void UWPWindow::FocusImpl() {}
void UWPWindow::RequestPaintImpl() {}

}  // namespace ui
}  // namespace xe
