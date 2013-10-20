
namespace RecastFlags//originally the base library was dependent on the sample application. extracted required data
{

  enum QueryFlags {
    DEFAULT_MASK = 1u<<0,
    NAVMESH_MASK = 1u<<1,
    OBSTACLE_MASK= 1u<<2
  };

}
