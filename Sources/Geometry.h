



#ifndef _GEOMETRY_H_
#define _GEOMETRY_H_


#include "General.h"


namespace Geometry
{


struct VECTOR2
{
	float x;
	float y;

	VECTOR2()
	{
	}
	
	VECTOR2(const float x, const float y)
	{
		(*this).x=x;
		(*this).y=y;
	}
	
	~VECTOR2()
	{
	}
	
	float AngleOX() const
	{
		if(y==0&&x==0)
			return 0;
	
		return Atan2(y, x);
	}
	
	float AngleOY() const
	{
		if(x==0&&y==0)
			return 0;
	
		return Atan2(x, y);
	}

	float CosAngleWith(const VECTOR2 &Axis) const
	{
		float absProd=Sqrt((*this).sqabs()*Axis.sqabs());

		if(absProd==0)
			return 1; // Cos(Angle)=1 <=> Angle=0
			
		return ((*this)%Axis)/absProd;
	}

	float AngleWith(const VECTOR2 &Axis) const
	{
		return Acos(CosAngleWith(Axis));
	}
	
	float sqabs() const
	{
		return (*this).x*(*this).x+(*this).y*(*this).y;
	}
	
	float abs() const
	{
		return Sqrt((*this).sqabs());
	}
	
	VECTOR2 &Scale(const float abs)
	{
		(*this).x*=abs;
		(*this).y*=abs;
		return (*this);
	}
	
	VECTOR2 &Setabs(const float abs)
	{
		float thisabs=(*this).abs();
	
		if(thisabs!=0)
			(*this).Scale(abs/thisabs);
		else
			(*this)=VECTOR2(abs, 0);
	
		return (*this);
	}
	
	VECTOR2 &Rotate(const float alpha)
	{
		float cosa=Cos(alpha);
		float sina=Sin(alpha);
		float x;
		float y;
	
		x=(*this).x*cosa-(*this).y*sina;
		y=(*this).x*sina+(*this).y*cosa;
	
		(*this).x=x;
		(*this).y=y;
	
		return (*this);
	}
	
	VECTOR2 operator + () const
	{
		return (*this);
	}
	
	VECTOR2 operator - () const
	{
		return VECTOR2(-(*this).x, -(*this).y);
	}
	
	VECTOR2 operator + (const VECTOR2 &a) const
	{
		return VECTOR2((*this).x+a.x, (*this).y+a.y);
	}
	
	VECTOR2 &operator += (const VECTOR2 &a)
	{
		return (*this)=(*this)+a;
	}
	
	VECTOR2 operator - (const VECTOR2 &a) const
	{
		return VECTOR2((*this).x-a.x, (*this).y-a.y);
	}
	
	VECTOR2 &operator -= (const VECTOR2 &a)
	{
		return (*this)=(*this)-a;
	}

	float operator % (const VECTOR2 &a) const // Dot Product
	{
		return (*this).x*a.x+(*this).y*a.y;
	}
	
	VECTOR2 operator * (const float a) const
	{
		return VECTOR2((*this).x*a, (*this).y*a);
	}
	
	VECTOR2 &operator *= (const float a)
	{
		return (*this)=(*this)*a;
	}
	
	VECTOR2 operator / (const float a) const
	{
		float sa=1/a;
		return VECTOR2((*this).x*sa, (*this).y*sa);
	}
	
	VECTOR2 operator /= (const float a)
	{
		return (*this)=(*this)/a;
	}
	
	int operator == (const VECTOR2 &a) const
	{
		return (a.x==(*this).x&&a.y==(*this).y);
	}
};

inline VECTOR2 operator * (const float a, const VECTOR2 &b)
{
	return VECTOR2(a*b.x, a*b.y);
}


struct VECTOR3
{
	float x;
	float y;
	float z;

	VECTOR3()
	{
	}
	
	VECTOR3(const float x, const float y, const float z)
	{
		(*this).x=x;
		(*this).y=y;
		(*this).z=z;
	}
	
	~VECTOR3()
	{
	}
	
	float AngleOX() const
	{
		float absval=abs();
	
		if(absval==0)
			return 0;
	
		return Acos(x/absval);
	}
	
	float AngleOY() const
	{
		float absval=abs();
	
		if(absval==0)
			return 0;
	
		return Acos(y/absval);
	}
		
	float AngleOZ() const
	{
		float absval=abs();
	
		if(absval==0)
			return 0;
	
		return Acos(z/absval);
	}

	float CosAngleWith(const VECTOR3 &Axis) const
	{
		float absProd=Sqrt((*this).sqabs()*Axis.sqabs());

		if(absProd==0)
			return 1; // Cos(Angle)=1 <=> Angle=0
			
		return ((*this)%Axis)/absProd;
	}

	float AngleWith(const VECTOR3 &Axis) const
	{
		return Acos(CosAngleWith(Axis));
	}
	
	VECTOR3 ProjectXOY() const
	{
		return VECTOR3(x, y, 0);
	}
	
	VECTOR3 ProjectYOZ() const
	{
		return VECTOR3(0, y, z);
	}
	
	VECTOR3 ProjectZOX() const
	{
		return VECTOR3(x, 0, z);
	}
	
	float sqabs() const
	{
		return (*this).x*(*this).x+(*this).y*(*this).y+(*this).z*(*this).z;
	}
	
	float abs() const
	{
		return Sqrt((*this).sqabs());
	}
	
	VECTOR3 &Scale(const float abs)
	{
		(*this).x*=abs;
		(*this).y*=abs;
		(*this).z*=abs;
		
		return (*this);
	}
	
	VECTOR3 &Setabs(const float abs)
	{
		float thisabs=(*this).abs();
	
		if(thisabs!=0)
			(*this).Scale(abs/thisabs);
		else
			(*this)=VECTOR3(abs, 0, 0);
	
		return (*this);
	}
	
	VECTOR3 &RotateX(const float alpha)
	{
		float cosa=Cos(alpha);
		float sina=Sin(alpha);
		float y;
		float z;
	
		y=(*this).y*cosa-(*this).z*sina;
		z=(*this).y*sina+(*this).z*cosa;
	
		(*this).y=y;
		(*this).z=z;
	
		return (*this);
	}
	
	VECTOR3 &RotateY(const float alpha)
	{
		float cosa=Cos(alpha);
		float sina=Sin(alpha);
		float z;
		float x;
	
		z=(*this).z*cosa-(*this).x*sina;
		x=(*this).z*sina+(*this).x*cosa;
	
		(*this).z=z;
		(*this).x=x;
	
		return (*this);
	}
	
	VECTOR3 &RotateZ(const float alpha)
	{
		float cosa=Cos(alpha);
		float sina=Sin(alpha);
		float x;
		float y;
	
		x=(*this).x*cosa-(*this).y*sina;
		y=(*this).x*sina+(*this).y*cosa;
	
		(*this).x=x;
		(*this).y=y;
	
		return (*this);
	}
	
	VECTOR3 &Rotate(const float alpha, VECTOR3 Axis)
	{
		if(Axis.x==0&&Axis.y==0&&Axis.z==0)
			return (*this);
	
		float AngleY;
		float AngleZ;
	
		if(Axis.x==0&&Axis.y==0)
			AngleZ=0;
		else
		{
			AngleZ=-Acos(Axis.x/Sqrt(Axis.x*Axis.x+Axis.y*Axis.y));
			if(Axis.y<0)
				AngleZ=-AngleZ;
		}
		Axis.RotateZ(AngleZ);
		
		if(Axis.x==0&&Axis.z==0)
			AngleY=0;
		else
		{
			AngleY=-Acos(Axis.x/Sqrt(Axis.x*Axis.x+Axis.z*Axis.z));
			if(Axis.z<0)
				AngleY=-AngleY;
		}
		// no need: Axis.RotateY(AngleY);
	
		RotateZ(AngleZ);
		RotateY(AngleY);
	
		RotateX(alpha);
		
		RotateY(-AngleY);
		RotateZ(-AngleZ);
	
		return (*this);
	}
	
	VECTOR3 operator + () const
	{
		return (*this);
	}
	
	VECTOR3 operator - () const
	{
		return VECTOR3(-(*this).x, -(*this).y, -(*this).z);
	}
	
	VECTOR3 operator + (const VECTOR3 &a) const
	{
		return VECTOR3((*this).x+a.x, (*this).y+a.y, (*this).z+a.z);
	}
	
	VECTOR3 &operator += (const VECTOR3 &a)
	{
		return (*this)=(*this)+a;
	}
	
	VECTOR3 operator - (const VECTOR3 &a) const
	{
		return VECTOR3((*this).x-a.x, (*this).y-a.y, (*this).z-a.z);
	}
	
	VECTOR3 &operator -= (const VECTOR3 &a)
	{
		return (*this)=(*this)-a;
	}
	
	VECTOR3 operator * (const VECTOR3 &a) const // Cross Product
	{
		return VECTOR3((*this).y*a.z-(*this).z*a.y, (*this).z*a.x-(*this).x*a.z, (*this).x*a.y-(*this).y*a.x);
	}
	
	VECTOR3 &operator *= (const VECTOR3 &a)
	{
		return (*this)=(*this)*a;
	}

	float operator % (const VECTOR3 &a) const // Dot Product
	{
		return (*this).x*a.x+(*this).y*a.y+(*this).z*a.z;
	}
	
	VECTOR3 operator * (const float &a) const
	{
		return VECTOR3((*this).x*a, (*this).y*a, (*this).z*a);
	}
	
	VECTOR3 &operator *= (const float &a)
	{
		return (*this)=(*this)*a;
	}
	
	VECTOR3 operator / (const float &a) const
	{
		float sa=1/a;
		return VECTOR3((*this).x*sa, (*this).y*sa, (*this).z*sa);
	}
		
	VECTOR3 &operator /= (const float &a)
	{
		return (*this)=(*this)/a;
	}
	
	int operator == (const VECTOR3 &a) const
	{
		return (a.x==(*this).x&&a.y==(*this).y&&a.z==(*this).z);
	}
};

inline VECTOR3 operator * (const float &a, const VECTOR3 &b)
{
	return VECTOR3(a*b.x, a*b.y, a*b.z);
}


inline VECTOR3 TriNormal(const VECTOR3 &A, const VECTOR3 &B, const VECTOR3 &C)
{
	return (A-B)*(A-C);
}

inline VECTOR3 TriMassCenter(const VECTOR3 &A, const VECTOR3 &B, const VECTOR3 &C)
{
	return (A+B+C)/3;
}


} // namespace Geometry


#endif // #ifndef _GEOMTERY_H_
