all: root

root: root.tex
	latexmk -pdf $^

test: test_chapter.tex
	latexmk -pdf $^

.PHONY: clean all view test-view all root test

clean:
	latexmk -c
	$(MAKE) -C chapters/ clean

view: root
	@setsid firefox root.pdf &

test-view: test
	@setsid firefox test_chapter.pdf &
