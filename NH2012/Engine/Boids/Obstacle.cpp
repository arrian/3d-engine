/*
 * Copyright (C) 1996, Christopher John Kline
 * Electronic mail: ckline@acm.org
 *
 * This software may be freely copied, modified, and redistributed
 * for academic purposes and by not-for-profit organizations, provided that
 * this copyright notice is preserved on all copies, and that the source
 * code is included or notice is given informing the end-user that the source
 * code is publicly available under the terms described here.
 *
 * Persons or organizations wishing to use this code or any modified version
 * of this code in a commercial and/or for-profit manner must contact the
 * author via electronic mail (preferred) or other method to arrange the terms
 * of usage. These terms may be as simple as giving the author visible credit
 * in the final product.
 *
 * There is no warranty or other guarantee of fitness for this software,
 * it is provided solely "as is". Bug reports or fixes may be sent
 * to the author, who may or may not act on them as he desires.
 *
 * If you use this software the author politely requests that you inform him via
 * electronic mail. 
 *
 */

#include "Obstacle.h"
#include <limits.h>
#include <stdarg.h>

namespace Boids
{


//---------------- CLASS OBSTACLE ------------------------------


//---------------- CLASS POLYGON ------------------------------

int 
operator==(const Polygon &a, const Polygon &b) { 
    int i, test;
  
    if (a.numvertices != b.numvertices) return 0; // definitely different

    // Check if all vertices are the same vertices.
    for (i=0, test=1; test && (i < a.numvertices); i++)       
	test = test && (a.vertex[i]==b.vertex[i]);

    return(test);
}

Polygon::Polygon(const Polygon &p) {
  
    d = p.d;
    i1 = p.i1;
    i2 = p.i2;
    normal = p.normal;

    // copy all the vertices over
    numvertices = p.numvertices;
    vertex = new Vector[numvertices];
    for (int i = 0; i < numvertices; i++) 
	vertex[i] = p.vertex[i];
  
}

Polygon::Polygon(int numverts, const Vector v0, 
		 const Vector v1, const Vector v2, ...) {

  Vector p;
  va_list arg_ptr;
  int i;

  // Set the number of vertices in the polygon and allocate the array
  // of vertices
  numvertices = numverts;
  vertex = new Vector[numvertices];

  // set up the first 3 required vertices
  vertex[0] = v0; vertex[1] = v1; vertex[2] = v2;
  
  // cross product provides normal to poly
  normal = (vertex[1] - vertex[0]) % (vertex[2] - vertex[0]); 
  normal.Normalize();

  // plane of polygon is: (normal dot vertex[0]) + d = 0 
  d = -vertex[0] * normal;  
  
  // Find find dominant axis of normal vector, and put the indices 
  // of the plane perpendicular to that axis into i1 and i2.
  // This tells us what the closest major plane to the polygon is.
  // (i.e., i1==0 and i2==2 means closest plane is xz-plane)  
  if (fabs(normal.x) > fabs(normal.y) && fabs(normal.x) > fabs(normal.z)) {
    i1 = 1; i2 = 2;
  }
  else if (fabs(normal.y) >= fabs(normal.x) && fabs(normal.y) >= fabs(normal.z)) {
    i1 = 0; i2 = 2;    
  }
  else {
    i1 = 0; i2 = 1;   
  }

  // set up the rest of them
  va_start(arg_ptr, v2); 
  for (i=3; i<numverts; i++) {
    vertex[i] = va_arg(arg_ptr, Vector);
  }
  va_end(arg_ptr);

}

/*
 *  IntersectionWithRay():
 *  Determines if a given ray intersects the polygon. 
 *  
 *  Parameters:
 *   raydirection: direction of ray
 *   rayorigin:    origin of ray
 *   
 *  Returns:
 *   An ISectData structure, which describes whether or
 *   not an intersection actually occured, and if so, information
 *   about that intersection
*/

ISectData 
Polygon::IntersectionWithRay(const Vector &raydirection,
			     const Vector &rayorigin) const {

    /* 
       Adapted by Chris Kline 10/12/95 from :
     
       An Efficient Ray/Polygon Intersection
       by Didier Badouel
       from "Graphics Gems", Academic Press, 1990
       */

    ISectData data;
    data.intersectionflag = 0;  // no intersection as default

    Vector rdirection = Direction(raydirection);     // only want a unit vector
    
    // If ray has no direction, then try to handle this 
    // gracefully by returning with no intersection.
    if (rdirection.x == 0 &&
	rdirection.y == 0 &&
	rdirection.z == 0)
      return data;
  
    
    // Compute t, where the parametric representation of the ray r(t) is:
    //   r(t) = rayorigin + rdirection * t  
    // Using r(t) and the representation of the polygon's plane:
    //   (normal dot vertex[0]) + d = 0
    // we can calculate the value of t

  
    double denom = normal * rdirection;

    // If polygon is parallel to the ray, or contains the ray,
    // there is no _unique_ intersection.
    if (denom == 0) return(data); // return data indicating no intersection
  
    double t = - (d + normal*rayorigin) / denom;

    // If intersection is behind the ray's origin, there is no intersection
    if (t < 0) return (data);
    
    // Calculate the point of intersection 
    data.point = rayorigin + rdirection*t;
  
    // Determine if the point of intersection is inside the polygon 
    double u0 = data.point[i1] - vertex[0][i1]; 
    double v0 = data.point[i2] - vertex[0][i2];
    double u1, u2, v1, v2, alpha, beta;
    char inside = 0;  // is the intersection point inside the polygon?
    int  i = 2;       // loop counter

    // break the polygon up into triangles and check each one to see if the
    // point of intersection is within it.
    do {
	// The polygon is viewed as (n-2) triangles. 
	u1 = vertex[i-1][i1] - vertex[0][i1]; 
	v1 = vertex[i-1][i2] - vertex[0][i2];
	u2 = vertex[i  ][i1] - vertex[0][i1]; 
	v2 = vertex[i  ][i2] - vertex[0][i2];
    
	if (u1 == 0) {
	    beta = u0/u2;
	    if ((beta >= 0.0) && (beta <= 1.0)) {
		alpha = (v0 - beta*v2)/v1;
		inside = ((alpha >= 0.0) && (alpha+beta <= 1.0));
	    }
	} 
	else {
	    beta = (v0*u1 - u0*v1)/(v2*u1 - u2*v1);
	    if ((beta >= 0.0)&&(beta <= 1.0)) {
		alpha = (u0 - beta*u2)/u1;
		inside = ((alpha >= 0.0)&&((alpha+beta) <= 1.0));
	    }
	}
	i++;
    } while ((inside == 0) && (i < numvertices));
  
    // Ok, so there was an intersection with the polygon's plane, 
    // but we only care if the intersection was inside the polygon 
    data.intersectionflag = inside; 

    // make sure to return the normal of the poly that there the ray
    // might be intersecting. 
    data.normal = normal;

    return(data);  
}

std::ostream &
Polygon::Disp(std::ostream &strm) const {

  strm << "[Polygon] " << numvertices << " vertices: ";
  for (int i=0; i<numvertices; strm << vertex[i++] << " ");

  return strm;
}

//---------------- CLASS BOX------------------------------

Box::Box(const Box &b) {
  
  for (int i= 0; i < 6; i++) {
    side[i] = new Polygon(*(b.side[i]));
  }

  tlb = b.tlb;
  brf = b.brf;

  boundingSphere = new Sphere(*(b.boundingSphere));
}

Box::Box(const Vector &topLeftBackCorner, const Vector &bottomRightFrontCorner) {

  // When constructing the box, imagine that it is originally positioned
  // with the bottomRightFrontCorner at (0, 0, 0) and the topLeftBackCorner
  // at (width, height, length). The top and bottom sides are parallel to
  // the x-z plane; the left and right are parallel to the y-z plane, and
  // the back and front are parallel to the x-y plane.
  // Then it is translated by the amount (x, y, z) where x, y, and z are the
  // respective components of the bottomRightFrontCorner.

  tlb = topLeftBackCorner;
  brf = bottomRightFrontCorner;

  double width  = tlb.x - brf.x;
  double height = tlb.y - brf.y;
  double length = tlb.z - brf.z;

#ifdef DEBUG_OBSTACLES
  cerr << "Constructor for Box" << endl;
  cerr << "length = " << length << endl;
  cerr << "width = " << width << endl;
  cerr << "height = " << height << endl;
  cerr << "tlb = " << tlb << endl;
  cerr << "brf = " << brf << endl;
#endif
  
  // Top
  side[0] = new Polygon(4,
			brf + Vector(0,     height, 0),
			brf + Vector(0,     height, length),
			brf + Vector(width, height, length),
			brf + Vector(width, height, 0));
  
  // Bottom
  side[1] = new Polygon(4,
			brf,
			brf + Vector(0,     0, length),
			brf + Vector(width, 0, length),
			brf + Vector(width, 0, 0));
  
  // Left
  side[2] = new Polygon(4,
			brf + Vector(width, 0,      0),
			brf + Vector(width, height, 0),
			tlb,
			brf + Vector(width, 0,      length));
  
  // Right
  side[3] = new Polygon(4,
			brf + Vector(0,      0,      0),
			brf + Vector(0,      height, 0),
			tlb + Vector(-width, 0,      0),
			brf + Vector(0,      0,      length));

  // Front
  side[4] = new Polygon(4,
			brf,
			brf + Vector(0,     height, 0),
			brf + Vector(width, height, 0),
			brf + Vector(width, 0,      0));
  

  
  // Back
  side[5] = new Polygon(4,
			brf + Vector(0,     0,      length),
			brf + Vector(0,     height, length),
			brf + Vector(width, height, length),
			brf + Vector(width, 0,      length));
  
#ifdef DEBUG_OBSTACLES
  for (int i = 0; i < 6; i++)
    cerr << "side[" << i<< "] = " << *side[i] << endl;
#endif

  Vector center(brf.x + width/2,
		brf.y + height/2,
		brf.z + length/2);

  
  double radius = Magnitude(tlb - center);
  double temp = Magnitude(brf - center);
  if (temp > radius)
    radius = temp;
  
  boundingSphere = new Sphere(center, radius);

#ifdef DEBUG_OBSTACLES
  cerr << "Bounding sphere = " << *boundingSphere << endl;
#endif
  
}

ISectData 
Box::IntersectionWithRay(const Vector &raydirection,
			 const Vector &rayorigin) const {
        
  // Do a quick check to see if the ray intersected the bounding sphere of
  // this box. If not, then no need to check the sides of the box
  // individually. 
  ISectData data = boundingSphere->DoesRayIntersect(raydirection, rayorigin);
  if (data.intersectionflag == 0) {
#ifdef DEBUG_OBSTACLES
    cerr << "Missed Bounding Sphere\n";
#endif
    return data;
  }

  ISectData closestIntersect;
  double distToClosestIntersect =  DBL_MAX;

  closestIntersect.intersectionflag = 0;

  // Attempt to find the closest intersection with a side of the box, if
  // there actually is an intersection.
  for (int i = 0; i < 6; i++) {
#ifdef DEBUG_OBSTACLES
    cerr << "Checking Poly: " << *side[i] << endl;
#endif
    data = side[i]->DoesRayIntersect(raydirection, rayorigin);
    if (data.intersectionflag == 1) {
#ifdef DEBUG_OBSTACLES
      cerr << "Found Isect in box\n";
#endif
      // Find closest intersection, since the ray could intersect multiple
      // sides. 
      double temp = Magnitude(data.point - rayorigin);
      if (temp < distToClosestIntersect) {
	distToClosestIntersect = temp;
	closestIntersect = data;
#ifdef DEBUG_OBSTACLES
	cerr << "Closest intersect at " << closestIntersect.point
	     << " Normal " << closestIntersect.normal << "\n";
#endif
      }
    }
  }

  return closestIntersect;
}

//---------------- CLASS SPHERE------------------------------

ISectData 
Sphere::IntersectionWithRay(const Vector &raydirection,
			    const Vector &rayorigin) const {
    
    ISectData data;

    Vector rdirection = Direction(raydirection);     // only want a unit vector   

    // If the ray starts at the sphere's origin, then calculations
    // are trivial
    if (rayorigin == origin) {
      data.intersectionflag = 1; // The ray MUST intersect the sphere!
      data.point = radius*rdirection; 
      data.normal = Direction(origin - data.point);
      return data; // bail out early and save time
    }

    data.intersectionflag = 0;  // no intersection as default

    // If ray has no direction, then try to handle this 
    // gracefully by returning with no intersection.
    // This situation shouldn't happen, though
    if (rdirection == Vector(0,0,0)) return data;
    
    //  The stuff below is Adapted by Chris Kline 11/28/95 from :
    //
    //  Intersection Of A Ray With A Sphere
    //  by James Hultquist
    //  from "Graphics Gems", Academic Press, 1990
    // 
    // Modified to handle special case when the rayorigin is
    // inside the sphere, which Hultquist didn't consider (because
    // he was only interested in raytracing the outside of spheres).
    
    // Find component of the ray from rayorigin to the
    // sphere's origin in the direction specified by 
    // rdirection
    double v = (origin - rayorigin) * rdirection;

    // Find the square of the distance from the ray-sphere 
    // intersection to the point on the ray closest to the 
    // sphere's origin
    double d = 
	pow(radius, 2) - 
	pow(Magnitude(origin - rayorigin), 2) +
	pow(v, 2);

    // If d < 0 then there is no intersection
    if (d < 0) return data;

    // Otherwise find the intersection point
    if (Magnitude(rayorigin - origin) < radius) 
	data.point = (v + sqrt(d)) * rdirection + rayorigin; /* inside sphere */
    else 	
	data.point = (v - sqrt(d)) * rdirection + rayorigin; /* outside sphere */
	
    // An intersection with the ray occured
    data.intersectionflag = 1; 

    // set the normal at the point of ray-sphere intersection
    // (the outward-pointing normal) 	
    data.normal = Direction(data.point - origin);	

    return data;

}

//---------------- CLASS OBSTACLELIST------------------------------

Obstacle *
ObstacleList::Add(const Obstacle &o) {

    // Copies o and adds the copy to the front of the list. Returns a pointer
    // to the COPY (not o)

    obnode *n = new obnode;
  
    n->obj = o.Clone(); // tell object to make a copy of itself
    n->next = head;
    head = n;           // link in new obnode
    return(n->obj);
}

Obstacle *
ObstacleList::Delete(Obstacle *o) {

    // Deletes o from the list, and returns o if successful, or NULL
    // to indicate failure

    obnode *n, *prevn;

    // remove the object from the list of objects
    prevn = n = head;
    while (n) {
	if (n->obj == o) { 
	    (n == head) ? (head = n->next) : (prevn->next = n->next);
	    break;
	}
	prevn = n;
	n = n->next;
    }

    if (n) {
	delete n;
	return(o);
    }
    else {
	return(NULL); // failure
    }
}

}

