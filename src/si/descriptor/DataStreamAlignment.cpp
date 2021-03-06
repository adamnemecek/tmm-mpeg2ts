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
 * DataStreamAlignment.cpp
 *
 *  Created on: 08/01/2014
 *      Author: Felippe Nagato
 */

#include "si/descriptor/DataStreamAlignment.h"

namespace br {
namespace pucrio {
namespace telemidia {
namespace mpeg2 {

DataStreamAlignment::DataStreamAlignment() : MpegDescriptor(0x06) {
	alignmentType = 0x01;
}

DataStreamAlignment::~DataStreamAlignment() {

}

int DataStreamAlignment::process() {
	int pos = MpegDescriptor::process();

	alignmentType = stream[pos++] & 0xFF;

	return pos;
}

int DataStreamAlignment::updateStream() {
	int pos = MpegDescriptor::updateStream();

	stream[pos++] = alignmentType;

	return pos;
}

unsigned int DataStreamAlignment::calculateDescriptorSize() {
	int pos = MpegDescriptor::calculateDescriptorSize();

	pos += 1;

	return pos;
}

void DataStreamAlignment::setAlignmentType(unsigned char at) {
	alignmentType = at;
}

unsigned char DataStreamAlignment::getAlignmentType() {
	return alignmentType;
}

}
}
}
}


