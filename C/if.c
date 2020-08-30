// gcc -Wall -o if if.c
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdarg.h>

struct room_t {
    const char *name;
    const char *description;
    int *links;
    int num_links;
};

const int LIVING_ROOM   = 0;
const int KITCHEN       = 1;
const int BATHROOM      = 2;
const int NUM_ROOMS     = 3;

int main() {
    struct room_t rooms[NUM_ROOMS];

    rooms[LIVING_ROOM] = (struct room_t) {
        "Living Room",
        "You see here a nice sofa and a hallway to the kitchen.",
        (int *)calloc(1, sizeof(int)),
        1
    };
    rooms[LIVING_ROOM].links[0] = KITCHEN;

    rooms[KITCHEN] = (struct room_t) {
        "Kitchen",
        "You see here a grungy stove, and the door to the bathroom.",
        (int *)calloc(2, sizeof(int)),
        2
    };
    rooms[KITCHEN].links[0] = LIVING_ROOM;
    rooms[KITCHEN].links[1] = BATHROOM;

    rooms[BATHROOM] = (struct room_t) {
        "Bathroom",
        "You see here a discolored toilet.",
        (int *)calloc(1, sizeof(int)),
        1
    };
    rooms[BATHROOM].links[0] = KITCHEN;

    int current_room = LIVING_ROOM;
    bool done = false;
    int choice = -1;
    while (!done) {
        struct room_t room = rooms[current_room];

        printf("\n=====================\n\nYou are in the %s\n", room.name);
        printf("\n%s\n\n", room.description);
        printf("  Where would you like to go? ('0' to quit)\n\n");
        for (int i = 0; i < room.num_links; i++) {
            int link = room.links[i];
            struct room_t linked_room = rooms[link];
            printf("  %d. %s\n", i + 1, linked_room.name);
        }
        printf("  > ");

        bool valid_choice = false;
        do {
            scanf("%d", &choice);
            if (choice <= 0) {
                done = true;
            } else if (choice <= room.num_links) {
                valid_choice = true;
                current_room = room.links[choice - 1];
            } else {
                printf("Not valid choice. Please try again: ");
                choice = -1;
            }
        } while (!done && !valid_choice);
    }

    for (int i=0; i < NUM_ROOMS; i++) {
        free(rooms[i].links);
    }
    return 0;
}
