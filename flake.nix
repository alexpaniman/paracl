{
  description = "Paracl programming language.";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs = { self, nixpkgs, flake-utils }:
    flake-utils.lib.eachDefaultSystem (system:
      let
        pkgs = import nixpkgs { inherit system; };
      in rec
      {
        packages = {
          default = pkgs.clangStdenv.mkDerivation {
            name = "paracl";
            src = ./.;

            nativeBuildInputs = with pkgs; [ clang cmake ];
            buildInputs = with pkgs; [];

            cmakeFlags = [
              "-DCMAKE_BUILD_TYPE=Release"
            ];
          };

        };

        devShell = pkgs.mkShell {
          packages = [
            pkgs.clang-tools
            pkgs.mold
            pkgs.ninja
          ] ++ packages.default.nativeBuildInputs;
        };
      }
    );
}
