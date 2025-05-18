package main

import (
	"bufio"
	"finamore00/brainfrick/src/interpreter"
	"fmt"
	"os"
)

func main() {
	if len(os.Args) != 2 {
		fmt.Fprintln(os.Stderr, "ERROR: Wrong usage.")
		fmt.Fprintln(os.Stderr, "Usage: brainfrick <src_file>")
		return
	}

	f, ferr := os.Open(os.Args[1])
	if ferr != nil {
		fmt.Fprintln(os.Stderr, "ERROR: Inputted file couldn't be read.")
		return
	}

	i := interpreter.New(f, bufio.NewReader(os.Stdin), 10000)

	err := i.Run()
	if err != nil {
		fmt.Fprintln(os.Stderr, err.Error())
	}
}
