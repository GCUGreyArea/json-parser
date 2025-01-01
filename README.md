# JSON Parser

This is a simple JSON parser built with `Flex` and `Bison`. It is not designed to be efficient, but simply to demonstrate the use of `Flex` and `Bison` in buildign a real world 

## Test harrnes

A simple application called `main` is availible to test the paraer

```bash
make main
./build/main -f test/resources/large-file.json 
```

The function can allso be run against an input string

```bash
./build/main '{"name":"Barry Robinson"}'
```

Both commands will generate a list of tokens

```bash
object @ 0, size 3
        Parent: object @ 0, size 3
string: "name" @ 1
        Parent: object @ 0, size 3
string: "Barry Robinson" @ 2
        Parent: object @ 0, size 3
```

## TODO

1. Add unit tests for JSON parsing