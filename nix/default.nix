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

stdenv.mkDerivation (finalAttrs: {
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

  cmakeFlags = [
    "-DPREFER_QT_5=${if lib.versionAtLeast qtbase.version "6" then "OFF" else "ON"}"
  ];

  meta = {
    description = "Qt and QML bindings for wlroots";
    homepage = "https://github.com/vioken/qwlroots";
    license = with lib.licenses; [ gpl3Only lgpl3Only asl20 ];
    platforms = wlroots.meta.platforms;
    maintainers = with lib.maintainers; [ rewine ];
  };
})

