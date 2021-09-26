package goplunger

type Inventory struct {
	Objects []Object
}

func (i Inventory) Size() int {
	return len(i.Objects)
}

func (i *Inventory) AddItem(item Object) {
	i.Objects = append(i.Objects, item)
}

func (i *Inventory) RemoveItemIndex(index int) Object {
	if index < 0 || index >= i.Size() {
		panic(ErrOutOfRange{
			Input: index,
			Min:   0,
			Max:   i.Size() - 1,
		})
	}

	var removed Object

	newSlice := make([]Object, 0, len(i.Objects)-1)
	for j, item := range i.Objects {
		if j != index {
			newSlice = append(newSlice, item)
		} else {
			removed = item
		}
	}
	i.Objects = newSlice

	if removed == nil {
		panic("no item was removed")
	}

	return removed
}

func (i *Inventory) RemoveItem(itemToRemove Object) Object {
	if itemToRemove == nil {
		panic("itemToRemove must not be nil")
	}

	var removed Object

	newSlice := make([]Object, 0, len(i.Objects)-1)
	for _, item := range i.Objects {
		if item != itemToRemove {
			newSlice = append(newSlice, item)
		} else {
			removed = item
		}
	}
	i.Objects = newSlice

	if removed == nil {
		panic("no item was removed")
	}

	return removed
}
