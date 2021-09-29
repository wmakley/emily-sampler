package goplunger

type Inventory struct {
	Items []Item
}

func (i Inventory) Size() int {
	return len(i.Items)
}

func (i *Inventory) AddItem(item Item) {
	i.Items = append(i.Items, item)
}

func (i *Inventory) RemoveItemIndex(index int) Item {
	if index < 0 || index >= i.Size() {
		panic(ErrOutOfRange{
			Input: index,
			Min:   0,
			Max:   i.Size() - 1,
		})
	}

	var removed Item

	newSlice := make([]Item, 0, len(i.Items)-1)
	for j, item := range i.Items {
		if j != index {
			newSlice = append(newSlice, item)
		} else {
			removed = item
		}
	}
	i.Items = newSlice

	if removed == nil {
		panic("no item was removed")
	}

	return removed
}

func (i *Inventory) RemoveItem(itemToRemove Item) Item {
	if itemToRemove == nil {
		panic("itemToRemove must not be nil")
	}

	var removed Item

	newSlice := make([]Item, 0, len(i.Items)-1)
	for _, item := range i.Items {
		if item != itemToRemove {
			newSlice = append(newSlice, item)
		} else {
			removed = item
		}
	}
	i.Items = newSlice

	if removed == nil {
		panic("no item was removed")
	}

	return removed
}
