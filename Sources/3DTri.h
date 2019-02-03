


#ifndef _3DTRI_H_
#define _3DTRI_H_


#include "2DGraph.h"
#include "3DGraph.h"


#define I_TYPE float


using namespace Graphics::Graphics2D;
using namespace FixedPoint;


namespace Graphics
{
namespace Graphics3D
{
namespace Triangle3D
{


namespace _Private
{


template <class VALUES, class CONSTS> struct _TRI3D // Most general kind of triangle (interpolate anything)
{
protected:

	struct VERTEX3D
	{
		VECTOR3 Transformed;
		VALUES Values;
	};

	struct VERTEX2D
	{
		VECTOR2 Position2D;
		VALUES Values;

		void CopyValuesFrom(const VERTEX3D &Vert)
		{
			Values=Vert.Values;
		}
	};

	struct PACKET3D
	{
		VERTEX3D a;
		VERTEX3D b;
		VERTEX3D c;
	};

	struct PACKET2D
	{
		VERTEX2D a;
		VERTEX2D b;
		VERTEX2D c;

		void CopyValuesFrom(const PACKET3D &Pack)
		{
			a.CopyValuesFrom(Pack.a);
			b.CopyValuesFrom(Pack.b);
			c.CopyValuesFrom(Pack.c);
		}

		CONSTS Con;
	};


public:

	VECTOR3 a;
	VECTOR3 b;
	VECTOR3 c;


	_TRI3D()
	{
	}

	virtual ~_TRI3D()
	{
	}

	void Project();


protected:

	virtual void StartValuesProject(PACKET3D &Original)=0;
	virtual void CalculateConsts(PACKET2D &Sorted, const PACKET3D &Obj)=0;
	virtual void StartValuesDraw(PACKET2D &Sorted)=0;
	virtual void ExtractValues(VALUES &Dest, const VALUES &Src)=0;
	virtual void HandlePixel(unsigned long Tol, const VALUES &Vals, const CONSTS &Con)=0;


private:

	void _Draw(PACKET2D &Sorted);
	void _Line(short x1, short x2, short y, VALUES v1, VALUES v2, const CONSTS &Con);
	void _Sort(PACKET2D &Sorted, PACKET2D &Obj);
};


} // namespace _Private

using namespace _Private;


namespace _Private
{


struct _EMPTY_CLASS
{
};


} // namespace _Private


namespace _Private
{


struct _TRIC3D_VALUES
{
	float z;
	fRGB Color;

	_TRIC3D_VALUES()
	{
	}

	_TRIC3D_VALUES(float z, fRGB Color)
	{
		(*this).z=z;
		(*this).Color=Color;
	}

	~_TRIC3D_VALUES()
	{
	}

	void Zero()
	{
		z=0;
		Color=fRGB(0, 0, 0);
	}

	_TRIC3D_VALUES operator + (const _TRIC3D_VALUES &v) const
	{
		return _TRIC3D_VALUES(z+v.z, Color+v.Color);
	}

	_TRIC3D_VALUES &operator += (const _TRIC3D_VALUES &v)
	{
		return (*this)=(*this)+v;
	}

	_TRIC3D_VALUES operator - (const _TRIC3D_VALUES &v) const
	{
		return _TRIC3D_VALUES(z-v.z, Color-v.Color);
	}

	_TRIC3D_VALUES &operator -= (const _TRIC3D_VALUES &v)
	{
		return (*this)=(*this)-v;
	}

	_TRIC3D_VALUES operator * (const float a) const
	{
		return _TRIC3D_VALUES(z*a, Color*a);
	}

	_TRIC3D_VALUES operator / (const float a) const
	{
		return (*this)*(1/a);
	}
};


} // namespace _Private


struct TRIC3D : _TRI3D <_TRIC3D_VALUES, _EMPTY_CLASS> // Tri-Colored Triangle
{
	RGB ac;
	RGB bc;
	RGB cc;

	TRIC3D()
	{
	}

	TRIC3D(VECTOR3 a, VECTOR3 b, VECTOR3 c, RGB ac, RGB bc, RGB cc)
	{
		(*this).a=a;
		(*this).b=b;
		(*this).c=c;
		(*this).ac=ac;
		(*this).bc=bc;
		(*this).cc=cc;
	}

	~TRIC3D()
	{
	}

	void StartValuesProject(PACKET3D &Original)
	{
		Original.a.Values.z=Original.a.Transformed.z;
		Original.b.Values.z=Original.b.Transformed.z;
		Original.c.Values.z=Original.c.Transformed.z;

		Original.a.Values.Color=fRGB(ac);
		Original.b.Values.Color=fRGB(bc);
		Original.c.Values.Color=fRGB(cc);
	}

	void CalculateConsts(PACKET2D &, const PACKET3D &)
	{
	}

	void StartValuesDraw(PACKET2D &Sorted)
	{
		Sorted.a.Values.z=1/Sorted.a.Values.z;
		Sorted.b.Values.z=1/Sorted.b.Values.z;
		Sorted.c.Values.z=1/Sorted.c.Values.z;

		Sorted.a.Values.Color*=Sorted.a.Values.z;
		Sorted.b.Values.Color*=Sorted.b.Values.z;
		Sorted.c.Values.Color*=Sorted.c.Values.z;
	}

	void ExtractValues(_TRIC3D_VALUES &Dest, const _TRIC3D_VALUES &Src)
	{
		Dest.z=1/Src.z;

		Dest.Color=Src.Color*Dest.z;
	}

	void HandlePixel(unsigned long Tol, const _TRIC3D_VALUES &Vals, const _EMPTY_CLASS &)
	{
		if(ZCheckNP(Vals.z))
			return;

		Graph3D.PutPixelZ(Tol, Vals.z, Vals.Color.ToRGBCheck());
	}
};


namespace _Private
{


struct _TRITEX3D_VALUES
{
	float z;
	VECTOR2 TexCoords;

	_TRITEX3D_VALUES()
	{
	}

	_TRITEX3D_VALUES(float z, VECTOR2 TexCoords)
	{
		(*this).z=z;
		(*this).TexCoords=TexCoords;
	}

	~_TRITEX3D_VALUES()
	{
	}

	void Zero()
	{
		z=0;
		TexCoords=VECTOR2(0, 0);
	}

	_TRITEX3D_VALUES operator + (const _TRITEX3D_VALUES &v) const
	{
		return _TRITEX3D_VALUES(z+v.z, TexCoords+v.TexCoords);
	}

	_TRITEX3D_VALUES &operator += (const _TRITEX3D_VALUES &v)
	{
		return (*this)=(*this)+v;
	}

	_TRITEX3D_VALUES operator - (const _TRITEX3D_VALUES &v) const
	{
		return _TRITEX3D_VALUES(z-v.z, TexCoords-v.TexCoords);
	}

	_TRITEX3D_VALUES &operator -= (const _TRITEX3D_VALUES &v)
	{
		return (*this)=(*this)-v;
	}

	_TRITEX3D_VALUES operator * (const float a) const
	{
		return _TRITEX3D_VALUES(z*a, TexCoords*a);
	}

	_TRITEX3D_VALUES operator / (const float a) const
	{
		return (*this)*(1/a);
	}
};


struct _TRIMTEX3D_CONSTS
{
	float RrN;
};


struct _TRIMTEX3D : _TRI3D <_TRITEX3D_VALUES, _TRIMTEX3D_CONSTS> // Used in TRITEX3D for projecting Mip-Mapped
{
	PIXEL at;
	PIXEL bt;
	PIXEL ct;

	SURFACE *Tex;

	_TRIMTEX3D()
	{
	}

	_TRIMTEX3D(VECTOR3 a, VECTOR3 b, VECTOR3 c, PIXEL at, PIXEL bt, PIXEL ct, SURFACE *Tex)
	{
		(*this).a=a;
		(*this).b=b;
		(*this).c=c;
		(*this).at=at;
		(*this).bt=bt;
		(*this).ct=ct;
		(*this).Tex=Tex;
	}

	~_TRIMTEX3D()
	{
	}

	void StartValuesProject(PACKET3D &Original)
	{
		Original.a.Values.z=Original.a.Transformed.z;
		Original.b.Values.z=Original.b.Transformed.z;
		Original.c.Values.z=Original.c.Transformed.z;

		Original.a.Values.TexCoords=(VECTOR2)at;
		Original.b.Values.TexCoords=(VECTOR2)bt;
		Original.c.Values.TexCoords=(VECTOR2)ct;
	}

	void CalculateConsts(PACKET2D &Sorted, const PACKET3D &Obj)
	{
		Sorted.Con.RrN=Graph3D.ViewPort.R32*Graph3D.ViewPort.sNearPlane*
			(((Obj.a.Values.TexCoords-Obj.b.Values.TexCoords).abs()+(Obj.b.Values.TexCoords-Obj.c.Values.TexCoords).abs()+(Obj.c.Values.TexCoords-Obj.a.Values.TexCoords).abs())/
				((Obj.a.Transformed-Obj.b.Transformed).abs()+(Obj.b.Transformed-Obj.c.Transformed).abs()+(Obj.c.Transformed-Obj.a.Transformed).abs()) );
	}

	void StartValuesDraw(PACKET2D &Sorted)
	{
		Sorted.a.Values.z=1/Sorted.a.Values.z;
		Sorted.b.Values.z=1/Sorted.b.Values.z;
		Sorted.c.Values.z=1/Sorted.c.Values.z;

		Sorted.a.Values.TexCoords*=Sorted.a.Values.z;
		Sorted.b.Values.TexCoords*=Sorted.b.Values.z;
		Sorted.c.Values.TexCoords*=Sorted.c.Values.z;
	}

	void ExtractValues(_TRITEX3D_VALUES &Dest, const _TRITEX3D_VALUES &Src)
	{
		Dest.z=1/Src.z;

		Dest.TexCoords=Src.TexCoords*Dest.z;
	}

	void HandlePixel(unsigned long Tol, const _TRITEX3D_VALUES &Vals, const _TRIMTEX3D_CONSTS &Con)
	{
		if(ZCheck(Tol, Vals.z))
			return;

		Graph3D.PutPixelZ(Tol, Vals.z, Tex->GetPixelTile((short)Vals.TexCoords.x, (short)Vals.TexCoords.y, (unsigned long)(Vals.z*Con.RrN)));
	}
};


struct _TRIATEX3D : _TRI3D <_TRITEX3D_VALUES, _EMPTY_CLASS> // Used in TRITEX3D for projecting Alpha-Mapped
{
	PIXEL at;
	PIXEL bt;
	PIXEL ct;

	SURFACE *Tex;

	_TRIATEX3D()
	{
	}

	_TRIATEX3D(VECTOR3 a, VECTOR3 b, VECTOR3 c, PIXEL at, PIXEL bt, PIXEL ct, SURFACE *Tex)
	{
		(*this).a=a;
		(*this).b=b;
		(*this).c=c;
		(*this).at=at;
		(*this).bt=bt;
		(*this).ct=ct;
		(*this).Tex=Tex;
	}

	~_TRIATEX3D()
	{
	}

	void StartValuesProject(PACKET3D &Original)
	{
		Original.a.Values.z=Original.a.Transformed.z;
		Original.b.Values.z=Original.b.Transformed.z;
		Original.c.Values.z=Original.c.Transformed.z;

		Original.a.Values.TexCoords=(VECTOR2)at;
		Original.b.Values.TexCoords=(VECTOR2)bt;
		Original.c.Values.TexCoords=(VECTOR2)ct;
	}

	void CalculateConsts(PACKET2D &, const PACKET3D &)
	{
	}

	void StartValuesDraw(PACKET2D &Sorted)
	{
		Sorted.a.Values.z=1/Sorted.a.Values.z;
		Sorted.b.Values.z=1/Sorted.b.Values.z;
		Sorted.c.Values.z=1/Sorted.c.Values.z;

		Sorted.a.Values.TexCoords*=Sorted.a.Values.z;
		Sorted.b.Values.TexCoords*=Sorted.b.Values.z;
		Sorted.c.Values.TexCoords*=Sorted.c.Values.z;
	}

	void ExtractValues(_TRITEX3D_VALUES &Dest, const _TRITEX3D_VALUES &Src)
	{
		Dest.z=1/Src.z;

		Dest.TexCoords=Src.TexCoords*Dest.z;
	}

	void HandlePixel(unsigned long Tol, const _TRITEX3D_VALUES &Vals, const _EMPTY_CLASS &)
	{
		if(ZCheck(Tol, Vals.z))
			return;

		Graph3D.PutPixelZ(Tol, Vals.z, Tex->GetAPixelTile((short)Vals.TexCoords.x, (short)Vals.TexCoords.y));
	}
};


} // namespace _Private


struct TRITEX3D : _TRI3D <_TRITEX3D_VALUES, _EMPTY_CLASS> // Textured triangle
{
	PIXEL at;
	PIXEL bt;
	PIXEL ct;

	SURFACE *Tex;

	TRITEX3D()
	{
	}

	TRITEX3D(VECTOR3 a, VECTOR3 b, VECTOR3 c, PIXEL at, PIXEL bt, PIXEL ct, SURFACE *Tex)
	{
		(*this).a=a;
		(*this).b=b;
		(*this).c=c;
		(*this).at=at;
		(*this).bt=bt;
		(*this).ct=ct;
		(*this).Tex=Tex;
	}

	~TRITEX3D()
	{
	}

	void ProjectMipMapped()
	{
		_TRIMTEX3D(a, b, c, at, bt, ct, Tex).Project();
	}

	void ProjectAlphaMapped()
	{
		_TRIATEX3D(a, b, c, at, bt, ct, Tex).Project();
	}

	void StartValuesProject(PACKET3D &Original)
	{
		Original.a.Values.z=Original.a.Transformed.z;
		Original.b.Values.z=Original.b.Transformed.z;
		Original.c.Values.z=Original.c.Transformed.z;

		Original.a.Values.TexCoords=(VECTOR2)at;
		Original.b.Values.TexCoords=(VECTOR2)bt;
		Original.c.Values.TexCoords=(VECTOR2)ct;
	}

	void CalculateConsts(PACKET2D &, const PACKET3D &)
	{
	}

	void StartValuesDraw(PACKET2D &Sorted)
	{
		Sorted.a.Values.z=1/Sorted.a.Values.z;
		Sorted.b.Values.z=1/Sorted.b.Values.z;
		Sorted.c.Values.z=1/Sorted.c.Values.z;

		Sorted.a.Values.TexCoords*=Sorted.a.Values.z;
		Sorted.b.Values.TexCoords*=Sorted.b.Values.z;
		Sorted.c.Values.TexCoords*=Sorted.c.Values.z;
	}

	void ExtractValues(_TRITEX3D_VALUES &Dest, const _TRITEX3D_VALUES &Src)
	{
		Dest.z=1/Src.z;

		Dest.TexCoords=Src.TexCoords*Dest.z;
	}

	void HandlePixel(unsigned long Tol, const _TRITEX3D_VALUES &Vals, const _EMPTY_CLASS &)
	{
		if(ZCheck(Tol, Vals.z))
			return;

		Graph3D.PutPixelZ(Tol, Vals.z, Tex->GetPixelTile((short)Vals.TexCoords.x, (short)Vals.TexCoords.y));
	}
};


namespace _Private
{


struct _TRILGTEX3D_VALUES
{
	float z;
	VECTOR2 TexCoords;
	fRGB LightEffect;

	_TRILGTEX3D_VALUES()
	{
	}

	_TRILGTEX3D_VALUES(float z, VECTOR2 TexCoords, fRGB LightEffect)
	{
		(*this).z=z;
		(*this).TexCoords=TexCoords;
		(*this).LightEffect=LightEffect;
	}

	~_TRILGTEX3D_VALUES()
	{
	}

	void Zero()
	{
		z=0;
		TexCoords=VECTOR2(0, 0);
		LightEffect=fRGB(0, 0, 0);
	}

	_TRILGTEX3D_VALUES operator + (const _TRILGTEX3D_VALUES &v) const
	{
		return _TRILGTEX3D_VALUES(z+v.z, TexCoords+v.TexCoords, LightEffect+v.LightEffect);
	}

	_TRILGTEX3D_VALUES &operator += (const _TRILGTEX3D_VALUES &v)
	{
		return (*this)=(*this)+v;
	}

	_TRILGTEX3D_VALUES operator - (const _TRILGTEX3D_VALUES &v) const
	{
		return _TRILGTEX3D_VALUES(z-v.z, TexCoords-v.TexCoords, LightEffect-v.LightEffect);
	}

	_TRILGTEX3D_VALUES &operator -= (const _TRILGTEX3D_VALUES &v)
	{
		return (*this)=(*this)-v;
	}

	_TRILGTEX3D_VALUES operator * (const float a) const
	{
		return _TRILGTEX3D_VALUES(z*a, TexCoords*a, LightEffect*a);
	}

	_TRILGTEX3D_VALUES operator / (const float a) const
	{
		return (*this)*(1/a);
	}
};


struct _TRILGMTEX3D_CONSTS
{
	float RrN;
};


struct _TRILGMTEX3D : _TRI3D <_TRILGTEX3D_VALUES, _TRILGMTEX3D_CONSTS> // Used in TRILGTEX3D for projecting Mip-Mapped
{
	PIXEL at;
	PIXEL bt;
	PIXEL ct;

	VECTOR3 aN;
	VECTOR3 bN;
	VECTOR3 cN;

	SURFACE *Tex;

	_TRILGMTEX3D()
	{
	}

	_TRILGMTEX3D(VECTOR3 a, VECTOR3 b, VECTOR3 c, PIXEL at, PIXEL bt, PIXEL ct, SURFACE *Tex, VECTOR3 aN, VECTOR3 bN, VECTOR3 cN)
	{
		(*this).a=a;
		(*this).b=b;
		(*this).c=c;
		(*this).at=at;
		(*this).bt=bt;
		(*this).ct=ct;
		(*this).Tex=Tex;
		(*this).aN=aN;
		(*this).bN=bN;
		(*this).cN=cN;
	}

	~_TRILGMTEX3D()
	{
	}

	void StartValuesProject(PACKET3D &Original)
	{
		Original.a.Values.z=Original.a.Transformed.z;
		Original.b.Values.z=Original.b.Transformed.z;
		Original.c.Values.z=Original.c.Transformed.z;

		Original.a.Values.TexCoords=(VECTOR2)at;
		Original.b.Values.TexCoords=(VECTOR2)bt;
		Original.c.Values.TexCoords=(VECTOR2)ct;

		Original.a.Values.LightEffect=Graph3D.LightManager.GetLightEffect(aN);
		Original.b.Values.LightEffect=Graph3D.LightManager.GetLightEffect(bN);
		Original.c.Values.LightEffect=Graph3D.LightManager.GetLightEffect(cN);
	}

	void CalculateConsts(PACKET2D &Sorted, const PACKET3D &Obj)
	{
		Sorted.Con.RrN=Graph3D.ViewPort.R32*Graph3D.ViewPort.sNearPlane*
			(((Obj.a.Values.TexCoords-Obj.b.Values.TexCoords).abs()+(Obj.b.Values.TexCoords-Obj.c.Values.TexCoords).abs()+(Obj.c.Values.TexCoords-Obj.a.Values.TexCoords).abs())/
				((Obj.a.Transformed-Obj.b.Transformed).abs()+(Obj.b.Transformed-Obj.c.Transformed).abs()+(Obj.c.Transformed-Obj.a.Transformed).abs()) );
	}

	void StartValuesDraw(PACKET2D &Sorted)
	{
		Sorted.a.Values.z=1/Sorted.a.Values.z;
		Sorted.b.Values.z=1/Sorted.b.Values.z;
		Sorted.c.Values.z=1/Sorted.c.Values.z;

		Sorted.a.Values.TexCoords*=Sorted.a.Values.z;
		Sorted.b.Values.TexCoords*=Sorted.b.Values.z;
		Sorted.c.Values.TexCoords*=Sorted.c.Values.z;

		Sorted.a.Values.LightEffect*=Sorted.a.Values.z;
		Sorted.b.Values.LightEffect*=Sorted.b.Values.z;
		Sorted.c.Values.LightEffect*=Sorted.c.Values.z;
	}

	void ExtractValues(_TRILGTEX3D_VALUES &Dest, const _TRILGTEX3D_VALUES &Src)
	{
		Dest.z=1/Src.z;

		Dest.TexCoords=Src.TexCoords*Dest.z;
		Dest.LightEffect=Src.LightEffect*Dest.z;
	}

	void HandlePixel(unsigned long Tol, const _TRILGTEX3D_VALUES &Vals, const _TRILGMTEX3D_CONSTS &Con)
	{
		if(ZCheck(Tol, Vals.z))
			return;

		Graph3D.PutPixelZ(Tol, Vals.z, Graph3D.LightManager.MixLightWithColor(Vals.LightEffect, (fRGB)Tex->GetPixelTileO(Vals.TexCoords, (unsigned long)(Vals.z*Con.RrN))));
	}
};


struct _TRILGATEX3D : _TRI3D <_TRILGTEX3D_VALUES, _EMPTY_CLASS> // Used in TRILGTEX3D for projecting AlphaMapped
{
	PIXEL at;
	PIXEL bt;
	PIXEL ct;

	VECTOR3 aN;
	VECTOR3 bN;
	VECTOR3 cN;

	SURFACE *Tex;

	_TRILGATEX3D()
	{
	}

	_TRILGATEX3D(VECTOR3 a, VECTOR3 b, VECTOR3 c, PIXEL at, PIXEL bt, PIXEL ct, SURFACE *Tex, VECTOR3 aN, VECTOR3 bN, VECTOR3 cN)
	{
		(*this).a=a;
		(*this).b=b;
		(*this).c=c;
		(*this).at=at;
		(*this).bt=bt;
		(*this).ct=ct;
		(*this).Tex=Tex;
		(*this).aN=aN;
		(*this).bN=bN;
		(*this).cN=cN;
	}

	~_TRILGATEX3D()
	{
	}

	void StartValuesProject(PACKET3D &Original)
	{
		Original.a.Values.z=Original.a.Transformed.z;
		Original.b.Values.z=Original.b.Transformed.z;
		Original.c.Values.z=Original.c.Transformed.z;

		Original.a.Values.TexCoords=(VECTOR2)at;
		Original.b.Values.TexCoords=(VECTOR2)bt;
		Original.c.Values.TexCoords=(VECTOR2)ct;

		Original.a.Values.LightEffect=Graph3D.LightManager.GetLightEffect(aN);
		Original.b.Values.LightEffect=Graph3D.LightManager.GetLightEffect(bN);
		Original.c.Values.LightEffect=Graph3D.LightManager.GetLightEffect(cN);
	}

	void CalculateConsts(PACKET2D &, const PACKET3D &)
	{
	}

	void StartValuesDraw(PACKET2D &Sorted)
	{
		Sorted.a.Values.z=1/Sorted.a.Values.z;
		Sorted.b.Values.z=1/Sorted.b.Values.z;
		Sorted.c.Values.z=1/Sorted.c.Values.z;

		Sorted.a.Values.TexCoords*=Sorted.a.Values.z;
		Sorted.b.Values.TexCoords*=Sorted.b.Values.z;
		Sorted.c.Values.TexCoords*=Sorted.c.Values.z;

		Sorted.a.Values.LightEffect*=Sorted.a.Values.z;
		Sorted.b.Values.LightEffect*=Sorted.b.Values.z;
		Sorted.c.Values.LightEffect*=Sorted.c.Values.z;
	}

	void ExtractValues(_TRILGTEX3D_VALUES &Dest, const _TRILGTEX3D_VALUES &Src)
	{
		Dest.z=1/Src.z;

		Dest.TexCoords=Src.TexCoords*Dest.z;
		Dest.LightEffect=Src.LightEffect*Dest.z;
	}

	void HandlePixel(unsigned long Tol, const _TRILGTEX3D_VALUES &Vals, const _EMPTY_CLASS &)
	{
		if(ZCheck(Tol, Vals.z))
			return;

		Graph3D.PutPixelZ(Tol, Vals.z, Graph3D.LightManager.MixLightWithColor(Vals.LightEffect, Tex->GetAPixelTile((short)Vals.TexCoords.x, (short)Vals.TexCoords.y)));
	}
};


} // namespace _Private


struct TRILGTEX3D : _TRI3D <_TRILGTEX3D_VALUES, _EMPTY_CLASS> // Gouraud-Shaded Textured Triangle
{
	PIXEL at;
	PIXEL bt;
	PIXEL ct;

	VECTOR3 aN;
	VECTOR3 bN;
	VECTOR3 cN;

	SURFACE *Tex;

	TRILGTEX3D()
	{
	}

	TRILGTEX3D(VECTOR3 a, VECTOR3 b, VECTOR3 c, PIXEL at, PIXEL bt, PIXEL ct, SURFACE *Tex, VECTOR3 aN, VECTOR3 bN, VECTOR3 cN)
	{
		(*this).a=a;
		(*this).b=b;
		(*this).c=c;
		(*this).at=at;
		(*this).bt=bt;
		(*this).ct=ct;
		(*this).Tex=Tex;
		(*this).aN=aN;
		(*this).bN=bN;
		(*this).cN=cN;
	}

	~TRILGTEX3D()
	{
	}

	void ProjectMipMapped()
	{
		_TRILGMTEX3D(a, b, c, at, bt, ct, Tex, aN, bN, cN).Project();
	}

	void ProjectAlphaMapped()
	{
		_TRILGATEX3D(a, b, c, at, bt, ct, Tex, aN, bN, cN).Project();
	}

	void StartValuesProject(PACKET3D &Original)
	{
		Original.a.Values.z=Original.a.Transformed.z;
		Original.b.Values.z=Original.b.Transformed.z;
		Original.c.Values.z=Original.c.Transformed.z;

		Original.a.Values.TexCoords=(VECTOR2)at;
		Original.b.Values.TexCoords=(VECTOR2)bt;
		Original.c.Values.TexCoords=(VECTOR2)ct;

		Original.a.Values.LightEffect=Graph3D.LightManager.GetLightEffect(aN);
		Original.b.Values.LightEffect=Graph3D.LightManager.GetLightEffect(bN);
		Original.c.Values.LightEffect=Graph3D.LightManager.GetLightEffect(cN);
	}

	void CalculateConsts(PACKET2D &, const PACKET3D &)
	{
	}

	void StartValuesDraw(PACKET2D &Sorted)
	{
		Sorted.a.Values.z=1/Sorted.a.Values.z;
		Sorted.b.Values.z=1/Sorted.b.Values.z;
		Sorted.c.Values.z=1/Sorted.c.Values.z;

		Sorted.a.Values.TexCoords*=Sorted.a.Values.z;
		Sorted.b.Values.TexCoords*=Sorted.b.Values.z;
		Sorted.c.Values.TexCoords*=Sorted.c.Values.z;

		Sorted.a.Values.LightEffect*=Sorted.a.Values.z;
		Sorted.b.Values.LightEffect*=Sorted.b.Values.z;
		Sorted.c.Values.LightEffect*=Sorted.c.Values.z;
	}

	void ExtractValues(_TRILGTEX3D_VALUES &Dest, const _TRILGTEX3D_VALUES &Src)
	{
		Dest.z=1/Src.z;

		Dest.TexCoords=Src.TexCoords*Dest.z;
		Dest.LightEffect=Src.LightEffect*Dest.z;
	}

	void HandlePixel(unsigned long Tol, const _TRILGTEX3D_VALUES &Vals, const _EMPTY_CLASS &)
	{
		if(ZCheck(Tol, Vals.z))
			return;

		Graph3D.PutPixelZ(Tol, Vals.z, Graph3D.LightManager.MixLightWithColor(Vals.LightEffect, (fRGB)Tex->GetPixelTile((short)Vals.TexCoords.x, (short)Vals.TexCoords.y)));
	}
};


namespace _Private
{


template <class TRI> struct _TRI_MESH // Most genenral kind of triangle mesh
{
	unsigned long NoTris;
	TRI *Tris;

	_TRI_MESH()
	{
		Tris=NULL;
		NoTris=0;
	}

	~_TRI_MESH()
	{
		Delete();
	}

	void New(unsigned long NoTris)
	{
		Delete();

		(*this).NoTris=NoTris;
		Tris=new TRI[NoTris]; // The only way it works. TODO: Find out why. (Might be a bug in the sizeof(template))

		Log.Message("Created Triangle Mesh.");
	}

	void Delete()
	{
		if(Tris!=NULL)
		{
			delete Tris;
			Tris=NULL;

			Log.Message("Deleted Triangle Mesh.");
		}

		NoTris=0;
	}

	void TriTransform(void (* TransformingFunction)(TRI &Tri));

	virtual void LoadMesh(char Filename[])=0;
	virtual void SaveMesh(char Filename[])=0;

	static void Tr_Project(TRI &Tri)
	{
		Tri.Project();
	}

	virtual void Translate(const VECTOR3 &Translation);
	virtual void RotateX(const float Alpha);
	virtual void RotateY(const float Alpha);
	virtual void RotateZ(const float Alpha);
	virtual void Scale(const float Coef);
};


} // namespace _Private


namespace CustomMeshes
{


struct TRIC_MESH : _TRI_MESH <TRIC3D>
{
	void LoadMesh(char Filename[]);
	void SaveMesh(char Filename[]);
};


struct TRITEX_MESH : _TRI_MESH <TRITEX3D>
{
	void LoadMesh(char Filename[]);
	void SaveMesh(char Filename[]);

	static void Tr_ProjectMipMapped(TRITEX3D &Tri)
	{
		Tri.ProjectMipMapped();
	}

	static void Tr_ProjectAlphaMapped(TRITEX3D &Tri)
	{
		Tri.ProjectAlphaMapped();
	}

	void ChangeTexture(SURFACE *Tex);
};


struct TRILGTEX_MESH : _TRI_MESH <TRILGTEX3D>
{
	void LoadMesh(char Filename[]);
	void SaveMesh(char Filename[]);

	static void Tr_ProjectMipMapped(TRILGTEX3D &Tri)
	{
		Tri.ProjectMipMapped();
	}

	static void Tr_ProjectAlphaMapped(TRILGTEX3D &Tri)
	{
		Tri.ProjectAlphaMapped();
	}

	void RotateX(const float Alpha);
	void RotateY(const float Alpha);
	void RotateZ(const float Alpha);

	void ChangeTexture(SURFACE *Tex);
};


} // namespace CustomMeshes

using namespace CustomMeshes;


} // namespace Triangle3D
} // namespace Graphics3D
} // namespace Graphics


#include "HField.h"


#endif // #ifndef _3DTRI_H_
