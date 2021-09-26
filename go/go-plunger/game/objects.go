package goplunger

import "fmt"

type SimpleObject struct {
	name string
}

func (o SimpleObject) Name() string {
	return o.name
}

func (o SimpleObject) Usable(g *Game) bool {
	return false
}

func (o SimpleObject) Use(g *Game) {
	fmt.Println("That doesn't seem to do anything here.")
}

func NewSimpleObject(name string) Object {
	return &SimpleObject{
		name: name,
	}
}

type Plunger struct {
	SimpleObject
}

func NewPlunger() Object {
	return &Plunger{
		SimpleObject{
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
	SimpleObject
}

func NewPlungerWithRat() Object {
	return &PlungerWithRat{
		SimpleObject{
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
	sunBathingRat := NewSimpleObject("Sun-bathing rat")
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
