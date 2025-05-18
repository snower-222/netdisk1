#include "../include/func.h"

int enqueue(queue_t *p_queue, int net_fd){
    
    node_t *p_node = (node_t *)calloc(1, sizeof(node_t));
    p_node->net_fd = net_fd;

    if(p_queue->size == 0){
        p_queue->head = p_node;
        p_queue->end = p_node;
    }else{
        p_queue->end->p_next = p_node;
        p_queue->end = p_node;
    }

    p_queue->size++;

    return 0;
}

int dequeue(queue_t *p_queue){

    if(p_queue->size == 0){
        return -1;
    }

    node_t *p_node = p_queue->head;
    p_queue->head = p_node->p_next;

    if(p_queue->size == 1){
        p_queue->end = NULL;
    }

    p_queue->size--;
    free(p_node);

    return 0;

}

