{ pkgs ? import <inxpkgs> { }, nix-filter }:

rec {
  qwlroots-qt6 = pkgs.qt6.callPackage ./nix {
    inherit nix-filter;
    wlroots = pkgs.wlroots_0_17;
  };
  
  qwlroots-qt5 = pkgs.libsForQt5.callPackage ./nix {
    inherit nix-filter;
    wlroots = pkgs.wlroots_0_17;
  };

  qwlroots-qt6-wlroots-16 = qwlroots-qt6.override {
    wlroots = pkgs.wlroots_0_16;
  };

  qwlroots-qt6-dbg = qwlroots-qt6.override {
    stdenv = pkgs.stdenvAdapters.keepDebugInfo pkgs.stdenv;
  };

  qwlroots-qt6-clang = qwlroots-qt6.override {
    stdenv = pkgs.clangStdenv;
  };

  default = qwlroots-qt6;
}
