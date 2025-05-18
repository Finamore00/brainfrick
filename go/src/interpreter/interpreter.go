package interpreter

import (
	"errors"
	"finamore00/brainfrick/src/programstack"
	"fmt"
	"io"
	"os"
	"slices"
)

type Interpreter struct {
	src    *os.File
	input  io.ByteReader
	memory []byte
	memCap uint64
	memPtr uint64
	pc     uint64
	st     programstack.Stack[uint64]
}

func New(src *os.File, input io.ByteReader, memCap uint64) *Interpreter {
	return &Interpreter{
		src:    src,
		input:  input,
		memory: make([]byte, memCap),
		memCap: memCap,
		pc:     0,
		memPtr: 0,
		st:     programstack.Stack[uint64]{},
	}
}

func (i *Interpreter) Run() error {
	buff := make([]byte, 1) //Stupid solution to be able to read a file byte by byte

	for _, err := i.src.Read(buff); err != io.EOF; _, err = i.src.Read(buff) {
		switch buff[0] {
		case '+':
			i.memory[i.memPtr] += 1
		case '-':
			i.memory[i.memPtr] -= 1
		case '<':
			if i.memPtr > 0 {
				i.memPtr -= 1
			}
		case '>':
			if i.memPtr < i.memCap-1 {
				i.memPtr += 1
			}
		case '.':
			fmt.Printf("%c", i.memory[i.memPtr])
		case ',':
			i.memory[i.memPtr], err = i.input.ReadByte()
			if err != nil {
				return err
			}
		case '[':
			if i.memory[i.memPtr] == 0 {
				tokens := []byte{'+', '-', '<', '>', '.', ',', '[', ']'}

				//Look for matching ']'
				depth := uint64(1)
				for _, err = i.src.Read(buff); depth > 0 && err != io.EOF; _, err = i.src.Read(buff) {
					if buff[0] == '[' {
						depth += 1
					}

					if buff[0] == ']' {
						depth -= 1
					}

					if slices.Contains(tokens, buff[0]) {
						i.pc += 1
					}
				}

				if err == io.EOF {
					return errors.New("ERROR: Found '[' token with no matching ']'")
				}
			} else {
				i.st.Push(i.pc)
			}
		case ']':
			if i.st.IsEmpty() {
				return errors.New("ERROR: Found ']' token with no matching '['")
			}

			if i.memory[i.memPtr] != 0 {
				tokens := []byte{'+', '-', '<', '>', '.', ',', '[', ']'}

				tgtPc, _ := i.st.Peek()
				for i.pc > tgtPc {
					i.src.Seek(-2, io.SeekCurrent)
					i.src.Read(buff)

					if slices.Contains(tokens, buff[0]) {
						i.pc -= 1
					}
				}
			} else {
				i.st.Pop()
			}
		default:
			continue //Everything else is dismissed as comment
		}

		i.pc += 1
	}
	return nil
}
