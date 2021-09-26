package goplunger

import "fmt"

type GenericMenuItem struct {
	desc   string
	action func(g *Game)
}

func (m GenericMenuItem) Description() string {
	return m.desc
}

func (m GenericMenuItem) Action() func(g *Game) {
	return m.action
}

func NewWalkToRoomMenuItem(room Room) MenuItem {
	if room == nil {
		panic("room may not be nil")
	}

	return &GenericMenuItem{
		desc: fmt.Sprintf("Walk to %s", room.Name()),
		action: func(g *Game) {
			g.WalkToRoom(room)
		},
	}
}

func NewPickupItemMenuItem(item Item) MenuItem {
	return &GenericMenuItem{
		desc: fmt.Sprintf("Pick up %s", item.Name()),
		action: func(g *Game) {
			g.PickupItem(item)
		},
	}
}

func NewDropItemMenuItem(item Item) MenuItem {
	return &GenericMenuItem{
		desc: fmt.Sprintf("Drop %s", item.Name()),
		action: func(g *Game) {
			g.DropItem(item)
		},
	}
}

func NewUseItemMenuItem(item Item) MenuItem {
	return &GenericMenuItem{
		desc: fmt.Sprintf("Use %s", item.Name()),
		action: func(g *Game) {
			item.Use(g)
		},
	}
}
