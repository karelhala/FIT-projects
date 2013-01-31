/******************************************************************************
 * Projekt - Zaklady pocitacove grafiky - IZG
 * spanel@fit.vutbr.cz
 *
 * $Id: student.c 217 2012-02-28 17:10:06Z spanel $
 */

#include "student.h"
#include "transform.h"

#include <memory.h>


/*****************************************************************************
 * Funkce vytvori vas renderer a nainicializuje jej */

S_Renderer * studrenCreate()
{
    S_StudentRenderer * renderer = (S_StudentRenderer *)malloc(sizeof(S_StudentRenderer));
    IZG_CHECK(renderer, "Cannot allocate enough memory");

    /* inicializace default rendereru */
    renInit(&renderer->base);

    /* nastaveni ukazatelu na vase upravene funkce */
    renderer->base.projectTriangleFunc = studrenProjectTriangle;

    renderer->base.calcReflectanceFunc = studrenPhongReflectance;

    return (S_Renderer *)renderer;
}

/****************************************************************************
 * Funkce vyhodnoti Phonguv osvetlovaci model pro zadany bod
 * a vraci barvu, kterou se bude kreslit do frame bufferu
 * point - souradnice bodu v prostoru
 * normal - normala k povrchu v danem bode */

S_RGBA studrenPhongReflectance(S_Renderer *pRenderer, const S_Coords *point, const S_Coords *normal)
{
   S_Coords    lvec;
    double      diffuse, specular, r=0, g=0, b=0;
    S_RGBA      color;
	

    /* vektor ke zdroji svetla */
    lvec = makeCoords(pRenderer->light_position.x - point->x,
                      pRenderer->light_position.y - point->y,
                      pRenderer->light_position.z - point->z);
    coordsNormalize(&lvec);

    /* difuzni cast */
    diffuse = lvec.x * normal->x + lvec.y * normal->y + lvec.z * normal->z;
    if( diffuse > 0 )
    {
        r += diffuse * pRenderer->light_diffuse.red * pRenderer->mat_diffuse.red;
        g += diffuse * pRenderer->light_diffuse.green * pRenderer->mat_diffuse.green;
        b += diffuse * pRenderer->light_diffuse.blue * pRenderer->mat_diffuse.blue;
    }
    
    //vektor zdroj svetla
	S_Coords viewVect = makeCoords (-point->x,-point->y,pRenderer->camera_dist - point->z);
	//odraz
	S_Coords erVect = makeCoords (2*diffuse*normal->x -lvec.x,2*diffuse*normal->y -lvec.y,2*diffuse*normal->z -lvec.z);
	
	coordsNormalize(&viewVect);
	coordsNormalize(&erVect);
	
	//spekularni slozka
	specular = erVect.x * viewVect .x - erVect.y *viewVect .y - erVect.z*viewVect .z;
	if( specular > 0 )
    {
        r += specular * pRenderer->light_specular.red * pRenderer->mat_specular.red;
        g += specular * pRenderer->light_specular.green * pRenderer->mat_specular.green;
        b += specular * pRenderer->light_specular.blue * pRenderer->mat_specular.blue;
    }
    
    //ambientni slozka
    r+=pRenderer->light_ambient.red * pRenderer->mat_ambient.red;
    g+=pRenderer->light_ambient.green * pRenderer->mat_ambient.green;
    b+=pRenderer->light_ambient.blue * pRenderer->mat_ambient.blue;
    
	
    /* saturace osvetleni*/
    r = MIN(1, r);
    g = MIN(1, g);
    b = MIN(1, b);

    /* kreslici barva */
    color.red = ROUND2BYTE(255 * r);
    color.green = ROUND2BYTE(255 * g);
    color.blue = ROUND2BYTE(255 * b);
    return color;
}

/****************************************************************************
 * Nova fce pro rasterizace trojuhelniku do frame bufferu
 * vcetne interpolace z souradnice a prace se z-bufferem
 * a interpolaci normaly pro Phonguv osvetlovaci model
 * v1, v2, v3 - ukazatele na vrcholy trojuhelniku ve 3D pred projekci
 * n1, n2, n3 - ukazatele na normaly ve vrcholech ve 3D pred projekci
 * x1, y1, ... - vrcholy trojuhelniku po projekci do roviny obrazovky */

void studrenDrawTriangle(S_Renderer *pRenderer,
                         S_Coords *v1, S_Coords *v2,S_Coords *v3,
                         S_Coords *n1, S_Coords *n2,S_Coords *n3,
                         int x1, int y1,
                         int x2, int y2,
                         int x3, int y3
                         )
{
   S_RGBA  color;      /* kreslici barva */
   /* Lambertuv osvetlovaci model vyhodnotime v prvnim vrcholu trojuhelniku */
   // color = pRenderer->calcReflectanceFunc(pRenderer, v1, nn);
  
    // obalka trojuhleniku
  int minx = MIN(x1, MIN(x2, x3));
  int maxx = MAX(x1, MAX(x2, x3));
  int miny = MIN(y1, MIN(y2, y3));
  int maxy = MAX(y1, MAX(y2, y3));
  
  
  // oriznuti podle rozmeru okna
  miny = MAX(miny, 0);
  maxy = MIN(maxy, pRenderer->frame_h - 1);
  minx = MAX(minx, 0);
  maxx = MIN(maxx, pRenderer->frame_w - 1);
	
	// normala primky (A, B) - vektor kolmy k vektoru mezi dvema vrcholy, tedy (-dy, dx)
  int a1 = y1 - y2;
  int a2 = y2 - y3;
  int a3 = y3 - y1;
  int b1 = x2 - x1;
  int b2 = x3 - x2;
  int b3 = x1 - x3;

  // koeficient C
  int c1 = x1 * y2 - x2 * y1;
  int c2 = x2 * y3 - x3 * y2;
  int c3 = x3 * y1 - x1 * y3;

  // vypocet hranove fce pro protejsi body
  int s1 = a1 * x3 + b1 * y3 + c1;
  int s2 = a2 * x1 + b2 * y1 + c2;
  int s3 = a3 * x2 + b3 * y2 + c3;

  // normalizace, aby vzdalenost od primky byla kladna uvnitr trojuhelniku
  if( s1 < 0 )
  {
    a1 *= -1; b1 *= -1; c1 *= -1;
  }
  if( s2 < 0 )
  {
    a2 *= -1; b2 *= -1; c2 *= -1;
  }
  if( s3 < 0 )
  {
    a3 *= -1; b3 *= -1; c3 *= -1;
  }

  // vyplnovani...
  int y=0;
  for (y = miny; y <= maxy; ++y)
  {
    // inicilizace hranove fce v bode [minx, y]

    int x=0;
    for (x = minx; x <= maxx; ++x)
    {
		int e1 = a1 * x + b1 * y + c1;
		int e2 = a2 * x + b2 * y + c2;
		int e3 = a3 * x + b3 * y + c3;
		if( e1 >= 0 && e2 >= 0 && e3 >= 0 )
		{
			double l1,l2,l3;
			l1=((y2-y3)*(x-x3)+(x3-x2)*(y-y3))/(double)((y2-y3)*(x1-x3)+(x3-x2)*(y1-y3)); 
			l2=((y3-y1)*(x-x3)+(x1-x3)*(y-y3))/(double)((y2-y3)*(x1-x3)+(x3-x2)*(y1-y3)); 
			l3=1-l1-l2;
			
			double z = l1*v1->z + l2*v2->z + l3*v3->z;
			if (z < DEPTH(pRenderer, x, y)){
				S_Coords NewN;
				NewN.x = n1->x*l1+n2->x*l2+n3->x*l3;
				NewN.y = n1->y*l1+n2->y*l2+n3->y*l3;
				NewN.z = n1->z*l1+n2->z*l2+n3->z*l3;
				coordsNormalize(&NewN);
				S_Coords pixel = makeCoords(v1->x*l1 + v2->x*l2 + v3->x*l3,	v1->y*l1 + v2->y*l2 + v3->y*l3,	v1->z*l1 + v2->z*l2 + v3->z*l3);
				color = pRenderer->calcReflectanceFunc(pRenderer, &pixel, &NewN);
				PIXEL(pRenderer,x, y) = color;
				DEPTH(pRenderer, x, y) = z;
			}	
		}

		// hranova fce o pixel vedle
		e1 += a1;
		e2 += a2;
		e3 += a3;
    }
  }
}

/*****************************************************************************
 * Vykresli i-ty trojuhelnik modelu
 * Pred vykreslenim aplikuje na vrcholy a normaly trojuhelniku
 * aktualne nastavene transformacni matice!
 * i - index trojuhelniku */

void studrenProjectTriangle(S_Renderer *pRenderer, S_Model *pModel, int i)
{
    S_Coords    aa, bb, cc;             /* souradnice vrcholu po transformaci ve 3D pred projekci */
    S_Coords    nn;                     /* normala trojuhelniku po transformaci */
    S_Coords    n1;                     /* normala trojuhelniku po transformaci */
    S_Coords    n2;                     /* normala trojuhelniku po transformaci */
    S_Coords    n3;                     /* normala trojuhelniku po transformaci */
    int         u1, v1, u2, v2, u3, v3; /* souradnice vrcholu po projekci do roviny obrazovky */
    S_Triangle  * triangle;

    IZG_ASSERT(pRenderer && pModel && i >= 0 && i < trivecSize(pModel->triangles));

    /* z modelu si vytahneme trojuhelnik */
    triangle = trivecGetPtr(pModel->triangles, i);

    /* transformace vrcholu matici model */
    trTransformVertex(&aa, cvecGetPtr(pModel->vertices, triangle->v[0]));
    trTransformVertex(&bb, cvecGetPtr(pModel->vertices, triangle->v[1]));
    trTransformVertex(&cc, cvecGetPtr(pModel->vertices, triangle->v[2]));

    /* promitneme vrcholy trojuhelniku na obrazovku */
    trProjectVertex(&u1, &v1, &aa);
    trProjectVertex(&u2, &v2, &bb);
    trProjectVertex(&u3, &v3, &cc);

    /* transformace normaly trojuhelniku matici model */
    trTransformVector(&nn, cvecGetPtr(pModel->trinormals, triangle->n));
    trTransformVector(&n1, cvecGetPtr(pModel->normals, triangle->v[0]));
    trTransformVector(&n2, cvecGetPtr(pModel->normals, triangle->v[1]));
    trTransformVector(&n3, cvecGetPtr(pModel->normals, triangle->v[2]));
    
    /* normalizace normaly */
    coordsNormalize(&nn);
    coordsNormalize(&n1);
    coordsNormalize(&n2);
    coordsNormalize(&n3);

    /* je troj. privraceny ke kamere, tudiz viditelny? */
    if( !renCalcVisibility(pRenderer, &aa, &nn) )
    {
        /* odvracene troj. vubec nekreslime */
        return;
    }

    /* rasterizace celeho trojuhelniku */
    studrenDrawTriangle(pRenderer,
                    &aa, &bb, &cc, &n1,&n2,&n3,
                    u1, v1, u2, v2, u3, v3
                    );
}

/*****************************************************************************
 *****************************************************************************/
