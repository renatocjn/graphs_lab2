#ifndef ITERATOR_H
#define ITERATOR_H

class Iterator
{
    virtual bool hasNext() = 0;
    virtual int next() = 0;
};

#endif // ITERATOR_H
