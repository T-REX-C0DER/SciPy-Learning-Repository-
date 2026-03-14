#include <stdio.h>
#include <stdlib.h>

// ─── Linked List Node ───────────────────────────────────────────────
typedef struct ListNode {
    int val;
    struct ListNode *next;
} ListNode;

// ─── Min-Heap (Priority Queue) ──────────────────────────────────────
typedef struct {
    ListNode **data;
    int size;
    int capacity;
} MinHeap;

// ─── Helper: Create a new node ──────────────────────────────────────
ListNode *createNode(int val) {
    ListNode *node = (ListNode *)malloc(sizeof(ListNode));
    node->val  = val;
    node->next = NULL;
    return node;
}

// ─── Min-Heap Operations ────────────────────────────────────────────
MinHeap *createHeap(int capacity) {
    MinHeap *heap    = (MinHeap *)malloc(sizeof(MinHeap));
    heap->data       = (ListNode **)malloc(capacity * sizeof(ListNode *));
    heap->size       = 0;
    heap->capacity   = capacity;
    return heap;
}

void swap(ListNode **a, ListNode **b) {
    ListNode *tmp = *a;
    *a = *b;
    *b = tmp;
}

void heapifyUp(MinHeap *heap, int idx) {
    while (idx > 0) {
        int parent = (idx - 1) / 2;
        if (heap->data[parent]->val > heap->data[idx]->val) {
            swap(&heap->data[parent], &heap->data[idx]);
            idx = parent;
        } else break;
    }
}

void heapifyDown(MinHeap *heap, int idx) {
    while (1) {
        int smallest = idx;
        int left     = 2 * idx + 1;
        int right    = 2 * idx + 2;

        if (left  < heap->size && heap->data[left]->val  < heap->data[smallest]->val)
            smallest = left;
        if (right < heap->size && heap->data[right]->val < heap->data[smallest]->val)
            smallest = right;

        if (smallest != idx) {
            swap(&heap->data[smallest], &heap->data[idx]);
            idx = smallest;
        } else break;
    }
}

void heapPush(MinHeap *heap, ListNode *node) {
    heap->data[heap->size++] = node;
    heapifyUp(heap, heap->size - 1);
}

ListNode *heapPop(MinHeap *heap) {
    ListNode *top = heap->data[0];
    heap->data[0] = heap->data[--heap->size];
    heapifyDown(heap, 0);
    return top;
}

// ─── Core Function ──────────────────────────────────────────────────
ListNode *mergeKLists(ListNode **lists, int k) {
    if (k == 0) return NULL;

    MinHeap  *heap   = createHeap(k);
    ListNode  dummy  = {0, NULL};
    ListNode *tail   = &dummy;

    // Push the head of each non-null list into the heap
    for (int i = 0; i < k; i++)
        if (lists[i]) heapPush(heap, lists[i]);

    // Repeatedly extract the minimum node
    while (heap->size > 0) {
        ListNode *node = heapPop(heap);
        tail->next = node;
        tail       = tail->next;

        if (node->next) heapPush(heap, node->next);
    }

    free(heap->data);
    free(heap);
    return dummy.next;
}

// ─── Utility: Build list from array ─────────────────────────────────
ListNode *buildList(int *arr, int n) {
    if (n == 0) return NULL;
    ListNode *head = createNode(arr[0]);
    ListNode *cur  = head;
    for (int i = 1; i < n; i++) {
        cur->next = createNode(arr[i]);
        cur = cur->next;
    }
    return head;
}

// ─── Utility: Print list ─────────────────────────────────────────────
void printList(ListNode *head) {
    while (head) {
        printf("%d", head->val);
        if (head->next) printf(" -> ");
        head = head->next;
    }
    printf("\n");
}

// ─── Main ────────────────────────────────────────────────────────────
int main() {
    // lists = [[1,4,5],[1,3,4],[2,6]]
    int a1[] = {1, 4, 5};
    int a2[] = {1, 3, 4};
    int a3[] = {2, 6};

    ListNode *lists[3];
    lists[0] = buildList(a1, 3);
    lists[1] = buildList(a2, 3);
    lists[2] = buildList(a3, 2);

    printf("Input lists:\n");
    for (int i = 0; i < 3; i++) {
        printf("  List %d: ", i + 1);
        printList(lists[i]);
    }

    ListNode *result = mergeKLists(lists, 3);

    printf("\nMerged output:\n  ");
    printList(result);

    return 0;
}
```

### Output
```
Input lists:
  List 1: 1 -> 4 -> 5
  List 2: 1 -> 3 -> 4
  List 3: 2 -> 6

Merged output:
  1 -> 1 -> 2 -> 3 -> 4 -> 4 -> 5 -> 6