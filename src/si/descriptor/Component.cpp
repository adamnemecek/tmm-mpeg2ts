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
 * Component.cpp
 *
 *  Created on: 30/07/2013
 *      Author: Felippe Nagato
 */

#include "si/descriptor/Component.h"

namespace br {
namespace pucrio {
namespace telemidia {
namespace mpeg2 {

Component::Component() : MpegDescriptor(0x50) {
	streamContent = 0;
	componentType = 0;
	componentTag = 0;
	iso639LanguageCode.assign("por");
}

Component::~Component() {

}

int Component::process() {
	int strLen;
	int pos = MpegDescriptor::process();

	streamContent = stream[pos++] & 0x0F;
	componentType = stream[pos++] & 0xFF;
	componentTag = stream[pos++] & 0xFF;
	iso639LanguageCode.assign(stream + pos, 3);
	pos += 3;
	strLen = descriptorLength - 6;
	if (strLen < 0) strLen = 0;
	text.assign(stream + pos, strLen);
	pos += strLen;

	return pos;
}

int Component::updateStream() {
	int pos = MpegDescriptor::updateStream();

	stream[pos] = 0xF0;
	stream[pos] = stream[pos] | (streamContent & 0x0F);
	pos++;
	stream[pos++] = componentType;
	stream[pos++] = componentTag;
	memcpy(stream + pos, iso639LanguageCode.c_str(), iso639LanguageCode.length());
	pos += 3;
	memcpy(stream + pos, text.c_str(), text.length());
	pos += text.length();

	return pos;
}

unsigned int Component::calculateDescriptorSize() {
	int pos = MpegDescriptor::calculateDescriptorSize();

	pos += 6 + text.length();

	return pos;
}

void Component::setStreamContent(unsigned char sc) {
	streamContent = sc;
}

unsigned char Component::getStreamContent() {
	return streamContent;
}

void Component::setComponentType(unsigned char ct) {
	componentType = ct;
}

unsigned char Component::getComponentType() {
	return componentType;
}

void Component::setComponentTag(unsigned char ct) {
	componentTag = ct;
}

unsigned char Component::getComponentTag() {
	return componentTag;
}

void Component::setIso639LanguageCode(string code) {
	if (code.length() != 3) iso639LanguageCode.assign("por");
	iso639LanguageCode = code;
}

string Component::getIso639LanguageCode() {
	return iso639LanguageCode;
}

unsigned char Component::getTextLength() {
	return text.length();
}

void Component::setText(string text) {
	this->text = text;
}

string Component::getText() {
	return text;
}

}
}
}
}



