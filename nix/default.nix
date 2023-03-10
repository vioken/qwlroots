{ stdenv
, lib
, fetchFromGitHub
, cmake
, pkg-config
, qtbase
, wrapQtAppsHook
, wlroots
, wayland
, wayland-protocols
, wayland-scanner
, pixman
, mesa
, vulkan-loader
, libinput
, xorg
, seatd
}:
stdenv.mkDerivation rec {
  pname = "qwlroots";
  version = "0.0.1";

  src = ./..;

  nativeBuildInputs = [
    cmake
    pkg-config
    wrapQtAppsHook
    wayland-scanner
  ];

  buildInputs = [
    qtbase
    wlroots
    wayland
    wayland-protocols
    pixman
    mesa
    vulkan-loader
    libinput
    xorg.libXdmcp
    xorg.xcbutilerrors
    seatd
  ];

  cmakeFlags = [
    "-DUSE_QT5=${if lib.versionOlder qtbase.version "6.0.0" then "ON" else "OFF"}"
  ];

  meta = with lib; {
    description = "Qt and QML bindings for wlroots";
    homepage = "https://github.com/vioken/qwlroots";
    license = licenses.gpl3Plus;
    platforms = platforms.linux;
  };
}

