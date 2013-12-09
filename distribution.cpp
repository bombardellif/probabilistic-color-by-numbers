#include "distribution.h"

Distribution::Distribution():
    sorted(true),
    count(0)
{
    this->data.clear();
    this->frequency.clear();
}

void Distribution::add(double x)
{
    if (this->frequency.count(x)) {
        this->frequency[x]++;
    } else {
        this->frequency[x] = 1;
        this->data.push_back(x);
        this->sorted = false;
    }
    this->count++;
}

double Distribution::probability(double x)
{
    // Se nao estiver ordenado, entao ordene
    if (!this->sorted) {
        std::sort(this->data.begin(), this->data.end());
        this->sorted = true;
    }

    // busca o elemento no vetor
    std::vector<double>::iterator elementGtOrEq;
    elementGtOrEq = std::lower_bound(this->data.begin(), this->data.end(), x);

    // Se elemento encontrado for igual ao procurado ou for o ultimo ou o primeiro, retorna a probabilidade desse
    if ((*elementGtOrEq == x) || (*elementGtOrEq == this->data.back()) || (*elementGtOrEq == this->data.front())) {
        return ((double)this->frequency[x]) / this->count;
    }
    // Senao, faz uma interpolaçao entre este e o anterior, pois o procurado esta entre os dois
    else {
        return (this->frequency[*elementGtOrEq] + this->frequency[*(elementGtOrEq-1)]) / (2*this->count);
    }
}

