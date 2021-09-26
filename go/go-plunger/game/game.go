package goplunger

import (
	"bufio"
	"fmt"
	"os"
	"strconv"
	"strings"
)

var (
	Kitchen    = NewKitchen(2)
	Bathroom   = NewSimpleRoom("Bathroom", "It is reasonably clean.", 2)
	LivingRoom = NewSimpleRoom("Living Room", "You are standing near the coffee table.\nIt is cluttered.\nYou can see the kitchen, bathroom, and back deck.", 3)
	BackDeck   = NewSimpleRoom("Back Deck", "You are standing just outside of the sliding door.\nIt is sunny and beautiful.", 1)
)

func NewGame() *Game {
	rooms := []Room{
		Kitchen, Bathroom, LivingRoom, BackDeck,
	}
	Kitchen.AddLink(Bathroom)
	Kitchen.AddLink(LivingRoom)

	Bathroom.AddLink(Kitchen)
	Bathroom.AddLink(LivingRoom)

	LivingRoom.AddLink(Kitchen)
	LivingRoom.AddLink(Bathroom)
	LivingRoom.AddLink(BackDeck)

	BackDeck.AddLink(LivingRoom)

	plunger := NewPlunger()
	Bathroom.Inventory().AddItem(plunger)

	return &Game{
		Rooms:       rooms,
		CurrentRoom: nil,
		Player: Player{
			Name: "William",
		},
		Flags: make(map[int]bool, 1),
	}
}

func (g *Game) Start() {
	fmt.Println("Game start!")
	g.WalkToRoom(g.Rooms[0])

	reader := bufio.NewReader(os.Stdin)
	done := false
	for !done {
		if g.RoomChanged {
			g.DescribeRoom()
		}
		if g.RoomChanged || g.RoomInventoryChanged {
			g.ListRoomObjects()
		}
		g.RebuildMenu()
		g.PrintMenu()
		g.ResetTemporaryFlags()

		fmt.Print("\n>> ")

		input, err := reader.ReadString('\n')
		if err != nil {
			fmt.Println("An error occurred while reading input. Please try again", err)
			continue
		}

		input = strings.TrimSuffix(input, "\n")
		key := input[0]

		if key == 'q' || key == 'Q' {
			done = true
		} else if key >= '0' && key <= '9' {
			num, err := strconv.ParseInt(string(key), 10, 32)
			if err != nil {
				badInput(input)
				continue
			}

			err = g.ExecuteMenuItem(int(num))
			if err != nil {
				fmt.Println(err.Error())
			}
		} else {
			badInput(input)
		}
	}

	fmt.Println("Goodbye!")
}

func badInput(input string) {
	_, err := fmt.Printf("Unrecognized input: '%s'\n", input)
	if err != nil {
		panic(err)
	}
}

func (g *Game) RebuildMenu() {
	g.MenuItems = make([]MenuItem, 0, len(g.CurrentRoom.Links())+g.CurrentRoom.Inventory().Size()+(len(g.Player.Objects)*2))

	for _, room := range g.CurrentRoom.Links() {
		g.MenuItems = append(g.MenuItems, NewWalkToRoomMenuItem(room))
	}

	for _, roomItem := range g.CurrentRoom.Inventory().Objects {
		g.MenuItems = append(g.MenuItems, NewPickupItemMenuItem(roomItem))
	}

	for _, playerItem := range g.Player.Inventory.Objects {
		if playerItem.Usable(g) {
			g.MenuItems = append(g.MenuItems, NewUseItemMenuItem(playerItem))
		}
		g.MenuItems = append(g.MenuItems, NewDropItemMenuItem(playerItem))
	}
}

func (g Game) PrintMenu() {
	fmt.Print("\nOptions:\n\n")

	for i, item := range g.MenuItems {
		fmt.Printf("  (%d): %s\n", i+1, item.Description())
	}

	fmt.Println("  (q): Quit Game")
}

func (g *Game) ResetTemporaryFlags() {
	g.RoomChanged = false
	g.RoomInventoryChanged = false
}

func (g *Game) SetFlag(key int, val bool) {
	g.Flags[key] = val
}

func (g Game) Flag(key int) bool {
	val, exists := g.Flags[key]
	if !exists {
		return false
	}
	return val
}

func (g *Game) ExecuteMenuItem(index int) error {
	if index < 1 || index > len(g.MenuItems) {
		return ErrOutOfRange{
			Input: index,
			Min:   0,
			Max:   len(g.MenuItems),
		}
	}

	menuItem := g.MenuItems[index-1]
	menuItem.Action()(g)

	return nil
}

func (g *Game) WalkToRoom(room Room) {
	if room == nil {
		panic("room must not be nil")
	}
	g.CurrentRoom = room
	g.RoomChanged = true
}

func (g *Game) DescribeRoom() {
	fmt.Printf("You are in the %s.\n", g.CurrentRoom.Name())
	fmt.Println(g.CurrentRoom.Description(g))
}

func (g *Game) ListRoomObjects() {
	objects := g.CurrentRoom.Inventory().Objects
	if len(objects) > 0 {
		fmt.Print("\nObjects:\n\n")

		for _, obj := range objects {
			fmt.Printf("  * %s\n", obj.Name())
		}
	}
}

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

func (g *Game) PickupItem(itemToPickUp Object) {
	item := g.CurrentRoom.Inventory().RemoveItem(itemToPickUp)
	g.Player.Inventory.AddItem(item)
	fmt.Printf("Picked up %s.\nYou now have a %s.\n", item.Name(), item.Name())
	g.RoomInventoryChanged = true
}

func (g *Game) DropItem(itemToDrop Object) {
	item := g.Player.Inventory.RemoveItem(itemToDrop)
	g.CurrentRoom.Inventory().AddItem(item)
	fmt.Printf("Dropped %s.\nYou no longer have a %s.\n", item.Name(), item.Name())
	g.RoomInventoryChanged = true
}

func NewPickupItemMenuItem(item Object) MenuItem {
	return &GenericMenuItem{
		desc: fmt.Sprintf("Pick up %s", item.Name()),
		action: func(g *Game) {
			g.PickupItem(item)
		},
	}
}

func NewDropItemMenuItem(item Object) MenuItem {
	return &GenericMenuItem{
		desc: fmt.Sprintf("Drop %s", item.Name()),
		action: func(g *Game) {
			g.DropItem(item)
		},
	}
}

func NewUseItemMenuItem(item Object) MenuItem {
	return &GenericMenuItem{
		desc: fmt.Sprintf("Use %s", item.Name()),
		action: func(g *Game) {
			item.Use(g)
		},
	}
}
