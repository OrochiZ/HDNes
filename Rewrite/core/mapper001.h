#ifndef MAPPER001_H
#define MAPPER001_H

#include "cart.h"


class mapper001 : public cart
{
    public:
        mapper001();
        ~mapper001();

        void reset();
        void saveState(fstream* statefile);
        void loadState(fstream* statefile);

    protected:

    private:
};

#endif // MAPPER001_H
