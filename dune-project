(lang dune 3.8)

(using melange 0.1)

(generate_opam_files true)

(name melange-express)

(license "MIT")

(maintainers "Qwick Qwackers")

(authors "The Bilge Rats")

(homepage "No I Refuse")

(bug_reports "JFC just use chortcut")

(package
 (allow_empty)
 (name melange-express)
 (synopsis "Melange-express")
 (depends
  (ocaml
   (and
    (>= "4.14.0")
    (< "4.15.0")))
  (reason
   (and
    (>= "3.8.2")
    (< "4.0.0")))
  melange))
