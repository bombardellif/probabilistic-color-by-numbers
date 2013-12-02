#ifndef DISTRIBUTION_H
#define DISTRIBUTION_H

#include <vector>

class AbsDistribution { };

template <class T>
class Distribution: public AbsDistribution
{
private:
    std::vector<T> data;

public:
    Distribution();

    void add(T x);
    double probability(T x);
};

template <class T>
Distribution<T>::Distribution()
{
    this->data.clear();
}

template <class T>
void Distribution<T>::add(T x)
{
    this->data.push_back(x);
}

template <class T>
double Distribution<T>::probability(T x)
{

}

#endif // DISTRIBUTION_H
