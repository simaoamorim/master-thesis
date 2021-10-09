LATEXMKFLAGS = -pdf -auxdir -nobibtexfudge
ROOTNAME = Real-time_Ethernet_Netorks_a_practical_approach_to_cycle_time_influence_in_control_applications

all: root

root: $(ROOTNAME).tex
	latexmk $(LATEXMKFLAGS) $^

.PHONY: clean all view all root

clean:
	latexmk $(LATEXMKFLAGS) -C

view: root
	@setsid firefox $(ROOTNAME).pdf &
