//
// Created by Christian Caamano on 6/6/25.
//

#ifndef OMNICLIENT_OMNI_NETWORK_H
#define OMNICLIENT_OMNI_NETWORK_H

#include <QObject>

#include "omni_folder.h"

class OmniNetwork : public QObject {
    Q_OBJECT

public:
    explicit OmniNetwork(QObject *parent = nullptr);

    /**
     * @brief loadFromConfigJson - Load complete network info & structure from config json object.
     */
    void loadFromConfigJson(const nlohmann::json& configInfo);

    QString getName() const;
    std::vector<OmniFolder*> getFolders() const;

private:
    QString uuid;
    QString name;
    std::vector<std::unique_ptr<OmniFolder>> folders;
};


#endif //OMNICLIENT_OMNI_NETWORK_H
