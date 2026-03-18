// #include <stdlib.h>
// #include "./../../includes/objects/event.h" 

// Event* construct_event(int time, EventType type, Process* process, Event* next_event) {
//     Event* event = (Event *) malloc(sizeof(Event));
//     if (event) {
//         memset(event, 0, sizeof(Event));
//         event->time = time;
//         event->type = type;
//         event->process = process;
//     }
//     return event;
// }

// void destruct_event(Event* event) {
//     if (event) {
//         free(event);
//     }
// }