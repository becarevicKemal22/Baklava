//
// Created by kemal on 06-Mar-26.
//

#include "EnvironmentPool.h"
#include "Environment.h"

struct EnvironmentPool::Slot {
    union {
        Slot *next;
        alignas(Environment) char object[sizeof(Environment)];
    };
};

EnvironmentPool::~EnvironmentPool() {
    for (Slot *slot: storage) {
        delete[] slot;
    }
}

Environment *EnvironmentPool::create(Environment *parent) {
    if (!head) {
        allocateChunk();
    }
    Slot *chunk = head;
    head = head->next;

    return new(chunk->object) Environment(parent);
}

void EnvironmentPool::destroy(Environment *p) {
    if (!p) return;

    p->~Environment();

    Slot *chunk = reinterpret_cast<Slot *>(p);
    chunk->next = head;
    head = chunk;
}

void EnvironmentPool::allocateChunk() {
    Slot *newChunk = new Slot[chunkCount];
    storage.push_back(newChunk);

    for (size_t i = 0; i < chunkCount - 1; i++) {
        newChunk[i].next = &newChunk[i + 1];
    }
    newChunk[chunkCount - 1].next = head;
    head = &newChunk[0];
}
