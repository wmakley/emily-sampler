package goplunger

type Game struct {
	Rooms []Room
	Player
	CurrentRoom          Room
	MenuItems            []MenuItem
	RoomChanged          bool
	RoomSceneryChanged   bool
	RoomInventoryChanged bool
	Flags                map[int]bool
}

type Player struct {
	Name string
	Inventory
}

type Room interface {
	Name() string
	Description(g *Game) string
	Links() []Room
	AddLink(link Room)
	Inventory() *Inventory
	Scenery() []Scenery
	AddScenery(scenery Scenery)
	HasActiveScenery(id int) bool
	ActivateScenery(id int)
	DeactivateScenery(id int)
}

type Scenery struct {
	ID          int
	Active      bool
	Description string
}

type MenuItem struct {
	Description string
	Action      func(g *Game)
}

type Item interface {
	Name() string
	// Usable determines if a "Use" menu item will be available
	Usable(g *Game) bool
	Use(g *Game)
	OnPickup(g *Game)
	OnDrop(g *Game)
}
