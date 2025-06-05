//
// Created by Christian Caamano on 5/25/25.
//

#include "../include/fedex.h"

void FedEx::shipMessage(std::unique_ptr<Message> message)
{

}

void FedEx::processReceivedParcel(char byteBuffer[1024])
{
    messageformat_t bufferContents = messageformat_t::parse(byteBuffer);
}