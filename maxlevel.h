#ifndef MAXLEVEL_H_
#define MAXLEVEL_H_


#include <memory>

class MaxLevel {
    private:
    public:
        int level;
        int playerId;

        MaxLevel() : level(-1), playerId(-1) {}
        ~MaxLevel() = default;    
};

#endif /* MAXLEVEL_H */