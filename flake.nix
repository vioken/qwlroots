{
  description = "A basic flake to help develop wlroots";

  inputs = {
    flake-utils.url = "github:numtide/flake-utils";
    nixpkgs.url = "github:NixOS/nixpkgs/nixpkgs-unstable";
  };

  outputs = { self, flake-utils, nixpkgs }@input:
    flake-utils.lib.eachSystem [ "x86_64-linux" "aarch64-linux" ]
      (system:
        let
          pkgs = nixpkgs.legacyPackages.${system};
          qwlroots = pkgs.libsForQt5.callPackage ./nix {
            wlroots = pkgs.wlroots_0_16;
          };
        in
        rec {
          packages.default = qwlroots;

          devShell = pkgs.mkShell {
            nativeBuildInputs = qwlroots.nativeBuildInputs ++ (with pkgs; [
              # (weston.override { buildDemo = true; })
              wayland-utils
            ]);
            inherit (qwlroots) buildInputs;
            shellHook = ''
              echo "welcome to qwlroots"
              echo "wlroots: $(pkg-config --modversion wlroots)"
              echo "wayland-server: $(pkg-config --modversion wayland-server)"
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
