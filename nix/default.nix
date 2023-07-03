{ stdenv
, lib
, nix-filter
, fetchFromGitHub
, cmake
, pkg-config
, qtbase
, wrapQtAppsHook
, wlroots
, wayland
, wayland-protocols
, wlr-protocols
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

  src = nix-filter.lib.filter {
    root = ./..;

    exclude = [
      ".git"
      "LICENSE"
      "README.md"
      "README.zh_CN.md"
      (nix-filter.lib.matchExt "nix")
    ];
  };

  nativeBuildInputs = [
    cmake
    pkg-config
    wrapQtAppsHook
    wayland-scanner
  ];

  buildInputs = [
    qtbase
    wayland
    wayland-protocols
    wlr-protocols
    pixman
    mesa
    vulkan-loader
    libinput
    xorg.libXdmcp
    xorg.xcbutilerrors
    seatd
  ];

  propagatedBuildInputs = [
    wlroots
  ];

  meta = with lib; {
    description = "Qt and QML bindings for wlroots";
    homepage = "https://github.com/vioken/qwlroots";
    license = licenses.gpl3Plus;
    platforms = platforms.linux;
  };
}

