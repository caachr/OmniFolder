//
// Created by Christian Caamano on 5/16/25.
//

#include "../include/drive.h"

OmniDrive::OmniDrive()
    : driveNickname()
    , driveID(0)
    , pathToOmniFolder()
    , driveType(true)
{}

OmniDrive::~OmniDrive()
{}

void OmniDrive::fromJSON(const json& jsonObj)
{
    // TODO
}

json OmniDrive::toJSON() const
{
    // TODO
}