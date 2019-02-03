


#ifndef _FROMMX_H_
#define _FROMMX_H_


namespace FromMX
{


void Transform()
{
	fstream gin("mm.dat", ios::in);

    unsigned long n;
	unsigned long i;

	unsigned long m;
	unsigned long aux;

	gin>>n;
	VECTOR3 *p=(VECTOR3 *)Heap.Allocate(n, sizeof(VECTOR3));
	VECTOR3 *N=(VECTOR3 *)Heap.Allocate(n, sizeof(VECTOR3));

	for(i=0;i<n;i++)
		gin>>p[i].x>>p[i].y>>p[i].z;

	for(i=0;i<n;i++)
		gin>>N[i].x>>N[i].y>>N[i].z;

	gin>>m;

	TRILGTEX_MESH Mesh;

	Mesh.New(m);

	for(i=0;i<m;i++)
	{
		gin>>aux;

		gin>>aux;
		Mesh.Tris[i].a=p[aux];
		Mesh.Tris[i].aN=N[aux];
		Mesh.Tris[i].at=PIXEL(0, 0);

		gin>>aux;
		Mesh.Tris[i].b=p[aux];
		Mesh.Tris[i].bN=N[aux];
		Mesh.Tris[i].bt=PIXEL(0, 0);

		gin>>aux;
		Mesh.Tris[i].c=p[aux];
		Mesh.Tris[i].cN=N[aux];
		Mesh.Tris[i].ct=PIXEL(0, 0);
	}

	Mesh.SaveMesh("MESH\\Ship.bx");

	Heap.Free(p);
	Heap.Free(N);
	gin.close();
}


} // namespace FromMX


#endif // #ifndef _FROMMX_H_