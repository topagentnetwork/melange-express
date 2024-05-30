set positional-arguments

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
  dune-release distrib
  dune-release publish distrib -m "Release melange-express" -y
  dune-release opam pkg
  dune-release opam submit -m "Release melange-express" --local-repo=/Users/emilio/Dev/tan/opam-packages --opam-repo=topagentnetwork/opam-packages  --remote-repo=https://github.com/topagentnetwork/opam-packages -y
