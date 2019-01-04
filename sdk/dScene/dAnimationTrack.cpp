/////////////////////////////////////////////////////////////////////////////
// Name:        dAnimationTrack.h
// Purpose:     
// Author:      Julio Jerez
// Modified by: 
// Created:     22/05/2010 08:02:08
// RCS-ID:      
// Copyright:   Copyright (c) <2010> <Newton Game Dynamics>
// License:     
// This software is provided 'as-is', without any express or implied
// warranty. In no event will the authors be held liable for any damages
// arising from the use of this software.
// 
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely
/////////////////////////////////////////////////////////////////////////////


#include "dSceneStdafx.h"
#include "dScene.h"
#include "dAnimationTrack.h"
#include <tinyxml.h>


D_IMPLEMENT_CLASS_NODE(dAnimationTrack);

dAnimationTrack::dAnimationTrack(dScene* const world) 
	:dNodeInfo () 
{
	SetName ("animationLayers");
}

dAnimationTrack::dAnimationTrack()
	:dNodeInfo () 
{
	SetName ("animationLayers");
}

dAnimationTrack::~dAnimationTrack(void)
{
}

void dAnimationTrack::AddPosition(dFloat time, dFloat x, dFloat y, dFloat z)
{
	dCurveValue& value = m_position.Append()->GetInfo();
	value.m_x = x;
	value.m_y = y;
	value.m_z = z;
	value.m_time = time;
}

void dAnimationTrack::AddRotation(dFloat time, dFloat x, dFloat y, dFloat z)
{
	dCurveValue& value = m_rotation.Append()->GetInfo();
	value.m_x = x;
	value.m_y = y;
	value.m_z = z;
	value.m_time = time;
}

void dAnimationTrack::OptimizeCurves()
{
	if (m_position.GetCount() > 2) {
		OptimizeCurve(m_position);
	}
	if (m_rotation.GetCount() > 2) {
		OptimizeCurve(m_rotation);
	}
}

void dAnimationTrack::OptimizeCurve(dList<dCurveValue>& curve)
{

}

void dAnimationTrack::Serialize (TiXmlElement* const rootNode) const
{
	SerialiseBase(dNodeInfo, rootNode);

	if (m_position.GetCount()) {

		TiXmlElement* const positionKeys = new TiXmlElement("positionKeyframes");
		rootNode->LinkEndChild(positionKeys);

		int bufferSizeInBytes = 3 * 12 * m_position.GetCount() * sizeof(dFloat);
		char* const buffer = dAlloca(char, bufferSizeInBytes);
		dFloat* const time = dAlloca(dFloat, m_position.GetCount());
		dFloat* const points = dAlloca(dFloat, 3 * m_position.GetCount());
		
		int count = 0; 
		for (dList<dCurveValue>::dListNode* node = m_position.GetFirst(); node; node = node->GetNext()) {
			const dCurveValue& value = node->GetInfo();
			time[count] = value.m_time;
			points[count * 3 + 0] = value.m_x;
			points[count * 3 + 1] = value.m_y;
			points[count * 3 + 2] = value.m_z;
			count++;
		}

		TiXmlElement* const timeLine = new TiXmlElement("timeLine");
		positionKeys->LinkEndChild(timeLine);
		dFloatArrayToString(time, count, buffer, bufferSizeInBytes);
		timeLine->SetAttribute("float", count);
		timeLine->SetAttribute("floats", buffer);

		TiXmlElement* const positions = new TiXmlElement("positions");
		positionKeys->LinkEndChild(positions);
		dFloatArrayToString(points, 3 * count, buffer, bufferSizeInBytes);
		positions->SetAttribute("float3", count);
		positions->SetAttribute("floats", buffer);
	}

	if (m_rotation.GetCount()) {
		TiXmlElement* const rotationKeys = new TiXmlElement("rotationKeyframes");
		rootNode->LinkEndChild(rotationKeys);

		int bufferSizeInBytes = 3 * 12 * m_rotation.GetCount() * sizeof(dFloat);
		char* const buffer = dAlloca(char, bufferSizeInBytes);
		dFloat* const time = dAlloca(dFloat, m_rotation.GetCount());
		dFloat* const points = dAlloca(dFloat, 3 * m_rotation.GetCount());

		int count = 0;
		for (dList<dCurveValue>::dListNode* node = m_rotation.GetFirst(); node; node = node->GetNext()) {
			const dCurveValue& value = node->GetInfo();
			time[count] = value.m_time;
			points[count * 3 + 0] = value.m_x;
			points[count * 3 + 1] = value.m_y;
			points[count * 3 + 2] = value.m_z;
			count++;
		}

		TiXmlElement* const timeLine = new TiXmlElement("timeLine");
		rotationKeys->LinkEndChild(timeLine);
		dFloatArrayToString(time, count, buffer, bufferSizeInBytes);
		timeLine->SetAttribute("float", count);
		timeLine->SetAttribute("floats", buffer);

		TiXmlElement* const positions = new TiXmlElement("angles");
		rotationKeys->LinkEndChild(positions);
		dFloatArrayToString(points, 3 * count, buffer, bufferSizeInBytes);
		positions->SetAttribute("float3", count);
		positions->SetAttribute("floats", buffer);
	}
}

bool dAnimationTrack::Deserialize (const dScene* const scene, TiXmlElement* const rootNode) 
{
	DeserialiseBase(scene, dNodeInfo, rootNode);

	// load all the vertexData
	TiXmlElement* const positionKeyframes = (TiXmlElement*)rootNode->FirstChild("positionKeyframes");
	if (positionKeyframes) {

		TiXmlElement* const timeLineElement = (TiXmlElement*)positionKeyframes->FirstChild("timeLine");
		TiXmlElement* const positionElement = (TiXmlElement*)positionKeyframes->FirstChild("positions");

		int keyFramesCount;
		timeLineElement->Attribute("float", &keyFramesCount);

		dFloat* const timeline = dAlloca(dFloat, keyFramesCount);
		dFloat* const points = dAlloca(dFloat, 3 * keyFramesCount);

		dStringToFloatArray(timeLineElement->Attribute("floats"), timeline, keyFramesCount);
		dStringToFloatArray(positionElement->Attribute("floats"), points, 3 * keyFramesCount);

		for (int i = 0; i < keyFramesCount; i ++) {
			AddPosition(timeline[i], points[i * 3 + 0], points[i * 3 + 1], points[i * 3 + 2]);
		}
	}

	TiXmlElement* const rotationKeyframes = (TiXmlElement*)rootNode->FirstChild("rotationKeyframes");
	if (rotationKeyframes) {

		TiXmlElement* const timeLineElement = (TiXmlElement*)rotationKeyframes->FirstChild("timeLine");
		TiXmlElement* const positionElement = (TiXmlElement*)rotationKeyframes->FirstChild("angles");

		int keyFramesCount;
		timeLineElement->Attribute("float", &keyFramesCount);

		dFloat* const timeline = dAlloca(dFloat, keyFramesCount);
		dFloat* const points = dAlloca(dFloat, 3 * keyFramesCount);

		dStringToFloatArray(timeLineElement->Attribute("floats"), timeline, keyFramesCount);
		dStringToFloatArray(positionElement->Attribute("floats"), points, 3 * keyFramesCount);

		for (int i = 0; i < keyFramesCount; i++) {
			AddRotation(timeline[i], points[i * 3 + 0], points[i * 3 + 1], points[i * 3 + 2]);
		}
	}

	return true;
}