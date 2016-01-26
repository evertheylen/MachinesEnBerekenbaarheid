# Geïmplementeerde algoritmes:


## CNF

- Normale versie van CNF
- epsilon in body wordt geinput door een | zonder regel achter, zie input
- Output is zoals verwacht, met de variable -> terminal regel is de variable == [_TERMINAL]
- Met de splits regel zijn de variables gelijk aan [_C0], [_C1], ...

## LR(0) parser

LR parser dat gebruik maakt van een **LR(0) parse table**.

De CFG wordt ingegeven via de CFG.xml dat bijgevoegd is in de map Test LR parser.

De string dat geparsed moet worden, wordt gespecifieërd aan de hand van een string dat meegegeven wordt in de terminal.

De output gebeurt in de terminal:

1. De string wordt wel/niet geaccepteerd door de gegeven CFG.
2. De rightmost derivation regels dat gebruikt werden.


## CYK

Input: een XML zoals gespecifieërd op blackboard, en een string via de commandline.

Output: "String is in the language!" of "String is not in the language."
