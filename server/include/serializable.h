//
// Created by Christian Caamano on 5/23/25.
//

#ifndef SERIALIZABLE_H
#define SERIALIZABLE_H

#include "../include/defines.h"

/**
 * Defines an interface for objects that can be serialized to a specific format.
 * @tparam serialFormatType The typename of the serialization format being used (e.g., nlohmann::json if you're using JSON as the serialization format)
 */
template <typename serialFormatType> class Serializable {
public:
    /**
     * Serializes the object.
     * @return The serialized object.
     */
    virtual serialFormatType serialize() const = 0;

    /**
     * Deserializes an entity/data and populates this object's fields with the result.
     * @param serialFormatEntity The entity/data to deserialize.
     */
    virtual void deserialize(const serialFormatType& serialFormatEntity) = 0;
};

#endif //SERIALIZABLE_H
