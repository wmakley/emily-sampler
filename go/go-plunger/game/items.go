package goplunger

import "fmt"

type SimpleItem struct {
	name string
}

func (o SimpleItem) Name() string {
	return o.name
}

func (o SimpleItem) Usable(g *Game) bool {
	return false
}

func (o SimpleItem) Use(g *Game) {
	fmt.Println("That doesn't seem to do anything here.")
}

func (o SimpleItem) OnPickup(g *Game) {
	fmt.Printf("Picked up %s.\nYou now have a %s.\n", o.name, o.name)
}

func (o SimpleItem) OnDrop(g *Game) {
	fmt.Printf("Dropped %s.\nYou no longer have a %s.\n", o.name, o.name)
}

func NewSimpleItem(name string) Item {
	return &SimpleItem{
		name: name,
	}
}

type Plunger struct {
	SimpleItem
}

func NewPlunger() Item {
	return &Plunger{
		SimpleItem{
			name: "Plunger",
		},
	}
}

func (p Plunger) Usable(g *Game) bool {
	switch g.CurrentRoom {
	case Bathroom:
		return true
	case Kitchen:
		return g.CurrentRoom.HasActiveScenery(SceneryRatEatingCrumbsID)
	}
	return false
}

func (p *Plunger) Use(g *Game) {
	room := g.CurrentRoom
	if room == Kitchen {
		if !room.HasActiveScenery(SceneryRatEatingCrumbsID) {
			panic("rat already removed")
		}

		room.DeactivateScenery(SceneryRatEatingCrumbsID)

		g.Player.Inventory.RemoveItem(p)
		g.Player.Inventory.AddItem(NewPlungerWithRat())
		fmt.Println("You suction the rat in your plunger.\nYou are carrying a plunger with a rat in it.")
		g.RoomSceneryChanged = true
	} else if room == Bathroom {
		fmt.Println("You plunge the toilet, even though it doesn't need it.")
	}
}

type PlungerWithRat struct {
	SimpleItem
}

func NewPlungerWithRat() Item {
	return &PlungerWithRat{
		SimpleItem{
			name: "Plunger with rat",
		},
	}
}

func (p PlungerWithRat) Usable(g *Game) bool {
	return g.CurrentRoom == BackDeck
}

func (p *PlungerWithRat) Use(g *Game) {
	room := g.CurrentRoom
	if room != BackDeck {
		panic("may only be used on BackDeck")
	}

	g.Player.Inventory.RemoveItem(p)
	sunBathingRat := NewSimpleItem("sun-bathing rat")
	room.Inventory().AddItem(sunBathingRat)

	fmt.Printf(
		"You release the rat. It is now sun-bathing on the back deck.\nYou no longer have a %s.\n",
		p.name,
	)

	plunger := NewPlunger()
	g.Player.Inventory.AddItem(plunger)
	fmt.Printf("You now have a %s.\n", plunger.Name())

	room.Inventory().AddItem(NewOliver())
	room.Inventory().AddItem(NewSimpleItem("large turkey tom"))
	fmt.Printf("\nA large turkey tom has appeared!\nOliver has appeared!\n")

	g.RoomInventoryChanged = true
}

type Oliver struct {
	SimpleItem
}

func NewOliver() Item {
	return &Oliver{
		SimpleItem{
			name: "cat named Oliver",
		},
	}
}

func (o Oliver)OnPickup(g *Game) {
	fmt.Println("You pick up Oliver. He is purring.")
}

func (o Oliver)OnDrop(g *Game) {
	fmt.Println("You release Oliver.\nHe leaps from your arms in a graceful arc.")
}
