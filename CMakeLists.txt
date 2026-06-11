cmake_minimum_required(VERSION 3.21)

project(QtVideoDownloader
  VERSION 0.1.0
  LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Qt6 REQUIRED COMPONENTS Widgets Network)
qt_standard_project_setup()

qt_add_executable(QtVideoDownloader
  src/main.cpp
  src/MainWindow.h
  src/MainWindow.cpp
  src/Theme.h
  src/Theme.cpp
  src/TitleBar.h
  src/TitleBar.cpp
  src/DarkNotifyDialog.h
  src/DarkNotifyDialog.cpp
  src/DownloadItem.h
  src/DownloadItem.cpp
  src/DownloadsModel.h
  src/DownloadsModel.cpp
  src/YtDlpClient.h
  src/YtDlpClient.cpp
  src/pages/NewDownloadPage.h
  src/pages/NewDownloadPage.cpp
  src/pages/QueuePage.h
  src/pages/QueuePage.cpp
  src/pages/SettingsPage.h
  src/pages/SettingsPage.cpp
  src/pages/AboutPage.h
  src/pages/AboutPage.cpp
)

target_include_directories(QtVideoDownloader PRIVATE src)
target_link_libraries(QtVideoDownloader PRIVATE Qt6::Widgets Qt6::Network)

set_target_properties(QtVideoDownloader PROPERTIES
  WIN32_EXECUTABLE TRUE
)
