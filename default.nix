{ pkgs ? import <inxpkgs> { }, wlroots_0_17_src ? null , nix-filter }:
let
  mkDate = longDate: (pkgs.lib.concatStringsSep "-" [
    (builtins.substring 0 4 longDate)
    (builtins.substring 4 2 longDate)
    (builtins.substring 6 2 longDate)
  ]);
in
rec {
  wlroots_0_17 = pkgs.wlroots_0_16.overrideAttrs (
    old: {
      version = "0.17.0";
      src = wlroots_0_17_src;
      buildInputs = old.buildInputs ++ (with pkgs; [ 
        hwdata
        libliftoff
        libdisplay-info
        cairo
      ]);
      postPatch = ""; # don't need patch hwdata path in wlroots 0.17
    }
  );

  qwlroots-qt6 = pkgs.qt6.callPackage ./nix {
    inherit nix-filter;
    wlroots = wlroots_0_17;
  };
  
  qwlroots-qt5 = pkgs.libsForQt5.callPackage ./nix {
    inherit nix-filter;
    wlroots = wlroots_0_17;
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
