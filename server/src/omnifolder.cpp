//
// Created by Christian Caamano on 5/16/25.
//

#include "../include/omnifolder.h"

OmniFolder::OmniFolder()
    : omniFolderNickname()
    , omniFolderID(0)
    , drives(nullptr)
    , lockHolder("0")
{}

OmniFolder::~OmniFolder()
{
    delete drives;
}

void OmniFolder::fromJSON(const json& jsonObj)
{
    // TODO
}

json OmniFolder::toJSON() const
{
    // TODO
}