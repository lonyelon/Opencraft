{
  description = "C++ recreation of Minecraft";

  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs/nixos-unstable";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs = { self, nixpkgs, flake-utils }:
    flake-utils.lib.eachDefaultSystem (system:
      let
        pkgs = import nixpkgs {
          inherit system;
        };

        cmakeProject = pkgs.stdenv.mkDerivation {
          pname = "Opencraft";
          version = "1.0.0";

          src = ./.;

          nativeBuildInputs = [
            pkgs.cmake
            pkgs.gcc
          ];

          buildInputs = [
            pkgs.glfw
            pkgs.libGL
            pkgs.xorg.libX11
            pkgs.libnoise
            pkgs.libGLU
          ];

          buildPhase = ''
            cmake .
            make
          '';

          installPhase = ''
            mkdir -p $out/bin
            cp Opencraft $out/bin
            ls
          '';
        };
      in {
        packages.default = cmakeProject;
        devShells.default = pkgs.mkShellNoCC {
          packages = [
            pkgs.cmake
            pkgs.gcc
            pkgs.glm
            pkgs.glfw
            pkgs.libGL
            pkgs.xorg.libX11
            pkgs.libnoise
            pkgs.libGLU
            pkgs.gdb
          ];
        };
        defaultPackage = cmakeProject;
        apps.default = flake-utils.lib.mkApp {
          drv = cmakeProject;
          name = "Opencraft";
        };
      });
}
