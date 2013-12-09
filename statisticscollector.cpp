#include "statisticscollector.h"

#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <vector>
#include <string>
#include <map>
#include <queue>
#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include "segmentedimage.h"
#include "distribution.h"
#include "segment.h"

int StatisticsCollector::K = 10;

int StatisticsCollector::getDir(std::string dir, std::vector<std::string> &files)
{
    DIR *dp;
    struct dirent *dirp;
    if((dp  = opendir(dir.c_str())) == NULL) {
        std::cout << "Error(" << errno << ") opening " << dir << std::endl;
        return errno;
    }

    while ((dirp = readdir(dp)) != NULL) {
        files.push_back(std::string(dirp->d_name));
    }
    closedir(dp);
    return 0;
}

StatisticsCollector::StatisticsCollector()
{
}

void StatisticsCollector::collectData(std::string directory)
{
    std::vector<std::string> files;
    std::vector<SegmentedImage*> imagesList;
    SegmentedImage *img;
    SegmentedImage *firstImage;
    std::vector<std::pair<char, Numeric> > props;
    int j, n;
    unsigned int i, m;

    if (!this->getDir(directory, files)) {

        this->distributions.clear();

        bool first=true;
        // Itera nas imagens do diretorio
        for (i=0, m=files.size(); i<m ;i++) {
            // Se for imagem png
            if (files[i].rfind(".png") != std::string::npos) {
                std::cout << files[i] << std::endl;

                // Carrega imagem
                img = new SegmentedImage(QString((directory + files[i]).c_str()));

                // Segmenta ela, caso seja a primeira
                if (first) {
                    img->segment();
                    firstImage = img;
                } else {
                    img->deepCopySegmentation(*firstImage);
                }

                // Captura as propriedades e adiciona às devidas distribuiçoes
                props = img->getProperties();

                // Se for primeiro loop inicializa as distribuiçoes
                if (first) {
                    first = false;
                    for (j=0, n=props.size(); j<n; j++) {
                        /*if (props[j].first == 'i') {
                            this->distributions.push_back(std::pair<char, Distribution<int>*>(props[j].first, new Distribution<int>()));
                        } else if (props[j].first == 'd') {
                            this->distributions.push_back(std::pair<char, Distribution<double>*>(props[j].first, new Distribution<double>()));
                        }*/
                        this->distributions.push_back(std::pair<char, Distribution*>(props[j].first, new Distribution()));
                    }
                }

                for (j=0, n=props.size(); j<n; j++) {

                    if (props[j].first == 'i') {

                        //(static_cast<Distribution<int>*>(this->distributions[j].second))->add(props[j].second.i);
                        this->distributions[j].second->add(props[j].second.i);
                    } else if (props[j].first == 'd') {

                        //(static_cast<Distribution<double>*>(this->distributions[j].second))->add(props[j].second.d);
                        this->distributions[j].second->add(props[j].second.d);
                    }
                }

                // Adiciona em uma lista de imagens para depois calcular o score
                imagesList.push_back(img);
            }
        }

        std::cout << this->distributions.size() << std::endl;

        // Itera nas imagens já carregadas para calcular os scores
        for (i=0, n=imagesList.size(); i<n; i++) {
            this->scoresDistribution.add(imagesList[i]->score(this->distributions));
        }
    }
}

std::vector<std::pair<double, SegmentedImage> > StatisticsCollector::sample(int numOfSamples, SegmentedImage &templateImg)
{
    std::vector<std::pair<double, SegmentedImage> > result;
    std::priority_queue<std::pair<double, SegmentedImage> > topProb;

    SegmentedImage img(templateImg.bits(), templateImg.width(), templateImg.height(), templateImg.format());
    img.deepCopySegmentation(templateImg);
    img.randomlyColor();
    img.getProperties();

    int i;
    double prob, ratio, lastProd;

    lastProd = this->scoresDistribution.probability(img.score(this->distributions));

    std::srand(std::time(NULL));

    // itera K vezes paraa realisar a amostragem da distribuiçao
    for (i=0; i<StatisticsCollector::K; i++) {
        // Copia o template para uma nova imagem e colore randomicamente
        img = SegmentedImage(templateImg.bits(), templateImg.width(), templateImg.height(), templateImg.format());
        img.deepCopySegmentation(templateImg);

        img.randomlyColor();
        img.getProperties();

        // calcula a divisao da probabilidade de img pela probabilidade do ultimo elemento inserido
        prob = this->scoresDistribution.probability(img.score(this->distributions));
        ratio = prob
            / lastProd;
        // Adiciona ao resultado a imagem caso tenha mais probabilidade que a anterior
        if ((std::rand() / (float)INT_MAX) <= std::min(1.0, ratio)) {

            topProb.push(std::pair<double, SegmentedImage>(prob, img));

            lastProd = prob;
        }

    }

    // Seleciona os top <numOfSamples> imagens do sampling acima
    for (i=0; (i<numOfSamples) && (!topProb.empty()); i++) {
        result.push_back(topProb.top());
        std::cout << topProb.top().first << std::endl;
        topProb.pop();
    }
    return result;
}
