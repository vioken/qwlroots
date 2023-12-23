{
  description = "A basic flake to help develop qwlroots";

  inputs = {
    flake-utils.url = "github:numtide/flake-utils";
    nix-filter.url = "github:numtide/nix-filter";
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";

    wlroots-17 = {
      url = "gitlab:wlroots/wlroots?host=gitlab.freedesktop.org&ref=0.17.1";
      flake = false;
    };
  };

  outputs = { self, flake-utils, nix-filter, nixpkgs, wlroots-17 }@input:
    flake-utils.lib.eachSystem [ "x86_64-linux" "aarch64-linux" "riscv64-linux" ]
      (system:
        let
          pkgs = nixpkgs.legacyPackages.${system};
        in
        rec {
          packages = import ./default.nix {
            inherit pkgs nix-filter;
            wlroots_0_17_src = wlroots-17;
          };

          devShells.default = pkgs.mkShell { 
            packages = with pkgs; [
              wayland-utils
            ];

            inputsFrom = [
              packages.default
            ];

            shellHook = ''
              echo "welcome to qwlroots"
              echo "wlroots: $(pkg-config --modversion wlroots)"
              echo "wayland-server: $(pkg-config --modversion wayland-server)"
              # unexpected QT_NO_DEBUG form qt-base-hook
              # https://github.com/NixOS/nixpkgs/issues/251918
              export NIX_CFLAGS_COMPILE=$(echo $NIX_CFLAGS_COMPILE | sed 's/-DQT_NO_DEBUG//')
              export QT_LOGGING_RULES="*.debug=true;qt.*.debug=false"
              export MESA_DEBUG=1
              export EGL_LOG_LEVEL=debug
              export LIBGL_DEBUG=verbose
              export WAYLAND_DEBUG=1
            '';
          };
        }
      );
}
