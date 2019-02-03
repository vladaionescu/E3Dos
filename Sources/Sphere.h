

// Generates a 3DSphere


#ifndef _SPHERE_H_
#define _SPHERE_H_


namespace Sphere
{


void _SubDivide(unsigned long &v, VECTOR3 &v1, VECTOR3 &v2, VECTOR3 &v3, uint8 Preq, TRITEX_MESH &Mesh)
{
	VECTOR3 v12, v23, v31;

	if(!Preq)
	{
		Mesh.Tris[v].a=v3;
		Mesh.Tris[v].b=v2;
		Mesh.Tris[v].c=v1;

		float alpha;
		VECTOR2 Projection;

		Projection=VECTOR2(Mesh.Tris[v].a.ProjectZOX().x, Mesh.Tris[v].a.ProjectZOX().z);
		alpha=Projection.AngleOX()+PI;
		alpha/=2*PI;
		Mesh.Tris[v].at=VECTOR2(alpha*1439, (1+Mesh.Tris[v].a.y)*719.*0.5);

		Projection=VECTOR2(Mesh.Tris[v].b.ProjectZOX().x, Mesh.Tris[v].b.ProjectZOX().z);
		alpha=Projection.AngleOX()+PI;
		alpha/=2*PI;
		Mesh.Tris[v].bt=VECTOR2(alpha*1439, (1+Mesh.Tris[v].b.y)*719.*0.5);

		Projection=VECTOR2(Mesh.Tris[v].c.ProjectZOX().x, Mesh.Tris[v].c.ProjectZOX().z);
		alpha=Projection.AngleOX()+PI;
		alpha/=2*PI;
		Mesh.Tris[v].ct=VECTOR2(alpha*1439, (1+Mesh.Tris[v].c.y)*719.*0.5);

		VECTOR2 vs[3];
		vs[0]=Mesh.Tris[v].at;
		vs[1]=Mesh.Tris[v].bt;
		vs[2]=Mesh.Tris[v].ct;

		uint8 i;

		BOOL m=0, M=0;

		for(i=0;i<3;i++)
			if(vs[i].x<500)
                m=1;
			else if(vs[i].x>900)
				M=1;

		if(m&&M)
			for(i=0;i<3;i++)
				if(vs[i].x<900)
					vs[i].x=1400-vs[i].x;

		Mesh.Tris[v].at=vs[0];
		Mesh.Tris[v].bt=vs[1];
		Mesh.Tris[v].ct=vs[2];

		v++;

		return;
	}

	v12=v1+v2;
	v23=v2+v3;
	v31=v3+v1;

	v12.Setabs(1);
	v23.Setabs(1);
	v31.Setabs(1);

	_SubDivide(v, v1, v12, v31, Preq-1, Mesh);
	_SubDivide(v, v2, v23, v12, Preq-1, Mesh);
	_SubDivide(v, v3, v31, v23, Preq-1, Mesh);
	_SubDivide(v, v12, v23, v31, Preq-1, Mesh);
}


void Generate(uint8 Preq)
{
	TRITEX_MESH Mesh;

	Mesh.New((unsigned long)20*((unsigned long)1<<((unsigned long)Preq<<1)));

#define X 0.525731112119133606
#define Y 0.0
#define Z 0.850650808352039932

	VECTOR3 VData[12]=
	{
		VECTOR3(-X,  Y,  Z), VECTOR3( X,  Y,  Z), VECTOR3(-X,  Y, -Z), VECTOR3( X,  Y, -Z),
		VECTOR3( Y,  Z,  X), VECTOR3( Y,  Z, -X), VECTOR3( Y, -Z,  X), VECTOR3( Y, -Z, -X),
		VECTOR3( Z,  X,  Y), VECTOR3(-Z,  X,  Y), VECTOR3( Z, -X,  Y), VECTOR3(-Z, -X,  Y)
	};

#undef X
#undef Y
#undef Z

	uint8 TIndices[20][3]=
	{
		{ 0,  4,  1}, { 0,  9,  4}, { 9,  5,  4}, { 4,  5,  8}, { 4,  8,  1},
		{ 8, 10,  1}, { 8,  3, 10}, { 5,  3,  8}, { 5,  2,  3}, { 2,  7,  3},
		{ 7, 10,  3}, { 7,  6, 10}, { 7, 11,  6}, {11,  0,  6}, { 0,  1,  6},
		{ 6,  1, 10}, { 9,  0, 11}, { 9, 11,  2}, { 9,  2,  5}, { 7,  2, 11}
	};

	unsigned long v=0;

	for(uint8 i=0;i<20;i++)
		_SubDivide(v, VData[TIndices[i][0]], VData[TIndices[i][1]], VData[TIndices[i][2]], Preq, Mesh);

	Mesh.SaveMesh("MESH\\Sphere.bx");
}


} // namespace Sphere

#endif // #ifndef _SPHERE_H_