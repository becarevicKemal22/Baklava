//
// Created by kemal on 06-Mar-26.
//

#ifndef BAKLAVA_ENVIRONMENTPOOL_H
#define BAKLAVA_ENVIRONMENTPOOL_H

#include <vector>

class Environment;

class EnvironmentPool {
    struct Slot;

    Slot *head;
    unsigned int chunkCount;
    std::vector<Slot *> storage;

public:
    EnvironmentPool(unsigned int chunkCount) : chunkCount(chunkCount), head(0) {
    }

    ~EnvironmentPool();

    Environment *create(Environment *parent);

    void destroy(Environment *p);

    void allocateChunk();
};

#endif //BAKLAVA_ENVIRONMENTPOOL_H
