{ pkgs ? import <inxpkgs> { }, wlroots_0_17_src ? null , nix-filter }:
let
  mkDate = longDate: (pkgs.lib.concatStringsSep "-" [
    (builtins.substring 0 4 longDate)
    (builtins.substring 4 2 longDate)
    (builtins.substring 6 2 longDate)
  ]);
in
rec {
  wlroots-git = (pkgs.wlroots_0_16.override {
    wayland-protocols = pkgs.wayland-protocols.overrideAttrs ( old : {
      version = "1.32.0";
      src = pkgs.fetchurl {
        url = "https://gitlab.freedesktop.org/wayland/wayland-protocols/-/releases/1.32/downloads/wayland-protocols-1.32.tar.xz";
        hash = "sha256-dFl5nTQMgpa2le+FfAfd7yTFoJsJq2p097kmQNKxuhE=";
      };
    });
  }).overrideAttrs (
    old: {
      version =  mkDate (wlroots_0_17_src.lastModifiedDate or "19700101") + "_" + (wlroots_0_17_src.shortRev or "dirty");
      src = wlroots_0_17_src;
      buildInputs = old.buildInputs ++ (with pkgs; [ 
        hwdata 
        libdisplay-info
      ]);
      postPatch = ""; # don't need patch hwdata path in wlroots 0.17
    }
  );

  qwlroots-qt6 = pkgs.qt6.callPackage ./nix {
    inherit nix-filter;
    wlroots = pkgs.wlroots_0_16;
  };
  
  qwlroots-qt5 = pkgs.libsForQt5.callPackage ./nix {
    inherit nix-filter;
    wlroots = pkgs.wlroots_0_16;
  };

  qwlroots-qt6-wlroots-git = qwlroots-qt6.override {
    wlroots = wlroots-git;
  };

  qwlroots-qt6-dbg = qwlroots-qt6.override {
    stdenv = pkgs.stdenvAdapters.keepDebugInfo pkgs.stdenv;
  };

  qwlroots-qt6-clang = qwlroots-qt6.override {
    stdenv = pkgs.clangStdenv;
  };

  default = qwlroots-qt6;
}
