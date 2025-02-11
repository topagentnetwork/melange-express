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
  opam install . --deps-only -y --with-test --with-dev-setup

publish:
  opam publish -b master --repo=topagentnetwork/opam-packages

test *args='':
  dune build @tests
  pnpm jest $@ ./_build/default/__tests__/tests/__tests__
