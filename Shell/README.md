# Shell

## Pipeline
- [x] Lexer
- [ ] Parser
- [ ] Interpreter

## BNF
```
<shell_cmd> ::= <list_of_cmd>
<list_of_cmd> ::= <pipe> {[one of &, &&, ||, ;] <pipe>} [&, ;]
<pipe> ::= <cmd> {| <cmd>}
<cmd> ::= <simple_cmd> | (<list_of_cmd>) [< <file_name>] [[one of  >, >>] <file_name>]
<simple_cmd> ::= <file_name> {<arg>} [< <file_name>] [[one of >, >>] <file_name>]
```