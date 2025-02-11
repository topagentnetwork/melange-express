{
  description = "melange-express";

  inputs = {
    flake-utils.url = "github:numtide/flake-utils";
    nixpkgs.url = "github:nixos/nixpkgs/nixpkgs-unstable";

  };

  outputs = { self, nixpkgs, flake-utils }:
    flake-utils.lib.eachDefaultSystem (system:
      let
        pkgs = nixpkgs.legacyPackages.${system};
        frameworks = pkgs.darwin.apple_sdk.frameworks;
      in
      {
        devShell = pkgs.mkShell {
          buildInputs = with pkgs; [
            opam
            frameworks.Security
            frameworks.CoreFoundation
            frameworks.CoreServices
            gmp
            pkgconf
            openssl
          ];
          shellHook = ''
            eval $(opam env)
          '';

        };
      });
}
