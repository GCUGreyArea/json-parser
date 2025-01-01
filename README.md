# JSON Parser

This is a simple JSON parser built with `Flex` and `Bison`. It is not designed to be efficient, but simply to demonstrate the use of `Flex` and `Bison` in buildign a real world 

## Test harrnes

A simple application called `main` is availible to test the paraer

```bash
make main
./build/main -f test/resources/large-file.json 
```

## TODO

1. Add unit tests for JSON parsing