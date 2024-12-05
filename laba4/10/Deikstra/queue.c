#include "queue.h"

void queInitialize(Queue *q) {
	q->head = NULL;
	q->tail = NULL;
}

bool isEmpty(Queue *q) { return q->head == NULL; }

int quePush(Queue *q, token value) {
	QNode *newNode = (QNode *)malloc(sizeof(QNode));
	if (newNode == NULL) {
		printf("Memory allocation failed!\n");
		return 1;
	}
	newNode->data = value;
	newNode->next = NULL;
	if (isEmpty(q)) {
		q->head = newNode;
		q->tail = newNode;
	} else {
		q->tail->next = newNode;
		q->tail = newNode;
	}
    return 0;
}

token quePop(Queue *q) {
	token value = q->head->data;
	QNode *temp = q->head;
	q->head = q->head->next;
	free(temp);
	return value;
}

token queTop(Queue *q) {
	return q->head->data;
}

void queDestroy(Queue *q) {
	while (!isEmpty(q)) {
		quePop(q);
	}
}