/* Copyright (c) <2003-2016> <Newton Game Dynamics>
* 
* This software is provided 'as-is', without any express or implied
* warranty. In no event will the authors be held liable for any damages
* arising from the use of this software.
* 
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute it
* freely
*/

#ifndef __D_VEHICLE_INTERFACE_H__
#define __D_VEHICLE_INTERFACE_H__

#include "dStdafxVehicle.h"
#include "dVehicleNode.h"

class dVehicleChassis;


class dVehicleDifferentialInterface: public dVehicleNode
{
	public:
	DVEHICLE_API dVehicleDifferentialInterface(dVehicleNode* const parent);
	virtual ~dVehicleDifferentialInterface() {}
};


class dVehicleEngineInterface: public dVehicleNode
{
	public:
	class dEngineInfo
	{
		public:
		dEngineInfo ()
		{
			memset (this, 0, sizeof (dEngineInfo));
		}

		dFloat m_mass;
		dFloat m_armatureRadius;
	};

	DVEHICLE_API dVehicleEngineInterface(dVehicleNode* const parent, const dEngineInfo& info,  dVehicleTireInterface* const differential);
	virtual ~dVehicleEngineInterface() {}

	dEngineInfo m_info;
	dVehicleTireInterface* m_differential;
};


class dVehicleTireInterface: public dVehicleNode
{
	public:
	enum dSuspensionType
	{
		m_offroad,
		m_confort,
		m_race,
		m_roller,
	};

	class dTireInfo
	{
		public:
		dTireInfo()
		{
			memset(this, 0, sizeof(dTireInfo));

			dFloat gravity = 9.8f;
			dFloat vehicleMass = 1000.0f;

			m_mass = 25.0f;
			m_radio = 0.5f;
			m_width = 0.15f;
			m_pivotOffset = 0.01f;
			m_steerRate = 0.5f * dPi;
			m_frictionCoefficient = 0.8f;
			m_maxSteeringAngle = 25.0f * dDegreeToRad;

			m_suspensionLength = 0.3f;
			m_dampingRatio = 15.0f * vehicleMass;
			m_springStiffness = dAbs(vehicleMass * 9.8f * 8.0f / m_suspensionLength);

			m_corneringStiffness = dAbs(vehicleMass * gravity * 1.0f);
			m_longitudinalStiffness = dAbs(vehicleMass * gravity * 1.0f);
		}

		dVector m_location;
		dFloat m_mass;
		dFloat m_radio;
		dFloat m_width;
		dFloat m_steerRate;
		dFloat m_pivotOffset;
		dFloat m_dampingRatio;
		dFloat m_springStiffness;
		dFloat m_suspensionLength;
		dFloat m_maxSteeringAngle;
		dFloat m_corneringStiffness;
		dFloat m_longitudinalStiffness;
		dFloat m_frictionCoefficient;
		//dFloat m_aligningMomentTrail;
		//int m_hasFender;
		//dSuspensionType m_suspentionType;
	};

	DVEHICLE_API dVehicleTireInterface(dVehicleNode* const parent, const dTireInfo& info);
	virtual ~dVehicleTireInterface() {}

	virtual dVehicleTireInterface* GetAsTire() const { return (dVehicleTireInterface*) this; }

	const dTireInfo& GetInfo() const { return m_info; }
	void SetInfo(const dTireInfo& info) { m_info = info; }

	virtual dMatrix GetLocalMatrix() const = 0;
	virtual dMatrix GetGlobalMatrix() const = 0;
	virtual NewtonCollision* GetCollisionShape() const = 0;

	virtual dFloat GetSteeringAngle() const = 0;
	virtual void SetSteeringAngle(dFloat steeringAngle) = 0;

	virtual dFloat GetBrakeTorque() const = 0;
	virtual void SetBrakeTorque(dFloat brakeTorque) = 0;

	protected:
	dTireInfo m_info;
};

class dVehicleInterface: public dVehicleNode
{
	public:
	DVEHICLE_API dVehicleInterface(dVehicleChassis* const chassis);
	virtual ~dVehicleInterface() {};

	virtual dMatrix GetMatrix() const = 0;
	virtual dVehicleInterface* GetAsVehicle() const {return (dVehicleInterface*)this;} 

	protected:
	virtual dVehicleDifferentialInterface* AddDifferential(dVehicleTireInterface* const leftTire, dVehicleTireInterface* const rightTire) = 0;
	virtual dVehicleEngineInterface* AddEngine(const dVehicleEngineInterface::dEngineInfo& engineInfo, dVehicleDifferentialInterface* const differential) = 0;
	virtual dVehicleTireInterface* AddTire(const dMatrix& locationInGlobalSpace, const dVehicleTireInterface::dTireInfo& tireInfo, const dMatrix& localFrame) = 0;

	friend class dVehicleChassis;
};


#endif 

