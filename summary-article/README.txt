Summary Article
===============

Compilation
-----------
In order to compile the summary article run the following command:
```bash
latexmk -pdf -auxdir=auxdir -nobibtexfudge summary-article.tex
```

Cleaning
--------
In order to clean the compiled and auxiliary files, run:
```bash
latexmk -auxdir=auxdir -nobibtexfudge -C
```