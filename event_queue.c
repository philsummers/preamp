#define QUEUE_SIZE 10

unsigned char queue[QUEUE_SIZE];
signed char rear = -1;
signed char front = -1;

char events_in_queue(void) {
    if (front == -1) {
        return 0;
    } else {
        return 1;
    }
}

char enqueue_event(char evt) {

    if ((front == 0 && rear == QUEUE_SIZE-1) || (front == rear+1)) {
        return 0;
    } else if (rear == -1) {
        rear++;
        front++;
    } else if (rear == QUEUE_SIZE-1 && front > 0) {
        rear = 0;
    } else {
        rear++;
    }

    queue[rear] = evt;
    return 1;
}


char dequeue_event() {
    char event;

    if (!events_in_queue()) {
        return -1;
    } else if (front == rear) {
        event = queue[front];
        front = -1;
        rear = -1;

        return event;
    } else {
        event = queue[front];
        front++;

        return event;
    }
}



