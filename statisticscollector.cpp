#include "statisticscollector.h"

#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <vector>
#include <string>
#include <map>
#include <iostream>
#include "segmentedimage.h"
#include "distribution.h"
#include "segment.h"

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
                    img->save("testeColor.png");
                }

                // Captura as propriedades e adiciona às devidas distribuiçoes
                props = img->getProperties();

                // Se for primeiro loop inicializa as distribuiçoes
                if (first) {
                    first = false;
                    for (j=0, n=props.size(); j<n; j++) {
                        if (props[j].first == 'i') {
                            this->distributions.push_back(std::pair<char, Distribution<int>*>(props[j].first, new Distribution<int>()));
                        } else if (props[j].first == 'd') {
                            this->distributions.push_back(std::pair<char, Distribution<double>*>(props[j].first, new Distribution<double>()));
                        }
                    }
                }

                for (j=0, n=props.size(); j<n; j++) {

                    if (props[j].first == 'i') {

                        (static_cast<Distribution<int>*>(this->distributions[j].second))->add(props[j].second.i);
                    } else if (props[j].first == 'd') {

                        (static_cast<Distribution<double>*>(this->distributions[j].second))->add(props[j].second.d);
                    }
                }

                // Adiciona em uma lista de imagens para depois calcular o score
                imagesList.push_back(img);
            }
        }

        std::cout << this->distributions.size() << std::endl;

        // Itera nas imagens já carregadas para calcular os scores
        /*if (i=0, n=imagesList.size(); i<n; i++) {

        }*/
    }
}
