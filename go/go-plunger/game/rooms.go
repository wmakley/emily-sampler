package goplunger

import "fmt"

type SimpleRoom struct {
	name        string
	description string
	links       []Room
	inventory   Inventory
	scenery     []Scenery
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

func (r *SimpleRoom) Scenery() []Scenery {
	return r.scenery
}

func (r *SimpleRoom) AddScenery(scenery Scenery) {
	r.scenery = append(r.scenery, scenery)
}

func (r SimpleRoom) HasActiveScenery(id int) bool {
	for _, s := range r.scenery {
		if s.ID == id {
			return true
		}
	}
	return false
}

func (r *SimpleRoom) ActivateScenery(id int) {
	found := false
	for i, s := range r.scenery {
		if s.ID == id {
			r.scenery[i].Active = true
			found = true
			break
		}
	}
	if !found {
		panic(fmt.Errorf("scenery with id %d not found", id))
	}
}

func (r *SimpleRoom) DeactivateScenery(id int) {
	found := false
	for i, s := range r.scenery {
		if s.ID == id {
			r.scenery[i].Active = false
			found = true
			break
		}
	}
	if !found {
		panic(fmt.Errorf("scenery with id %d not found", id))
	}
}

func NewSimpleRoom(name string, description string, numLinks int) Room {
	return &SimpleRoom{
		name:        name,
		description: description,
		links:       make([]Room, 0, numLinks),
		scenery:     make([]Scenery, 0, 0),
	}
}
