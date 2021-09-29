package goplunger

import "fmt"

func NewWalkToRoomMenuItem(room Room) MenuItem {
	if room == nil {
		panic("room may not be nil")
	}

	return MenuItem{
		Description: fmt.Sprintf("Walk to %s", room.Name()),
		Action: func(g *Game) {
			g.WalkToRoom(room)
		},
	}
}

func NewPickupItemMenuItem(item Item) MenuItem {
	return MenuItem{
		Description: fmt.Sprintf("Pick up %s", item.Name()),
		Action: func(g *Game) {
			g.PickupItem(item)
		},
	}
}

func NewDropItemMenuItem(item Item) MenuItem {
	return MenuItem{
		Description: fmt.Sprintf("Drop %s", item.Name()),
		Action: func(g *Game) {
			g.DropItem(item)
		},
	}
}

func NewUseItemMenuItem(item Item) MenuItem {
	return MenuItem{
		Description: fmt.Sprintf("Use %s", item.Name()),
		Action: func(g *Game) {
			item.Use(g)
		},
	}
}
