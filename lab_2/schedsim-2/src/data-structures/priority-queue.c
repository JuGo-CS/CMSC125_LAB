// #include <stdlib.h>
// #include <string.h>
// #include "./../../includes/data-structures/priority-queue.h"
// #include "./../../includes/objects/process.h"


// // Priority Queue Implementation
// PriorityQueueElement* construct_prio_element(void* data, int prio) {
//     PriorityQueueElement* element = malloc(sizeof(PriorityQueueElement));
//     if (element) {
//         element->data = data;
//         element->prio = prio;
//         element->next = NULL;
//     }
//     return element; 
// }
// void destruct_prio_element(PriorityQueueElement* element) {
//     free(element->data);
//     free(element);
// }

// // prio Implementation
// void prio_enqueue(AbstractQueue* self, void* data) {
//     PriorityQueue * pq = (PriorityQueue *) self; 

//     PriorityQueueElement* node = construct_priority_element(node->data);

//     if (pq->head == NULL) {
//         pq->head = node;    
//     } else {
//         PriorityQueueElement* curr = pq->head;
//         PriorityQueueElement* prev = NULL;

//         while (curr != NULL && prio >= curr->prio) {
//             prev = curr;
//             curr = curr->next;
//         }
        
//         node->next = curr;
//         prev->next = node;
//     }
//     self->size++;
// }

// void* prio_dequeue(AbstractQueue* self) {
//     PriorityQueue * pq = (PriorityQueue *)self;
//     if (pq->head == NULL) return NULL;

//     PriorityQueueElement* temp = pq->head;
//     void* data = temp->data;

//     pq->head = pq->head->next;

//     free(temp);
//     self->size--;
//     return data;
// }

// PriorityQueue * construct_prio_queue() {
//     PriorityQueue * pq = malloc(sizeof(PriorityQueue ));
//     pq->queue.enqueue = prio_enqueue;
//     pq->queue.dequeue = prio_dequeue;
//     pq->queue.size = 0;
//     pq->head = NULL;
//     return pq;
// }

// void destruct_prio_queue(PriorityQueue * pq) {
//     while (pq->head != NULL) {
//         free(prio_dequeue((AbstractQueue*)pq));
//     }
//     free(pq);
// }
