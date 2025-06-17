//
// Created by Christian Caamano on 6/6/25.
//

#ifndef OMNISERVER_OMNI_NETWORK_H
#define OMNISERVER_OMNI_NETWORK_H

#include <QObject>

#include "omni_folder.h"

class OmniNetwork : public QObject {
    Q_OBJECT

public:
    explicit OmniNetwork(QObject *parent = nullptr);

    /**
     * @brief loadFromConfig - Load complete network info & structure from config file.
     * @pre Config file exists and has been validated by app core.
     */
    void loadFromConfig();

    QString getName() const;
    std::vector<OmniFolder*> getFolders() const;

private:
    QString uuid;
    QString name;
    std::vector<std::unique_ptr<OmniFolder>> folders;
};


#endif //OMNISERVER_OMNI_NETWORK_H
