package goplunger

import "fmt"

type ErrOutOfRange struct {
	Input int
	Min   int
	Max   int
}

func (e ErrOutOfRange) Error() string {
	return fmt.Sprintf("Number too large or small: '%d'", e.Input)
}

func (e ErrOutOfRange) Is(target error) bool {
	_, ok := target.(*ErrOutOfRange)
	return ok
}
