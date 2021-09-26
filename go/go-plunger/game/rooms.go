package goplunger

import "fmt"

type SimpleRoom struct {
	name        string
	description string
	links       []Room
	inventory   Inventory
}

func (r SimpleRoom) Name() string {
	return r.name
}

func (r SimpleRoom) Description(g *Game) string {
	return r.description
}

func (r SimpleRoom) Links() []Room {
	return r.links
}

func (r *SimpleRoom) AddLink(link Room) {
	r.links = append(r.links, link)
}

func (r *SimpleRoom) Inventory() *Inventory {
	return &r.inventory
}

func NewSimpleRoom(name string, description string, numLinks int) Room {
	return &SimpleRoom{
		name:        name,
		description: description,
		links:       make([]Room, 0, numLinks),
	}
}

type KitchenRoom struct {
	SimpleRoom
}

func (k KitchenRoom) Description(g *Game) string {
	if g.Flags[FlagRatRemoved] {
		return k.description
	} else {
		return fmt.Sprintf("%s\nThere is a large rat eating the crumbs.", k.description)
	}
}

func NewKitchen(numLinks int) Room {
	return &KitchenRoom{
		SimpleRoom{
			name:        "Kitchen",
			description: "It is very grungy, and there are crumbs all over the stove.",
			links:       make([]Room, 0, numLinks),
		},
	}
}
