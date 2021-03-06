/******************************************************************************
TeleMídia Multiplexer MPEG-2 Transport Stream library
Copyright (C) 2016 TeleMídia Lab/PUC-Rio
https://github.com/TeleMidia/tmm-mpeg2ts/graphs/contributors

This program is free software: you can redistribute it and/or modify it under
the terms of the GNU Affero General Public License as published by the Free
Software Foundation, either version 3 of the License, or (at your option) any
later version. This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Affero General Public License
for more details. You should have received a copy of the GNU Affero General
Public License along with this program. If not, see http://www.gnu.org/licenses/.

*******************************************************************************/

/*
 * SystemManagement.cpp
 *
 *  Created on: 21/06/2013
 *      Author: Felippe Nagato
 */

#include "si/descriptor/SystemManagement.h"

namespace br {
namespace pucrio {
namespace telemidia {
namespace mpeg2 {

SystemManagement::SystemManagement() : MpegDescriptor(0xFE) {
	broadcastingFlag = SM_BROADCASTING;
	broadcastingIdentifier = ST_ISDB_SYSTEM;
	additionalBroadcastingId = 1; // What's it?
	additionalIdentificationInfoLength = 0;
	additionalIdentificationInfo = NULL;
}

SystemManagement::~SystemManagement() {
	if (additionalIdentificationInfo) delete additionalIdentificationInfo;
}

int SystemManagement::process() {
	char streamLen;
	int pos = MpegDescriptor::process();

	broadcastingFlag = ((stream[pos] >> 6) & 0xFF);
	broadcastingIdentifier = (stream[pos] & 0x3F);
	pos++;
	additionalBroadcastingId = stream[pos] & 0xFF;
	pos++;

	if (additionalIdentificationInfo) delete additionalIdentificationInfo;
	additionalIdentificationInfo = NULL;

	streamLen = descriptorLength - 2;
	if (streamLen > 0) {
		additionalIdentificationInfo = new char[streamLen];
		memcpy(additionalIdentificationInfo, stream + pos, streamLen);
		pos += streamLen;
	}

	return pos;
}

int SystemManagement::updateStream() {
	int pos = MpegDescriptor::updateStream();

	stream[pos] = (broadcastingFlag & 0xC0) << 6;
	stream[pos] = stream[pos] | (broadcastingIdentifier & 0x3F);
	pos++;
	stream[pos] = additionalBroadcastingId;
	pos++;

	if (additionalIdentificationInfo) {
		memcpy(stream + pos, additionalIdentificationInfo, additionalIdentificationInfoLength);
		pos += additionalIdentificationInfoLength;
	}

	return pos;
}

unsigned int SystemManagement::calculateDescriptorSize() {
	int pos = MpegDescriptor::calculateDescriptorSize();

	pos += 2 + additionalIdentificationInfoLength;

	return pos;
}

void SystemManagement::setBroadcastingFlag(unsigned char bf) {
	broadcastingFlag = bf;
}

unsigned char SystemManagement::getBroadcastingFlag() {
	return broadcastingFlag;
}

void SystemManagement::setBroadcastingIdentifier(unsigned char bi) {
	broadcastingIdentifier = bi;
}

unsigned char SystemManagement::getBroadcastingIdentifier() {
	return broadcastingIdentifier;
}

void SystemManagement::setAdditionalBroadcastingId(unsigned char abi) {
	additionalBroadcastingId = abi;
}

unsigned char SystemManagement::getAdditionalBroadcastingId() {
	return additionalBroadcastingId;
}

void SystemManagement::setAdditionalIdentificationInfo(char* stream, unsigned char len) {
	additionalIdentificationInfoLength = len;
	if (additionalIdentificationInfo) delete additionalIdentificationInfo;
	if (len == 0) {
		additionalIdentificationInfo = NULL;
		return;
	}
	additionalIdentificationInfo = new char[len];
	memcpy(stream, additionalIdentificationInfo, len);
}

unsigned char SystemManagement::getAdditionalIdentificationInfo(char** stream) {
	*stream = additionalIdentificationInfo;
	return additionalIdentificationInfoLength;
}

}
}
}
}


