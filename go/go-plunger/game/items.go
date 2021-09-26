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
		return !g.Flag(FlagRatRemoved)
	}
	return false
}

func (p *Plunger) Use(g *Game) {
	if g.CurrentRoom == Kitchen {
		if g.Flag(FlagRatRemoved) {
			panic("rat already removed")
		}

		g.SetFlag(FlagRatRemoved, true)

		g.Player.Inventory.RemoveItem(p)
		g.Player.Inventory.AddItem(NewPlungerWithRat())
		fmt.Println("You suction the rat in your plunger.\nYou are carrying a plunger with a rat in it.")
	} else if g.CurrentRoom == Bathroom {
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
	if g.CurrentRoom != BackDeck {
		panic("may only be used on BackDeck")
	}

	g.Player.Inventory.RemoveItem(p)
	sunBathingRat := NewSimpleItem("Sun-bathing rat")
	g.CurrentRoom.Inventory().AddItem(sunBathingRat)

	fmt.Printf(
		"You release the rat. It is now sun-bathing on the back deck.\nYou no longer have a %s.\n",
		p.name,
	)

	plunger := NewPlunger()
	g.Player.Inventory.AddItem(plunger)
	fmt.Printf("You now have a %s.\n", plunger.Name())

	g.RoomInventoryChanged = true
}
