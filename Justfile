set positional-arguments
set dotenv-load

local-repo := env("DUNE_RELEASE_LOCAL_REPO")
tan-repo := "topagentnetwork/opam-packages"

default:
  just --list

build: 
  dune build

dev: 
  dune build --watch

setup:
  opam repository add tan git+https://github.com/topagentnetwork/opam-packages

install: 
  opam install . --deps-only -y --with-test

release:
  dune-release distrib --skip-lint
  dune-release publish distrib -m "Release melange-express" -y
  dune-release opam pkg
  dune-release opam submit -m "Release melange-express" --local-repo={{local-repo}} --opam-repo={{tan-repo}}  --remote-repo=https://github.com/{{tan-repo}} -y
