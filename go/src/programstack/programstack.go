package programstack

import "errors"

type Stack[T any] []T

func (s Stack[T]) IsEmpty() bool {
	return len(s) <= 0
}

func (s *Stack[T]) Push(elem T) {
	*s = append(*s, elem)
}

func (s Stack[T]) Peek() (T, error) {
	if s.IsEmpty() {
		return *new(T), errors.New("ERROR: Called Peek on empty stack")
	}

	return s[len(s)-1], nil
}

func (s *Stack[T]) Pop() (T, error) {
	if s.IsEmpty() {
		return *new(T), errors.New("ERROR: Called Pop on empty stack")
	}

	elem := (*s)[len(*s)-1]
	*s = (*s)[:len(*s)-1]

	return elem, nil
}
